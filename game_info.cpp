#include "stdafx.h"
#include "game_info.h"
#include "creature.h"
#include "spell.h"
#include "creature_name.h"
#include "view_id.h"
#include "level.h"
#include "position.h"
#include "creature_attributes.h"
#include "view_object.h"
#include "spell_map.h"
#include "item.h"
#include "body.h"
#include "equipment.h"
#include "creature_debt.h"
#include "item_class.h"
#include "model.h"
#include "time_queue.h"
#include "game.h"
#include "content_factory.h"
#include "special_trait.h"
#include "automaton_part.h"
#include "container_range.h"
#include "experience_type.h"

CreatureInfo::CreatureInfo(const Creature* c)
    : viewId(c->getViewObject().getViewIdList()),
      uniqueId(c->getUniqueId()),
      name(c->getName().bare()),
      stackName(c->getName().stack()),
      bestAttack(c->getBestAttack(c->getGame()->getContentFactory())),
      morale(c->getMorale()) {
}

string PlayerInfo::getFirstName() const {
  if (!firstName.empty())
    return firstName;
  else
    return capitalFirst(name);
}

ImmigrantCreatureInfo getImmigrantCreatureInfo(const Creature* c, const ContentFactory* factory) {
  vector<ImmigrantCreatureInfo::TrainingInfo> limits;
  auto maxExp = c->getAttributes().getMaxExpLevel();
  for (auto expType : ENUM_ALL(ExperienceType))
    if (maxExp[expType] > 0) {
      limits.push_back(ImmigrantCreatureInfo::TrainingInfo { expType, maxExp[expType] });
      for (auto attr : getAttrIncreases()[expType])
        limits.back().attributes.push_back(factory->attrInfo.at(attr).viewId);
    }
  return ImmigrantCreatureInfo {
    c->getName().bare(),
    c->getViewObject().getViewIdList(),
    AttributeInfo::fromCreature(factory, c),
    c->getAttributes().getSpellSchools().transform([](auto id) -> string { return id.data(); }),
    std::move(limits)
  };
}

vector<ItemAction> getItemActions(const Creature* c, const vector<Item*>& item) {
  PROFILE;
  vector<ItemAction> actions;
  if (c->equip(item[0]))
    actions.push_back(ItemAction::EQUIP);
  if (c->applyItem(item[0]))
    actions.push_back(ItemAction::APPLY);
  if (c->unequip(item[0]))
    actions.push_back(ItemAction::UNEQUIP);
  else {
    actions.push_back(ItemAction::THROW);
    actions.push_back(ItemAction::DROP);
    if (item.size() > 1)
      actions.push_back(ItemAction::DROP_MULTI);
    if (item[0]->getShopkeeper(c))
      actions.push_back(ItemAction::PAY);
    if (c->getPosition().isValid())
      for (Position v : c->getPosition().neighbors8())
        if (Creature* other = v.getCreature())
          if (c->isFriend(other)/* && c->canTakeItems(item)*/) {
            actions.push_back(ItemAction::GIVE);
            break;
          }
  }
  actions.push_back(ItemAction::NAME);
  return actions;
}

bool ItemInfo::representsSteed() const {
  return actions.contains(ItemAction::DROP_STEED) || actions.contains(ItemAction::REPLACE_STEED);
}

ItemInfo ItemInfo::get(const Creature* creature, const vector<Item*>& stack, const ContentFactory* factory) {
  PROFILE;
  return CONSTRUCT(ItemInfo,
    c.name = stack[0]->getShortName(factory, creature, stack.size() > 1);
    c.fullName = stack[0]->getNameAndModifiers(factory, false, creature);
    c.description = (creature && creature->isAffected(LastingEffect::BLIND))
        ? vector<string>() : stack[0]->getDescription(factory);
    c.number = stack.size();
    c.viewId = stack[0]->getViewObject().getViewIdList();
    c.viewIdModifiers = stack[0]->getViewObject().getAllModifiers();
    for (auto it : stack)
      c.ids.insert(it->getUniqueId());
    if (creature)
      c.actions = getItemActions(creature, stack);
    c.equiped = creature && creature->getEquipment().isEquipped(stack[0]);
    c.weight = stack[0]->getWeight();
    if (creature && stack[0]->getShopkeeper(creature))
      c.price = make_pair(ViewId("gold"), stack[0]->getPrice());
  );
}

static vector<ItemInfo> fillIntrinsicAttacks(const Creature* c, const ContentFactory* factory) {
  vector<ItemInfo> ret;
  auto& intrinsicAttacks = c->getBody().getIntrinsicAttacks();
  for (auto part : ENUM_ALL(BodyPart))
    for (auto& attack : intrinsicAttacks[part]) {
      CHECK(!!attack.item) << "Intrinsic attacks not initialized for " << c->getName().bare() << " "
          << EnumInfo<BodyPart>::getString(part);
      ret.push_back(ItemInfo::get(c, {attack.item.get()}, factory));
      auto& item = ret.back();
      item.weight.reset();
      if (!c->getBody().numGood(part)) {
        item.unavailable = true;
        item.unavailableReason = "No functional body part: "_s + getName(part);
        item.actions.clear();
      } else {
        item.intrinsicAttackState = attack.active ? ItemInfo::ACTIVE : ItemInfo::INACTIVE;
        item.intrinsicExtraAttack = attack.isExtraAttack;
        item.actions = {attack.active ? ItemAction::INTRINSIC_DEACTIVATE : ItemAction::INTRINSIC_ACTIVATE};
      }
    }
  return ret;
}

static vector<ItemInfo> getItemInfos(const Creature* c, const vector<Item*>& items, const ContentFactory* factory) {
  map<string, vector<Item*> > stacks = groupBy<Item*, string>(items,
      [&] (Item* const& item) {
          return item->getNameAndModifiers(factory, false, c) + (c->getEquipment().isEquipped(item) ? "(e)" : ""); });
  vector<ItemInfo> ret;
  for (auto elem : stacks)
    ret.push_back(ItemInfo::get(c, elem.second, factory));
  return ret;
}

SpellSchoolInfo fillSpellSchool(const Creature* c, SpellSchoolId id, const ContentFactory* factory) {
  SpellSchoolInfo ret;
  auto& spellSchool = factory->getCreatures().getSpellSchools().at(id);
  ret.name = id.data();
  ret.experienceType = spellSchool.expType;
  for (auto& id : spellSchool.spells) {
    auto spell = factory->getCreatures().getSpell(id.first);
    ret.spells.push_back(
        SpellInfo {
          spell->getName(factory),
          spell->getSymbol(),
          id.second,
          !c || c->getAttributes().getExpLevel(spellSchool.expType) >= id.second,
          {spell->getDescription(factory)},
          none
        });
  }
  return ret;
}

PlayerInfo::PlayerInfo(const Creature* c, const ContentFactory* contentFactory)
    : bestAttack(c->getBestAttack(contentFactory))   {
  firstName = c->getName().firstOrBare();
  name = c->getName().bare();
  title = c->getName().title();
  description = capitalFirst(c->getAttributes().getDescription(contentFactory));
  viewId = c->getViewObject().getViewIdList();
  morale = c->getMorale(contentFactory);
  positionHash = c->getPosition().getHash();
  creatureId = c->getUniqueId();
  attributes = AttributeInfo::fromCreature(contentFactory, c);
  experienceInfo = getCreatureExperienceInfo(contentFactory, c);
  for (auto& id : c->getAttributes().getSpellSchools())
    spellSchools.push_back(fillSpellSchool(c, id, contentFactory));
  intrinsicAttacks = fillIntrinsicAttacks(c, contentFactory);
  kills = c->getKills().transform([&](auto& info){ return info.viewId; });
  killTitles = c->getKillTitles();
  aiType = c->getAttributes().getAIType();
  effects.clear();
  for (auto& adj : c->getBadAdjectives(contentFactory))
    effects.push_back({adj.name, adj.help, true});
  for (auto& adj : c->getGoodAdjectives(contentFactory))
    effects.push_back({adj.name, adj.help, false});
  spells.clear();
  for (auto spell : c->getSpellMap().getAvailable(c))
    spells.push_back({
        spell->getName(contentFactory),
        spell->getSymbol(),
        none,
        true,
        spell->getDescription(contentFactory),
        c->isReady(spell) ? none : optional<TimeInterval>(c->getSpellDelay(spell)),
        false
    });
  carryLimit = c->getBody().getCarryLimit();
  map<ItemClass, vector<Item*> > typeGroups = groupBy<Item*, ItemClass>(
      c->getEquipment().getItems(), [](Item* const& item) { return item->getClass();});
  debt = c->getDebt().getTotal(c);
  for (auto elem : ENUM_ALL(ItemClass))
    if (typeGroups[elem].size() > 0)
      append(inventory, getItemInfos(c, typeGroups[elem], contentFactory));
  if (c->getPosition().isValid())
    moveCounter = c->getPosition().getModel()->getMoveCounter();
  isPlayerControlled = c->isPlayer();
}

const CreatureInfo* CollectiveInfo::getMinion(UniqueEntity<Creature>::Id id) const {
  for (auto& elem : minions)
    if (elem.uniqueId == id)
      return &elem;
  return nullptr;
}


vector<AttributeInfo> AttributeInfo::fromCreature(const ContentFactory* contentFactory, const Creature* c) {
  PROFILE;
  vector<AttributeInfo> ret;
  for (auto& attr : contentFactory->attrOrder) {
    auto& info = contentFactory->attrInfo.at(attr);
    ret.push_back(AttributeInfo {
        capitalFirst(info.name),
        info.viewId,
        c->getRawAttr(attr),
        c->getAttrBonus(attr, true),
        info.help
    });
  }
  return ret;  
}

STRUCT_IMPL(ImmigrantDataInfo)
ImmigrantDataInfo::ImmigrantDataInfo() {}
HASH_DEF(ImmigrantDataInfo, requirements, info, creature, count, timeLeft, id, autoState, cost, generatedTime, keybinding, tutorialHighlight, specialTraits)
