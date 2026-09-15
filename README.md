# Bargain Bin Robotics

**Substitute being clever for being rich.**

Bargain Bin Robotics is the canonical home for low-cost educational robotics that uses calibration, repeated observation, statistics, and software to extract more value from inexpensive hardware.

## Core ideas

### Lazy LiDAR

Use low-cost ranging, motion, repeated observations, and mapping/statistical methods to obtain useful spatial information without requiring expensive scanning hardware.

### Platform independence

Arduino-class robots, micro:bit vehicles, RP2040 platforms, XRP, Maker Pi, CuteBot, Alvik-related work, and low-cost STC robots are targets or exemplars beneath the larger robotics idea. A board port does not automatically deserve its own project identity.

### Missions before gadgets

Useful educational missions include:

- navigation and obstacle avoidance;
- mapping from imperfect sensors;
- prospecting/claims activities;
- energy-budget and runtime missions;
- repeated-trial experiments where statistics improve inexpensive sensing.

The NOTHING IS FREE energy mission belongs educationally to CORE 10/NIF while its robot implementation can live here.

## Design rules

- Prefer cheap sensors used intelligently over expensive sensors used casually.
- Make uncertainty visible.
- Calibrate before blaming the hardware.
- Use repeated observations when one observation is unreliable.
- Keep the educational question more important than the robot platform.
- Archive platform ports underneath durable project ideas.

## Planned repository structure

- `lazy-lidar/`
- `missions/`
- `platforms/`
- `mapping/`
- `energy/`
- `docs/`
- `validation/`

## First exemplar target

The first packaged release should be one canonical robot mission with a clearly documented low-cost platform, runnable code, expected behavior, test procedure, and either a Lazy LiDAR demonstration or NIF-style energy mission.

## Current state

**Lifecycle:** `active`

**Priority:** P2 packaging after the P1 SDL/CORE/FFT work.

Historical platform code and experiments still need to be consolidated beneath this durable parent rather than treated as many unrelated projects.
