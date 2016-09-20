/* Copyright (C) 2013-2014 Michal Brzozowski (rusolis@poczta.fm)

   This file is part of KeeperRL.

   KeeperRL is free software; you can redistribute it and/or modify it under the terms of the
   GNU General Public License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   KeeperRL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.
   If not, see http://www.gnu.org/licenses/ . */
#pragma once

#include "enum_variant.h"
#include "square_type.h"
#include "util.h"
#include "minion_trait.h"
#include "workshop_type.h"
#include "cost_info.h"

enum class ItemClass;

class Game;
class Workshops;

enum class AttractionId {
  FURNITURE,
  ITEM_INDEX,
};

class MinionAttraction : public EnumVariant<AttractionId, TYPES(FurnitureType, ItemIndex),
    ASSIGN(FurnitureType, AttractionId::FURNITURE),
    ASSIGN(ItemIndex, AttractionId::ITEM_INDEX)> {
  using EnumVariant::EnumVariant;
};

namespace std {
  template <> struct hash<MinionAttraction> {
    size_t operator()(const MinionAttraction& t) const {
      return (size_t)t.getId();
    }
  };
}

struct AttractionInfo {
  AttractionInfo(MinionAttraction, double amountClaimed, double minAmount, bool mandatory = false);

  SERIALIZATION_DECL(AttractionInfo);

  MinionAttraction SERIAL(attraction);
  double SERIAL(amountClaimed);
  double SERIAL(minAmount);
  bool SERIAL(mandatory);
};

struct ImmigrantInfo {
  CreatureId SERIAL(id);
  double SERIAL(frequency);
  vector<AttractionInfo> SERIAL(attractions);
  EnumSet<MinionTrait> SERIAL(traits);
  bool SERIAL(spawnAtDorm);
  int SERIAL(salary);
  optional<TechId> SERIAL(techId);
  optional<SunlightState> SERIAL(limit);
  optional<Range> SERIAL(groupSize);
  bool SERIAL(autoTeam);
  bool SERIAL(ignoreSpawnType);

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);
};

struct BirthSpawn {
  CreatureId id;
  double frequency;
  optional<TechId> tech;
};

struct PopulationIncrease {
  FurnitureType SERIAL(type);
  double SERIAL(increasePerSquare);
  int SERIAL(maxIncrease);

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);
};

struct GuardianInfo {
  CreatureId SERIAL(creature);
  double SERIAL(probability);
  int SERIAL(minEnemies);
  int SERIAL(minVictims);

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);
};

struct DormInfo {
  FurnitureType bedType;
  optional<CollectiveWarning> warning;
};

struct ResourceInfo {
  optional<FurnitureType> storageType;
  optional<ItemIndex> itemIndex;
  ItemId itemId;
  string name;
  ViewId viewId;
  bool dontDisplay;
};

struct MinionTaskInfo {
  enum Type { FURNITURE, EXPLORE, COPULATE, CONSUME, EAT, SPIDER } type;
  MinionTaskInfo();
  MinionTaskInfo(FurnitureType, const string& description);
  typedef function<bool(const Creature*, FurnitureType)> UsagePredicate;
  MinionTaskInfo(UsagePredicate, const string& description);
  MinionTaskInfo(Type, const string& description, optional<CollectiveWarning> = none);
  UsagePredicate furniturePredicate;
  string description;
  optional<CollectiveWarning> warning;
};

struct WorkshopInfo {
  FurnitureType furniture;
  MinionTask minionTask;
  string taskName;
};

struct FloorInfo {
  SquareType type;
  CostInfo cost;
  string name;
  double efficiencyBonus;
};

class CollectiveConfig {
  public:
  static CollectiveConfig keeper(double immigrantFrequency, int payoutTime, double payoutMultiplier,
      int maxPopulation, vector<PopulationIncrease>, vector<ImmigrantInfo>);
  static CollectiveConfig withImmigrants(double immigrantFrequency, int maxPopulation, vector<ImmigrantInfo>);
  static CollectiveConfig noImmigrants();

  CollectiveConfig& allowRecruiting(int minPopulation);
  CollectiveConfig& setLeaderAsFighter();
  CollectiveConfig& setGhostSpawns(double prob, int number);
  CollectiveConfig& setGuardian(GuardianInfo);

  bool isLeaderFighter() const;
  bool getManageEquipment() const;
  bool getWorkerFollowLeader() const;
  double getImmigrantFrequency() const;
  int getPayoutTime() const;
  double getPayoutMultiplier() const;
  bool getStripSpawns() const;
  bool getFetchItems() const;
  bool getEnemyPositions() const;
  bool getWarnings() const;
  bool getConstructions() const;
  int getMaxPopulation() const;
  int getNumGhostSpawns() const;
  double getGhostProb() const;
  optional<int> getRecruitingMinPopulation() const;
  bool sleepOnlyAtNight() const;
  const vector<ImmigrantInfo>& getImmigrantInfo() const;
  const vector<PopulationIncrease>& getPopulationIncreases() const;
  const optional<GuardianInfo>& getGuardianInfo() const;
  vector<BirthSpawn> getBirthSpawns() const;
  unique_ptr<Workshops> getWorkshops() const;
  static const WorkshopInfo& getWorkshopInfo(WorkshopType);
  static optional<WorkshopType> getWorkshopType(MinionTask);
  static optional<WorkshopType> getWorkshopType(FurnitureType);

  bool activeImmigrantion(const Game*) const;
  const EnumMap<SpawnType, DormInfo>& getDormInfo() const;
  const vector<FurnitureType>& getRoomsNeedingLight() const;
  static const ResourceInfo& getResourceInfo(CollectiveResourceId);
  static optional<int> getTrainingMaxLevelIncrease(FurnitureType);
  static const MinionTaskInfo& getTaskInfo(MinionTask);
  static const FurnitureType& getEquipmentStorage();
  static const FurnitureType& getResourceStorage();
  static const vector<FloorInfo>& getFloors();
  static double getEfficiencyBonus(SquareType);
  static bool canBuildOutsideTerritory(FurnitureType);

  SERIALIZATION_DECL(CollectiveConfig);

  private:
  enum CollectiveType { KEEPER, VILLAGE };
  CollectiveConfig(double immigrantFrequency, int payoutTime, double payoutMultiplier,
      vector<ImmigrantInfo>, CollectiveType, int maxPopulation, vector<PopulationIncrease>);

  double SERIAL(immigrantFrequency);
  int SERIAL(payoutTime);
  double SERIAL(payoutMultiplier);
  int SERIAL(maxPopulation);
  vector<PopulationIncrease> SERIAL(populationIncreases);
  vector<ImmigrantInfo> SERIAL(immigrantInfo);
  CollectiveType SERIAL(type);
  optional<int> SERIAL(recruitingMinPopulation);
  bool SERIAL(leaderAsFighter) = false;
  int SERIAL(spawnGhosts) = 0;
  double SERIAL(ghostProb) = 0;
  optional<GuardianInfo> SERIAL(guardianInfo);
};
