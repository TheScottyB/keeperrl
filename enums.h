#ifndef _ENUMS_H
#define _ENUMS_H

#include "serialization.h"

#define ENUM_HASH(name) \
namespace std { \
  template <> struct hash<name> { \
    size_t operator()(const name& l) const { \
      return (size_t)l; \
    } \
  }; \
}

enum class MsgType {
    FEEL, // better
    BLEEDING_STOPS,
    COLLAPSE,
    FALL,
    PANIC,
    RAGE,
    DIE_OF,
    ARE, // bleeding
    YOUR, // your head is cut off
    FALL_ASLEEP, //
    WAKE_UP,
    DIE, //
    FALL_APART,
    TELE_APPEAR,
    TELE_DISAPPEAR,
    ATTACK_SURPRISE,
    CAN_SEE_HIDING,
    SWING_WEAPON,
    THRUST_WEAPON,
    KICK,
    PUNCH,
    BITE,
    HIT,
    CRAWL,
    TRIGGER_TRAP,
    DROP_WEAPON,
    GET_HIT_NODAMAGE, // body part
    HIT_THROWN_ITEM,
    HIT_THROWN_ITEM_PLURAL,
    MISS_THROWN_ITEM,
    MISS_THROWN_ITEM_PLURAL,
    ITEM_CRASHES,
    ITEM_CRASHES_PLURAL,
    STAND_UP,
    TURN_INVISIBLE,
    TURN_VISIBLE,
    ENTER_PORTAL,
    HAPPENS_TO,
    BURN,
    DROWN,
    SET_UP_TRAP,
    DECAPITATE,
    TURN,
    KILLED_BY,
    MISS_ATTACK}; //
enum class BodyPart { HEAD, TORSO, ARM, WING, LEG, BACK};
enum class AttackType { CUT, STAB, CRUSH, PUNCH, BITE, HIT, SHOOT, SPELL};
enum class AttackLevel { LOW, MIDDLE, HIGH };
enum class AttrType { STRENGTH, DAMAGE, TO_HIT, THROWN_DAMAGE, THROWN_TO_HIT, DEXTERITY, DEFENSE, SPEED, INV_LIMIT};
enum class ItemType { WEAPON, RANGED_WEAPON, AMMO, ARMOR, SCROLL, POTION, BOOK, AMULET, TOOL, OTHER, GOLD, FOOD,
    CORPSE };
enum class EquipmentSlot { WEAPON, RANGED_WEAPON, BODY_ARMOR, HELMET, BOOTS, AMULET };
enum class ArmorType { BODY_ARMOR, HELMET, BOOTS };

enum class SquareApplyType { DRINK, USE_CHEST, ASCEND, DESCEND, PRAY, SLEEP, TRAIN, WORKSHOP };

enum class MinionTask { SLEEP, GRAVE, TRAIN, WORKSHOP, STUDY, LABORATORY};
enum class TrapType { BOULDER, POISON_GAS, ALARM };

enum class SquareAttrib {
  NO_DIG,
  MOUNTAIN,
  HILL,
  LOWLAND,
  CONNECT, 
  LAKE,
  RIVER,
  ROAD_CUT_THRU,
  NO_ROAD,
  ROOM,
  COLLECTIVE_START,
  COLLECTIVE_STAIRS,
  EMPTY_ROOM,
  FOG,
};

ENUM_HASH(SquareAttrib);

enum class Dir { N, S, E, W, NE, NW, SE, SW };
ENUM_HASH(Dir);

enum class StairKey { DWARF, CRYPT, GOBLIN, PLAYER_SPAWN, HERO_SPAWN, PYRAMID, TOWER, CASTLE_CELLAR, DRAGON };
enum class StairDirection { UP, DOWN };

enum class CreatureId {
    KEEPER,

    GOBLIN,
    GREAT_GOBLIN,
    BANDIT,

    SPECIAL_MONSTER,
    SPECIAL_MONSTER_KEEPER,
    SPECIAL_HUMANOID,

    GHOST,
    DEVIL,
    DARK_KNIGHT,
    DRAGON,
    CYCLOPS,
    WITCH,

    CLAY_GOLEM,
    STONE_GOLEM,
    IRON_GOLEM,
    LAVA_GOLEM,

    ZOMBIE,
    VAMPIRE,
    VAMPIRE_LORD,
    VAMPIRE_BAT,
    MUMMY,
    MUMMY_LORD,
    SKELETON,
    
    DWARF,
    DWARF_BARON,

    IMP,
    BILE_DEMON,
    CHICKEN,
    DUNGEON_HEART,

    KNIGHT,
    AVATAR,
    CASTLE_GUARD,
    ARCHER,
    PESEANT,
    CHILD,

    LIZARDMAN,
    LIZARDLORD,
    
    ELF,
    ELF_ARCHER,
    ELF_CHILD,
    ELF_LORD,
    HORSE,
    COW,
    SHEEP,
    PIG,
    
    GNOME, 
    LEPRECHAUN,
    
    JACKAL,
    DEER,
    BOAR,
    FOX,
    RAVEN,
    VULTURE,
    WOLF,

    DEATH,
    NIGHTMARE,
    FIRE_SPHERE,
    KRAKEN,
    BAT,
    SNAKE,
    CAVE_BEAR,
    ACID_MOUND,
    SPIDER,
    SCORPION,
    FLY,
    RAT};


enum class ItemId { KNIFE,
  SWORD,
  SPECIAL_SWORD,
  ELVEN_SWORD,
  SPECIAL_ELVEN_SWORD,
  BATTLE_AXE,
  SPECIAL_BATTLE_AXE,
  WAR_HAMMER,
  SPECIAL_WAR_HAMMER,
  SCYTHE,
  BOW,
  ARROW,

  LEATHER_ARMOR,
  LEATHER_HELM,
  TELEPATHY_HELM,
  CHAIN_ARMOR,
  IRON_HELM,
  LEATHER_BOOTS,
  IRON_BOOTS,
  SPEED_BOOTS,

  TELE_SCROLL,
  PORTAL_SCROLL,
  IDENTIFY_SCROLL,
  BOULDER_SCROLL,
  FIRE_SCROLL,
  POISON_GAS_SCROLL,
  DESTROY_EQ_SCROLL,
  ENHANCE_A_SCROLL,
  ENHANCE_W_SCROLL,
  FIRE_SPHERE_SCROLL,
  WORD_OF_POWER_SCROLL,
  DECEPTION_SCROLL,
  SUMMON_INSECTS_SCROLL,

  HEALING_POTION,
  SLEEP_POTION,
  BLINDNESS_POTION,
  INVISIBLE_POTION,
  POISON_POTION,
  SLOW_POTION,
  SPEED_POTION,

  PANIC_MUSHROOM,
  RAGE_MUSHROOM,
  STRENGTH_MUSHROOM,
  DEXTERITY_MUSHROOM,
  HALLU_MUSHROOM,

  WARNING_AMULET,
  HEALING_AMULET,
  DEFENSE_AMULET,
  FRIENDLY_ANIMALS_AMULET,

  FIRST_AID_KIT,
  ROCK,
  IRON_ORE,
  GOLD_PIECE,
  WOOD_PLANK,

  MUSHROOM_BOOK,
  POTION_BOOK,
  AMULET_BOOK,

  BOULDER_TRAP_ITEM,
  GAS_TRAP_ITEM,
  ALARM_TRAP_ITEM,
};

enum class ViewLayer {
  CREATURE,
  LARGE_ITEM,
  ITEM,
  FLOOR,
  FLOOR_BACKGROUND,
};

enum class HighlightType {
  BUILD,
  RECT_SELECTION,
  POISON_GAS,
  FOG,
  MEMORY,
};

enum class StairLook {
  NORMAL,
  HELL,
  CELLAR,
  PYRAMID,
  DUNGEON_ENTRANCE,
  DUNGEON_ENTRANCE_MUD,
};

enum class SettlementType {
  VILLAGE,
  CASTLE,
  CASTLE2,
  COTTAGE,
};

const static vector<ViewLayer> allLayers =
    {ViewLayer::FLOOR_BACKGROUND, ViewLayer::FLOOR, ViewLayer::ITEM, ViewLayer::LARGE_ITEM, ViewLayer::CREATURE};

ENUM_HASH(ViewLayer);

enum class ViewId { 
  PLAYER,
  KEEPER,
  ELF,
  ELF_ARCHER,
  ELF_CHILD,
  ELF_LORD,
  ELVEN_SHOPKEEPER,
  LIZARDMAN,
  LIZARDLORD,
  DWARF,
  DWARF_BARON,
  DWARVEN_SHOPKEEPER,
  IMP,
  BILE_DEMON,
  CHICKEN,
  DARK_KNIGHT,
  DRAGON,
  CYCLOPS,
  WITCH,
  GHOST,
  DEVIL,
  KNIGHT,
  CASTLE_GUARD,
  AVATAR,
  ARCHER,
  PESEANT,
  CHILD,
  GREAT_GOBLIN,
  GOBLIN,
  BANDIT,
  CLAY_GOLEM,
  STONE_GOLEM,
  IRON_GOLEM,
  LAVA_GOLEM,
  ZOMBIE,
  SKELETON,
  VAMPIRE,
  VAMPIRE_LORD,
  MUMMY,
  MUMMY_LORD,
  HORSE,
  COW,
  PIG,
  SHEEP,
  JACKAL,
  DEER,
  BOAR,
  FOX,
  BEAR,
  WOLF,
  BAT,
  RAT,
  SPIDER,
  FLY,
  ACID_MOUND,
  SCORPION,
  SNAKE,
  VULTURE,
  RAVEN,
  GNOME,
  VODNIK,
  LEPRECHAUN,
  KRAKEN,
  KRAKEN2,
  FIRE_SPHERE,
  NIGHTMARE,
  DEATH,
  SPECIAL_BEAST,
  SPECIAL_HUMANOID,
  UNKNOWN_MONSTER,

  FLOOR,
  SAND,
  BRIDGE,
  PATH,
  ROAD,
  GRASS,
  CROPS,
  MUD,
  WALL,
  HILL,
  MOUNTAIN,
  MOUNTAIN2,
  SNOW,
  GOLD_ORE,
  IRON_ORE,
  STONE,
  WOOD_WALL,
  BLACK_WALL,
  YELLOW_WALL,
  HELL_WALL,
  LOW_ROCK_WALL,
  CASTLE_WALL,
  MUD_WALL,
  SECRETPASS,
  DUNGEON_ENTRANCE,
  DUNGEON_ENTRANCE_MUD,
  DOWN_STAIRCASE,
  UP_STAIRCASE,
  DOWN_STAIRCASE_HELL,
  UP_STAIRCASE_HELL,
  DOWN_STAIRCASE_CELLAR,
  UP_STAIRCASE_CELLAR,
  DOWN_STAIRCASE_PYR,
  UP_STAIRCASE_PYR,
  CANIF_TREE,
  DECID_TREE,
  TREE_TRUNK,
  BUSH,
  WATER,
  MAGMA,
  ABYSS,
  DOOR,
  PLANNED_DOOR,
  DIG_ICON,
  FOUNTAIN,
  CHEST,
  OPENED_CHEST,
  COFFIN,
  OPENED_COFFIN,
  THRONE,
  BED,
  STOCKPILE,
  TORTURE_TABLE,
  TRAINING_DUMMY,
  LIBRARY,
  LABORATORY,
  ANIMAL_TRAP,
  WORKSHOP,
  DUNGEON_HEART,
  ALTAR,
  GRAVE,
  BARS,
  BORDER_GUARD,
  DESTROYED_FURNITURE,
  BURNT_FURNITURE,
  FALLEN_TREE,
  BURNT_TREE,

  BODY_PART,
  BONE,
  SWORD,
  SPECIAL_SWORD,
  ELVEN_SWORD,
  KNIFE,
  WAR_HAMMER,
  SPECIAL_WAR_HAMMER,
  BATTLE_AXE,
  SPECIAL_BATTLE_AXE,
  BOW,
  ARROW,
  SCROLL,
  STEEL_AMULET,
  COPPER_AMULET,
  CRYSTAL_AMULET,
  WOODEN_AMULET,
  AMBER_AMULET,
  BOOK,
  FIRST_AID,
  EFFERVESCENT_POTION,
  MURKY_POTION,
  SWIRLY_POTION,
  VIOLET_POTION,
  PUCE_POTION,
  SMOKY_POTION,
  FIZZY_POTION,
  MILKY_POTION,
  GOLD,
  LEATHER_ARMOR,
  LEATHER_HELM,
  TELEPATHY_HELM,
  CHAIN_ARMOR,
  IRON_HELM,
  LEATHER_BOOTS,
  IRON_BOOTS,
  SPEED_BOOTS,
  BOULDER,
  UNARMED_BOULDER_TRAP,
  PORTAL,
  TRAP,
  GAS_TRAP,
  ALARM_TRAP,
  UNARMED_GAS_TRAP,
  UNARMED_ALARM_TRAP,
  ROCK,
  IRON_ROCK,
  WOOD_PLANK,
  SLIMY_MUSHROOM, 
  PINK_MUSHROOM, 
  DOTTED_MUSHROOM, 
  GLOWING_MUSHROOM, 
  GREEN_MUSHROOM, 
  BLACK_MUSHROOM,

  TRAP_ITEM,
  GUARD_POST,
  DESTROY_BUTTON,
  MANA,
  DANGER,
};

enum class SquareType {
  FLOOR,
  BRIDGE,
  PATH,
  ROAD,
  GRASS,
  CROPS,
  MUD,
  HELL_WALL,
  ROCK_WALL,
  LOW_ROCK_WALL,
  WOOD_WALL,
  BLACK_WALL,
  YELLOW_WALL,
  CASTLE_WALL,
  MUD_WALL,
  MOUNTAIN,
  MOUNTAIN2,
  GLACIER,
  HILL,
  SECRET_PASS,
  WATER,
  MAGMA,
  GOLD_ORE,
  IRON_ORE,
  STONE,
  ABYSS,
  SAND,
  DECID_TREE,
  CANIF_TREE,
  TREE_TRUNK,
  BUSH,
  BED,
  STOCKPILE,
  TORTURE_TABLE,
  TRAINING_DUMMY,
  LIBRARY,
  LABORATORY,
  ANIMAL_TRAP,
  WORKSHOP,
  HATCHERY,
  KEEPER_THRONE,
  GRAVE,
  ROLLING_BOULDER,
  POISON_GAS,
  FOUNTAIN,
  CHEST,
  TREASURE_CHEST,
  COFFIN,
  IRON_BARS,
  DOOR,
  TRIBE_DOOR,
  DOWN_STAIRS,
  UP_STAIRS,
  BORDER_GUARD,
  ALTAR,
};

inline bool isWall(SquareType type) {
  switch (type) {
    case SquareType::HELL_WALL:
    case SquareType::LOW_ROCK_WALL:
    case SquareType::ROCK_WALL:
    case SquareType::BLACK_WALL:
    case SquareType::YELLOW_WALL:
    case SquareType::WOOD_WALL:
    case SquareType::MUD_WALL:
    case SquareType::CASTLE_WALL: return true;
    default: return false;
  }
}

enum class EffectStrength { WEAK, NORMAL, STRONG };
enum class EffectType { 
    TELEPORT,
    HEAL,
    SLEEP,
    IDENTIFY,
    PANIC,
    RAGE,
    ROLLING_BOULDER,
    FIRE,
    SLOW,
    SPEED,
    HALLU,
    STR_BONUS,
    DEX_BONUS,
    BLINDNESS,
    INVISIBLE,
    PORTAL,
    DESTROY_EQUIPMENT,
    ENHANCE_ARMOR,
    ENHANCE_WEAPON,
    FIRE_SPHERE_PET,
    GUARDING_BOULDER,
    EMIT_POISON_GAS,
    POISON,
    WORD_OF_POWER,
    DECEPTION,
    SUMMON_INSECTS,
    ACID,
    ALARM,
};

enum class AnimationId {
  EXPLOSION,
};

enum class SpellId {
  HEALING,
  SUMMON_INSECTS,
  DECEPTION,
  SPEED_SELF,
  STR_BONUS,
  DEX_BONUS,
  FIRE_SPHERE_PET,
  TELEPORT,
  INVISIBILITY,
  WORD_OF_POWER,
};

enum class TechId {
  NECROMANCY,
  BEAST_TAMING,
  MATTER_ANIMATION,
  HUMANOID_BREEDING,
  SPELLCASTING,
};

ENUM_HASH(TechId);

enum class QuestId { DRAGON, CASTLE_CELLAR, BANDITS, GOBLINS, DWARVES };
ENUM_HASH(QuestId);

enum class TribeId {
  MONSTER,
  PEST,
  WILDLIFE,
  HUMAN,
  ELVEN,
  DWARVEN,
  GOBLIN,
  PLAYER,
  DRAGON,
  CASTLE_CELLAR,
  BANDIT,
  KILL_EVERYONE,
  PEACEFUL,
  KEEPER,
  LIZARD,
};
ENUM_HASH(TribeId);

#endif
