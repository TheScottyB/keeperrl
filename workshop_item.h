#pragma once

#include "item_type.h"
#include "cost_info.h"
#include "item_upgrade_info.h"
#include "view_id.h"
#include "tech_id.h"

struct WorkshopItem;

struct WorkshopItemCfg {
  WorkshopItem get(const ContentFactory*) const;
  ItemType SERIAL(item);
  double SERIAL(work) = 1;
  CostInfo SERIAL(cost);
  optional<TechId> SERIAL(tech);
  int SERIAL(batchSize) = 1;
  optional<TutorialHighlight> SERIAL(tutorialHighlight);
  optional<string> SERIAL(requireIngredient);
  bool SERIAL(notArtifact) = false;
  bool SERIAL(applyImmediately) = false;
  SERIALIZE_ALL(NAMED(item), OPTION(work), OPTION(cost), NAMED(tech), OPTION(batchSize), NAMED(tutorialHighlight), OPTION(requireIngredient), OPTION(notArtifact), OPTION(applyImmediately))
};

static_assert(std::is_nothrow_move_constructible<WorkshopItemCfg>::value, "T should be noexcept MoveConstructible");


struct WorkshopItem {
  ItemType SERIAL(type);
  string SERIAL(name);
  string SERIAL(pluralName);
  ViewId SERIAL(viewId);
  CostInfo SERIAL(cost);
  vector<string> SERIAL(description);
  int SERIAL(batchSize);
  double SERIAL(workNeeded);
  optional<TechId> SERIAL(techId);
  optional<TutorialHighlight> SERIAL(tutorialHighlight);
  optional<ItemUpgradeType> SERIAL(upgradeType);
  int SERIAL(maxUpgrades);
  optional<string> SERIAL(requireIngredient);
  bool SERIAL(notArtifact) = false;
  bool SERIAL(applyImmediately) = false;
  SERIALIZE_ALL(type, name, pluralName, viewId, cost, batchSize, workNeeded, techId, description, tutorialHighlight, upgradeType, maxUpgrades, requireIngredient, notArtifact, applyImmediately)
};

struct WorkshopQueuedItem {
  SERIALIZATION_CONSTRUCTOR(WorkshopQueuedItem)
  WorkshopQueuedItem(WorkshopItem item, int index, bool paid)
      : item(std::move(item)), indexInWorkshop(index), paid(paid) {}
  WorkshopQueuedItem(const WorkshopQueuedItem&) = delete;
  WorkshopQueuedItem(WorkshopQueuedItem&& o) = default;
  WorkshopQueuedItem& operator = (WorkshopQueuedItem&& o) = default;
  WorkshopItem SERIAL(item);
  int SERIAL(indexInWorkshop);
  double SERIAL(state) = 0;
  bool SERIAL(paid);
  vector<PItem> SERIAL(runes);
  SERIALIZE_ALL(item, runes, state, paid, indexInWorkshop)
};
