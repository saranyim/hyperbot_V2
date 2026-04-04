#pragma once
#include "vex.h"

using namespace vex;
int TaskAutonomous();

// Ramped drive – speeds up at the start and slows down at the end.
// maxSpeed : peak cruise speed  (default 80 %)
// minSpeed : start / stop speed (default 20 %)
// rampFrac : fraction of distance used for each ramp (default 0.25 = 25 %)
void driveRamped(directionType dir, double distance_mm,
                 uint16_t maxSpeed = 80, uint16_t minSpeed = 20,
                 double rampFrac = 0.25);

// Convenience wrappers – just pass the distance (and optionally max speed).
// If maxSpeed is omitted (or 0), uses the global driveSpeed variable.
void driveForward(double distance_mm, uint16_t maxSpeed = 0);
void driveReverse(double distance_mm, uint16_t maxSpeed = 0);

// Small nudge drive to confirm a pin/game-piece is seated in the claw.
// Returns the average actual wheel velocity (%) — use it to verify movement.
void driveNudge(directionType dir = forward, double distance_mm = 80, uint16_t speed = 20);

// Adjust position using the rear distance sensor until it reads targetDist_mm.
// Sensor faces the wall behind the bot.
//   reading < target  →  drive forward  (moves away from wall, distance increases)
//   reading > target  →  drive reverse  (moves toward  wall, distance decreases)
// Stops when within tolerance_mm of target.
//   targetDist_mm – desired rear distance to wall in mm  (e.g. 1500)
//   speed         – drive speed in percent               (default 50)
//   tolerance_mm  – dead-band around the target          (default 20)
void driveToWallDistance(double targetDist_mm,
                         uint16_t maxSpeed = 80,
                         uint16_t minSpeed = 20,
                         double   rampFrac = 0.25);
