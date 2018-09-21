#pragma once

#include "util.h"

RICH_ENUM(FXName,
  DUMMY,
  TEST_SIMPLE,
  TEST_MULTI,
  WOOD_SPLINTERS,
  ROCK_SPLINTERS,
  ROCK_CLOUD,
  EXPLOSION,
  RIPPLE,
  SAND_DUST,
  WATER_SPLASH,
  FIRE,
  FIRE_SPHERE,
  CIRCULAR_SPELL,
  CIRCULAR_BLAST,
  AIR_BLAST,
  AIR_BLAST2,
  MAGIC_MISSILE,
  FIREBALL,
  SLEEP,
  INSANITY,
  BLIND,
  GLITTERING,
  PEACEFULNESS,
  SPIRAL,
  SPIRAL2,
  SPEED,
  FLYING,
  DEBUFF
);

// TODO: better name?
RICH_ENUM(FXVariantName,
  PEACEFULNESS,
  BLIND,
  INSANITY,
  SPEED,
  SLEEP,
  FLYING,
  FIRE_SPHERE,

  DEBUFF_RED,
  DEBUFF_GREEN
);

bool fxesAvailable();
