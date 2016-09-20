#include "stdafx.h"
#include "util.h"
#include "creature.h"
#include "collective_config.h"
#include "tribe.h"
#include "game.h"
#include "technology.h"
#include "collective_warning.h"
#include "item_type.h"
#include "resource_id.h"
#include "inventory.h"
#include "workshops.h"
#include "lasting_effect.h"
#include "item.h"
#include "square_type.h"
#include "view_id.h"
#include "furniture_type.h"
#include "spawn_type.h"
#include "minion_task.h"
#include "furniture_usage.h"
#include "creature_attributes.h"

AttractionInfo::AttractionInfo(MinionAttraction a, double cl, double min, bool mand)
  : attraction(a), amountClaimed(cl), minAmount(min), mandatory(mand) {}

template <class Archive>
void CollectiveConfig::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(immigrantFrequency)
    & SVAR(payoutTime)
    & SVAR(payoutMultiplier)
    & SVAR(maxPopulation)
    & SVAR(populationIncreases)
    & SVAR(immigrantInfo)
    & SVAR(type)
    & SVAR(recruitingMinPopulation)
    & SVAR(leaderAsFighter)
    & SVAR(spawnGhosts)
    & SVAR(ghostProb)
    & SVAR(guardianInfo);
}

SERIALIZABLE(CollectiveConfig);
SERIALIZATION_CONSTRUCTOR_IMPL(CollectiveConfig);

template <class Archive>
void ImmigrantInfo::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(id)
    & SVAR(frequency)
    & SVAR(attractions)
    & SVAR(traits)
    & SVAR(spawnAtDorm)
    & SVAR(salary)
    & SVAR(techId)
    & SVAR(limit)
    & SVAR(groupSize)
    & SVAR(autoTeam)
    & SVAR(ignoreSpawnType);
}

SERIALIZABLE(ImmigrantInfo);

template <class Archive>
void AttractionInfo::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(attraction)
    & SVAR(amountClaimed)
    & SVAR(minAmount)
    & SVAR(mandatory);
}

SERIALIZABLE(AttractionInfo);
SERIALIZATION_CONSTRUCTOR_IMPL(AttractionInfo);

template <class Archive>
void PopulationIncrease::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(type)
    & SVAR(increasePerSquare)
    & SVAR(maxIncrease);
}

SERIALIZABLE(PopulationIncrease);

template <class Archive>
void GuardianInfo::serialize(Archive& ar, const unsigned int version) {
  ar& SVAR(creature)
    & SVAR(probability)
    & SVAR(minEnemies)
    & SVAR(minVictims);
}

SERIALIZABLE(GuardianInfo);

CollectiveConfig::CollectiveConfig(double freq, int payoutT, double payoutM, vector<ImmigrantInfo> im,
    CollectiveType t, int maxPop, vector<PopulationIncrease> popInc)
    : immigrantFrequency(freq), payoutTime(payoutT), payoutMultiplier(payoutM),
    maxPopulation(maxPop), populationIncreases(popInc), immigrantInfo(im), type(t) {}

CollectiveConfig CollectiveConfig::keeper(double freq, int payout, double payoutMult, int maxPopulation,
    vector<PopulationIncrease> increases, vector<ImmigrantInfo> im) {
  return CollectiveConfig(freq, payout, payoutMult, im, KEEPER, maxPopulation, increases);
}

CollectiveConfig CollectiveConfig::withImmigrants(double frequency, int maxPopulation, vector<ImmigrantInfo> im) {
  return CollectiveConfig(frequency, 0, 0, im, VILLAGE, maxPopulation, {});
}

CollectiveConfig CollectiveConfig::noImmigrants() {
  return CollectiveConfig(0, 0, 0, {}, VILLAGE, 10000, {});
}

CollectiveConfig& CollectiveConfig::setLeaderAsFighter() {
  leaderAsFighter = true;
  return *this;
}

CollectiveConfig& CollectiveConfig::setGhostSpawns(double prob, int num) {
  ghostProb = prob;
  spawnGhosts = num;
  return *this;
}

int CollectiveConfig::getNumGhostSpawns() const {
  return spawnGhosts;
}

double CollectiveConfig::getGhostProb() const {
  return ghostProb;
}

bool CollectiveConfig::isLeaderFighter() const {
  return leaderAsFighter;
}

bool CollectiveConfig::getManageEquipment() const {
  return type == KEEPER;
}

bool CollectiveConfig::getWorkerFollowLeader() const {
  return type == KEEPER;
}

bool CollectiveConfig::sleepOnlyAtNight() const {
  return type != KEEPER;
}

bool CollectiveConfig::activeImmigrantion(const Game* game) const {
  return type == KEEPER || game->isSingleModel();
}

double CollectiveConfig::getImmigrantFrequency() const {
  return immigrantFrequency;
}

int CollectiveConfig::getPayoutTime() const {
  return payoutTime;
}
 
double CollectiveConfig::getPayoutMultiplier() const {
  return payoutMultiplier;
}

bool CollectiveConfig::getStripSpawns() const {
  return type == KEEPER;
}

bool CollectiveConfig::getFetchItems() const {
  return type == KEEPER;
}

bool CollectiveConfig::getEnemyPositions() const {
  return type == KEEPER;
}

bool CollectiveConfig::getWarnings() const {
  return type == KEEPER;
}

bool CollectiveConfig::getConstructions() const {
  return type == KEEPER;
}

int CollectiveConfig::getMaxPopulation() const {
  return maxPopulation;
}

optional<int> CollectiveConfig::getRecruitingMinPopulation() const {
  return recruitingMinPopulation;
}

CollectiveConfig& CollectiveConfig::allowRecruiting(int minPop) {
  recruitingMinPopulation = minPop;
  return *this;
}

const vector<ImmigrantInfo>& CollectiveConfig::getImmigrantInfo() const {
  return immigrantInfo;
}

const vector<PopulationIncrease>& CollectiveConfig::getPopulationIncreases() const {
  return populationIncreases;
}

CollectiveConfig& CollectiveConfig::setGuardian(GuardianInfo info) {
  guardianInfo = info;
  return *this;
}

const optional<GuardianInfo>& CollectiveConfig::getGuardianInfo() const {
  return guardianInfo;
}

vector<BirthSpawn> CollectiveConfig::getBirthSpawns() const {
  if (type == KEEPER)
    return {{ CreatureId::GOBLIN, 1 },
      { CreatureId::ORC, 1 },
      { CreatureId::ORC_SHAMAN, 0.5 },
      { CreatureId::HARPY, 0.5 },
      { CreatureId::OGRE, 0.5 },
      { CreatureId::WEREWOLF, 0.5 },
      { CreatureId::SPECIAL_HM, 1.0, TechId::HUMANOID_MUT},
      { CreatureId::SPECIAL_BM, 1.0, TechId::BEAST_MUT },
    };
  else 
    return {};
}

const EnumMap<SpawnType, DormInfo>& CollectiveConfig::getDormInfo() const {
  static EnumMap<SpawnType, DormInfo> dormInfo ([](SpawnType type) -> DormInfo {
      switch (type) {
        case SpawnType::HUMANOID: return {FurnitureType::BED, CollectiveWarning::BEDS};
        case SpawnType::UNDEAD: return {FurnitureType::GRAVE};
        case SpawnType::BEAST: return {FurnitureType::BEAST_CAGE};
        case SpawnType::DEMON: return {FurnitureType::DEMON_SHRINE};
      }
  });
  return dormInfo;
}

const vector<FurnitureType>& CollectiveConfig::getRoomsNeedingLight() const {
  static vector<FurnitureType> ret {
    FurnitureType::WORKSHOP,
    FurnitureType::FORGE,
    FurnitureType::LABORATORY,
    FurnitureType::JEWELER,
    FurnitureType::STEEL_FURNACE,
    FurnitureType::TRAINING_WOOD,
    FurnitureType::TRAINING_IRON,
    FurnitureType::TRAINING_STEEL,
    FurnitureType::BOOK_SHELF};
  return ret;
};

const static auto resourceStorage = FurnitureType::STOCKPILE_RES;
const static auto equipmentStorage = FurnitureType::STOCKPILE_EQUIP;

const FurnitureType& CollectiveConfig::getEquipmentStorage() {
  return equipmentStorage;
}

const FurnitureType& CollectiveConfig::getResourceStorage() {
  return resourceStorage;
}

const vector<FloorInfo>& CollectiveConfig::getFloors() {
  static vector<FloorInfo> ret = {
    {SquareType{SquareId::CUSTOM_FLOOR, CustomFloorInfo{ViewId::WOOD_FLOOR1, "wooden floor"}},
        {CollectiveResourceId::WOOD, 10}, "Wooden", 0.02},
    {SquareType{SquareId::CUSTOM_FLOOR, CustomFloorInfo{ViewId::WOOD_FLOOR2, "wooden floor"}},
        {CollectiveResourceId::WOOD, 10}, "Wooden", 0.02},
    {SquareType{SquareId::CUSTOM_FLOOR, CustomFloorInfo{ViewId::STONE_FLOOR1, "stone floor"}},
        {CollectiveResourceId::STONE, 10}, "Stone", 0.04},
    {SquareType{SquareId::CUSTOM_FLOOR, CustomFloorInfo{ViewId::STONE_FLOOR2, "stone floor"}},
        {CollectiveResourceId::STONE, 10}, "Stone", 0.04},
    {SquareType{SquareId::CUSTOM_FLOOR, CustomFloorInfo{ViewId::CARPET_FLOOR1, "carpet floor"}},
        {CollectiveResourceId::GOLD, 10}, "Carpet", 0.06},
    {SquareType{SquareId::CUSTOM_FLOOR, CustomFloorInfo{ViewId::CARPET_FLOOR2, "carpet floor"}},
        {CollectiveResourceId::GOLD, 10}, "Carpet", 0.06},
  };
  return ret;
}

double CollectiveConfig::getEfficiencyBonus(SquareType type) {
  switch (type.getId()) {
    case SquareId::CUSTOM_FLOOR:
      for (auto& info : getFloors())
        if (info.type == type)
          return info.efficiencyBonus;
      FAIL << "Efficiency data not found for floor " << type.get<CustomFloorInfo>().name;
      return 0;
    case SquareId::DUNGEON_WALL:
      return 0.04;
    default:
      return 0;
  }
}

bool CollectiveConfig::canBuildOutsideTerritory(FurnitureType type) {
  switch (type) {
    case FurnitureType::EYEBALL:
    case FurnitureType::KEEPER_BOARD:
    case FurnitureType::BRIDGE: return true;
    default: return false;
  }
}

const ResourceInfo& CollectiveConfig::getResourceInfo(CollectiveResourceId id) {
  static EnumMap<CollectiveResourceId, ResourceInfo> resourceInfo([](CollectiveResourceId id)->ResourceInfo {
    switch (id) {
      case CollectiveResourceId::MANA:
        return { none, none, ItemId::GOLD_PIECE, "mana", ViewId::MANA};
      case CollectiveResourceId::PRISONER_HEAD:
        return { none, none, ItemId::GOLD_PIECE, "", ViewId::IMPALED_HEAD, true};
      case CollectiveResourceId::GOLD:
        return {FurnitureType::TREASURE_CHEST, ItemIndex::GOLD, ItemId::GOLD_PIECE, "gold", ViewId::GOLD};
      case CollectiveResourceId::WOOD:
        return { resourceStorage, ItemIndex::WOOD, ItemId::WOOD_PLANK, "wood", ViewId::WOOD_PLANK};
      case CollectiveResourceId::IRON:
        return { resourceStorage, ItemIndex::IRON, ItemId::IRON_ORE, "iron", ViewId::IRON_ROCK};
      case CollectiveResourceId::STEEL:
        return { resourceStorage, ItemIndex::STEEL, ItemId::STEEL_INGOT, "steel", ViewId::STEEL_INGOT};
      case CollectiveResourceId::STONE:
        return { resourceStorage, ItemIndex::STONE, ItemId::ROCK, "granite", ViewId::ROCK};
      case CollectiveResourceId::CORPSE:
        return { FurnitureType::GRAVE, ItemIndex::REVIVABLE_CORPSE, ItemId::GOLD_PIECE, "corpses", ViewId::BODY_PART, true};
    }
  });
  return resourceInfo[id];
}

MinionTaskInfo::MinionTaskInfo(Type t, const string& desc, optional<CollectiveWarning> w)
    : type(t), description(desc), warning(w) {
  CHECK(type != FURNITURE);
}

MinionTaskInfo::MinionTaskInfo() {}

MinionTaskInfo::MinionTaskInfo(FurnitureType type, const string& desc) : type(FURNITURE),
  furniturePredicate([type](const Creature*, FurnitureType t) { return t == type;}),
    description(desc) {
}

MinionTaskInfo::MinionTaskInfo(UsagePredicate pred, const string& desc) : type(FURNITURE),
  furniturePredicate(pred), description(desc) {
}

static EnumMap<WorkshopType, WorkshopInfo> workshops([](WorkshopType type)->WorkshopInfo {
  switch (type) {
    case WorkshopType::WORKSHOP: return {FurnitureType::WORKSHOP, MinionTask::WORKSHOP, "workshop"};
    case WorkshopType::FORGE: return {FurnitureType::FORGE, MinionTask::FORGE, "forge"};
    case WorkshopType::LABORATORY: return {FurnitureType::LABORATORY, MinionTask::LABORATORY, "laboratory"};
    case WorkshopType::JEWELER: return {FurnitureType::JEWELER, MinionTask::JEWELER, "jeweler"};
    case WorkshopType::STEEL_FURNACE: return {FurnitureType::STEEL_FURNACE, MinionTask::STEEL_FURNACE, "steel furnace"};
  }});

optional<WorkshopType> CollectiveConfig::getWorkshopType(MinionTask task) {
  static optional<EnumMap<MinionTask, optional<WorkshopType>>> map;
  if (!map) {
    map.emplace();
    for (auto type : ENUM_ALL(WorkshopType))
      (*map)[workshops[type].minionTask] = type;
  }
  return (*map)[task];
}

optional<WorkshopType> CollectiveConfig::getWorkshopType(FurnitureType furniture) {
  static optional<EnumMap<FurnitureType, optional<WorkshopType>>> map;
  if (!map) {
    map.emplace();
    for (auto type : ENUM_ALL(WorkshopType))
      (*map)[workshops[type].furniture] = type;
  }
  return (*map)[furniture];
}

optional<int> CollectiveConfig::getTrainingMaxLevelIncrease(FurnitureType type) {
  switch (type) {
    case FurnitureType::TRAINING_WOOD:
      return 3;
    case FurnitureType::TRAINING_IRON:
      return 7;
    case FurnitureType::TRAINING_STEEL:
      return 12;
    default:
      return none;
  }
}

const MinionTaskInfo& CollectiveConfig::getTaskInfo(MinionTask task) {
  static EnumMap<MinionTask, MinionTaskInfo> map([](MinionTask task) -> MinionTaskInfo {
    switch (task) {
      case MinionTask::TRAIN: return {[](const Creature* c, FurnitureType t) {
            if (auto maxIncrease = CollectiveConfig::getTrainingMaxLevelIncrease(t))
              return !c || c->getAttributes().getExpLevel() < *maxIncrease + c->getAttributes().getBaseExpLevel();
            else
              return false;
          }, "training"};
      case MinionTask::SLEEP: return {FurnitureType::BED, "sleeping"};
      case MinionTask::EAT: return {MinionTaskInfo::EAT, "eating"};
      case MinionTask::GRAVE: return {FurnitureType::GRAVE, "sleeping"};
      case MinionTask::LAIR: return {FurnitureType::BEAST_CAGE, "sleeping"};
      case MinionTask::THRONE: return {FurnitureType::THRONE, "throne"};
      case MinionTask::STUDY: return {FurnitureType::BOOK_SHELF, "studying"};
      case MinionTask::PRISON: return {FurnitureType::PRISON, "prison"};
      case MinionTask::CROPS: return {FurnitureType::CROPS, "crops"};
      case MinionTask::RITUAL: return {FurnitureType::DEMON_SHRINE, "rituals"};
      case MinionTask::COPULATE: return {MinionTaskInfo::COPULATE, "copulation"};
      case MinionTask::CONSUME: return {MinionTaskInfo::CONSUME, "consumption"};
      case MinionTask::EXPLORE: return {MinionTaskInfo::EXPLORE, "spying"};
      case MinionTask::SPIDER: return {MinionTaskInfo::SPIDER, "spider"};
      case MinionTask::EXPLORE_NOCTURNAL: return {MinionTaskInfo::EXPLORE, "spying"};
      case MinionTask::EXPLORE_CAVES: return {MinionTaskInfo::EXPLORE, "spying"};
      case MinionTask::EXECUTE: return {FurnitureType::PRISON, "execution ordered"};
      case MinionTask::BE_WHIPPED: return {FurnitureType::WHIPPING_POST, "being whipped"};
      case MinionTask::BE_TORTURED: return {FurnitureType::TORTURE_TABLE, "being tortured"};
      case MinionTask::WORKSHOP:
      case MinionTask::FORGE:
      case MinionTask::LABORATORY:
      case MinionTask::JEWELER:
      case MinionTask::STEEL_FURNACE: {
          auto& info = workshops[*CollectiveConfig::getWorkshopType(task)];
          return MinionTaskInfo(info.furniture, info.taskName);
        }
    }
  });
  return map[task];
}

const WorkshopInfo& CollectiveConfig::getWorkshopInfo(WorkshopType type) {
  return workshops[type];
}


unique_ptr<Workshops> CollectiveConfig::getWorkshops() const {
  return unique_ptr<Workshops>(new Workshops({
      {WorkshopType::WORKSHOP, {
          Workshops::Item::fromType(ItemId::FIRST_AID_KIT, 1, {CollectiveResourceId::WOOD, 20}),
          Workshops::Item::fromType(ItemId::LEATHER_ARMOR, 6, {CollectiveResourceId::WOOD, 100}),
          Workshops::Item::fromType(ItemId::LEATHER_HELM, 1, {CollectiveResourceId::WOOD, 30}),
          Workshops::Item::fromType(ItemId::LEATHER_BOOTS, 2, {CollectiveResourceId::WOOD, 50}),
          Workshops::Item::fromType(ItemId::LEATHER_GLOVES, 1, {CollectiveResourceId::WOOD, 10}),
          Workshops::Item::fromType(ItemId::CLUB, 3, {CollectiveResourceId::WOOD, 50}),
          Workshops::Item::fromType(ItemId::HEAVY_CLUB, 5, {CollectiveResourceId::WOOD, 100})
                  .setTechId(TechId::TWO_H_WEAP),
          Workshops::Item::fromType(ItemId::BOW, 13, {CollectiveResourceId::WOOD, 100}).setTechId(TechId::ARCHERY),
          Workshops::Item::fromType(ItemId::ARROW, 5, {CollectiveResourceId::WOOD, 50})
                  .setBatchSize(20).setTechId(TechId::ARCHERY),
          Workshops::Item::fromType(ItemId::BOULDER_TRAP_ITEM, 20, {CollectiveResourceId::STONE, 250})
                  .setTechId(TechId::TRAPS),
          Workshops::Item::fromType({ItemId::TRAP_ITEM,
              TrapInfo({TrapType::POISON_GAS, EffectId::EMIT_POISON_GAS})}, 10, {CollectiveResourceId::WOOD, 100})
                  .setTechId(TechId::TRAPS),
          Workshops::Item::fromType({ItemId::TRAP_ITEM,
              TrapInfo({TrapType::ALARM, EffectId::ALARM})}, 8, {CollectiveResourceId::WOOD, 100})
                  .setTechId(TechId::TRAPS),
          Workshops::Item::fromType({ItemId::TRAP_ITEM, TrapInfo({TrapType::WEB,
                EffectType(EffectId::LASTING, LastingEffect::ENTANGLED)})}, 8, {CollectiveResourceId::WOOD, 100})
                  .setTechId(TechId::TRAPS),
          Workshops::Item::fromType({ItemId::TRAP_ITEM,
              TrapInfo({TrapType::SURPRISE, EffectId::TELE_ENEMIES})}, 8, {CollectiveResourceId::WOOD, 100})
                  .setTechId(TechId::TRAPS),
          Workshops::Item::fromType({ItemId::TRAP_ITEM, TrapInfo({TrapType::TERROR,
                EffectType(EffectId::LASTING, LastingEffect::PANIC)})}, 8, {CollectiveResourceId::WOOD, 100})
                  .setTechId(TechId::TRAPS),
      }},
      {WorkshopType::FORGE, {
          Workshops::Item::fromType(ItemId::SWORD, 10, {CollectiveResourceId::IRON, 100}),
          Workshops::Item::fromType(ItemId::STEEL_SWORD, 20, {CollectiveResourceId::STEEL, 2})
                  .setTechId(TechId::STEEL_MAKING),
          Workshops::Item::fromType(ItemId::CHAIN_ARMOR, 30, {CollectiveResourceId::IRON, 200}),
          Workshops::Item::fromType(ItemId::STEEL_ARMOR, 60, {CollectiveResourceId::STEEL, 5}),
          Workshops::Item::fromType(ItemId::IRON_HELM, 8, {CollectiveResourceId::IRON, 80}),
          Workshops::Item::fromType(ItemId::IRON_BOOTS, 12, {CollectiveResourceId::IRON, 120}),
          Workshops::Item::fromType(ItemId::WAR_HAMMER, 16, {CollectiveResourceId::IRON, 190})
                  .setTechId(TechId::TWO_H_WEAP),
          Workshops::Item::fromType(ItemId::BATTLE_AXE, 22, {CollectiveResourceId::IRON, 250})
                  .setTechId(TechId::TWO_H_WEAP),
          Workshops::Item::fromType(ItemId::STEEL_BATTLE_AXE, 44, {CollectiveResourceId::STEEL, 6})
                  .setTechId(TechId::STEEL_MAKING),
      }},
      {WorkshopType::LABORATORY, {
          Workshops::Item::fromType({ItemId::POTION, EffectType{EffectId::LASTING, LastingEffect::SLOWED}}, 2,
              {CollectiveResourceId::MANA, 10}),
          Workshops::Item::fromType({ItemId::POTION, EffectType{EffectId::LASTING, LastingEffect::SLEEP}}, 2,
              {CollectiveResourceId::MANA, 10}),
          Workshops::Item::fromType({ItemId::POTION, EffectId::HEAL}, 4, {CollectiveResourceId::MANA, 30}),
          Workshops::Item::fromType({ItemId::POTION,
              EffectType{EffectId::LASTING, LastingEffect::POISON_RESISTANT}}, 3, {CollectiveResourceId::MANA, 30}),
          Workshops::Item::fromType({ItemId::POTION,
              EffectType{EffectId::LASTING, LastingEffect::POISON}}, 2, {CollectiveResourceId::MANA, 30}),
          Workshops::Item::fromType({ItemId::POTION,
              EffectType{EffectId::LASTING, LastingEffect::SPEED}}, 4, {CollectiveResourceId::MANA, 30})
                  .setTechId(TechId::ALCHEMY_ADV),
          Workshops::Item::fromType({ItemId::POTION,
              EffectType{EffectId::LASTING, LastingEffect::BLIND}}, 4, {CollectiveResourceId::MANA, 50})
                  .setTechId(TechId::ALCHEMY_ADV),
          Workshops::Item::fromType({ItemId::POTION,
              EffectType{EffectId::LASTING, LastingEffect::FLYING}}, 6, {CollectiveResourceId::MANA, 80})
                  .setTechId(TechId::ALCHEMY_ADV),
          Workshops::Item::fromType({ItemId::POTION,
              EffectType{EffectId::LASTING, LastingEffect::INVISIBLE}}, 6, {CollectiveResourceId::MANA, 200})
                  .setTechId(TechId::ALCHEMY_ADV),
      }},
      {WorkshopType::JEWELER, {
          Workshops::Item::fromType({ItemId::RING, LastingEffect::POISON_RESISTANT}, 10,
              {CollectiveResourceId::GOLD, 100}),
          Workshops::Item::fromType({ItemId::RING, LastingEffect::FIRE_RESISTANT}, 10,
              {CollectiveResourceId::GOLD, 150}),
          Workshops::Item::fromType(ItemId::WARNING_AMULET, 10, {CollectiveResourceId::GOLD, 150}),
          Workshops::Item::fromType(ItemId::DEFENSE_AMULET, 10, {CollectiveResourceId::GOLD, 200}),
          Workshops::Item::fromType(ItemId::HEALING_AMULET, 10, {CollectiveResourceId::GOLD, 300}),
      }},
      {WorkshopType::STEEL_FURNACE, {
          Workshops::Item::fromType(ItemId::STEEL_INGOT, 5, {CollectiveResourceId::IRON, 50}),
      }},
  }));
}

