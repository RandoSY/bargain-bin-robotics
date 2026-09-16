# Bargain Bin Robotics — Idea Record

## Problem

Robotics education often buys capability rather than teaching where capability comes from. Expensive sensors and integrated platforms can make systems work while hiding calibration, uncertainty, geometry, inference, and control.

## Central idea

**Substitute being clever for being rich.** Use inexpensive hardware plus calibration, repeated observation, statistics, controlled movement, known geometry, and explicit software inference to recover useful robotic capability.

## Lazy / Patient LiDAR

A cheap range sensor can produce useful map-like information when the robot deliberately changes viewpoint and accumulates repeated measurements. The missing premium hardware is partly replaced by time, motion, geometry, filtering, and confidence estimates.

The educational advantage is that the mapping process remains visible: learners can see why a point is believed rather than accepting a finished scan from a black box.

## Robot as measurement instrument

A low-cost mobile robot is a movable laboratory for:

- distance and heading;
- coordinate geometry;
- odometry and error accumulation;
- mapping and localization;
- search and route choice;
- energy/runtime measurement;
- sensor fusion and confidence;
- control and failure behavior.

## MURL / Robot Passport

Robot programs should target a stable conceptual mission API while a profile/adapter records what a specific platform can actually do. Hardware identity, calibration, limits, safety state, and evidence form a **Robot Passport** rather than being implicit assumptions in code.

This allows portability without pretending all robots are identical.

## Safe Approach / confidence-aware action

Cheap sensing should not be made trustworthy by pretending it is precise. The robot should expose confidence and use conservative behavior when evidence is weak.

## Standalone bridge boards

Modern wireless/control boards attached to inexpensive legacy robots should remain functional standalone when disconnected from the original robot. The bridge is therefore a reusable computing node, not disposable glue.

## Why it matters

The architecture makes robotics economically accessible while increasing—rather than reducing—the amount of engineering that learners can see.

## Distinctive contribution

Low cost is treated as an intellectual constraint that encourages explicit models and inference. The project does not seek a cheap imitation of premium robotics; it asks what capability can be reconstructed when money is replaced by reasoning.

## Representative evidence

- Bargain Bin Robotics v0.2 user guide and roadmap
- Balance-Bot source and historical snapshot
- MURL Prospector manual and recovered source modules
- Reka:Bit lineage
- Robot Passport / Evidence Record concepts
- Lazy LiDAR, Safe Approach, compass/ToF mapping, energy-budget and prospecting lineages

## Reconstruction path

Begin with a cheap 2WD robot, known wheel geometry, one simple range sensor, and a heading/odometry method. Calibrate each observable quantity. Record repeated measurements with pose. Build confidence-aware mapping from those records. Add richer sensors only after the learner can explain what each new sensor contributes.
