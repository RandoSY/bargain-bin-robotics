# MURL v0.1 + Prospector — Reference Architecture

**MURL:** MicroBlocks Universal Robotics Layer  
**Version:** 0.1.0 design draft / reference programming edition, 14 August 2026  
**Canonical role:** historical architecture and Prospector reference design within Bargain Bin Robotics / Robot EDU.  
**Maturity:** architecture ready to guide prototype implementation; **not an implementation standard and not a finished robot package**.

> **Standardize meaning, not machinery.**

## Core proposition

MicroBlocks already supplies a portable bytecode VM, live incremental programming, autonomous execution, concurrency, communication and inspectable libraries. MURL's job is narrower:

> **MicroBlocks is the portable machine. MURL is the portable definition of a robot.**

One behavior project should be able to operate honestly across different boards, motor controllers and sensor arrangements because applications speak in stable physical meanings while a profile and adapter disclose the real hardware.

“Universal” therefore does **not** mean that arbitrary hardware needs no adapter, nor that all robots have the same accuracy. It means a command such as “move 1000 mm at 150 mm/s” keeps the same physical intention and evidence vocabulary when that capability is genuinely available.

## Scope of v0.1

Initial scope is deliberately bounded to differential-drive educational/experimental robots at supervised classroom speeds, including MicroBlocks targets in the nRF52833, RP2040/RP2040W and ESP32 families.

In scope:

- open-loop motor effort;
- optional encoder-based velocity/distance/turn;
- range, line, IMU, bumper and power evidence;
- autonomous execution after live development;
- library-first implementation with narrowly justified native primitives;
- standardized trial evidence.

Explicitly outside the v0.1 claim:

- certified safety control;
- SLAM / heavy vision / fleet middleware;
- general manipulators, drones or balancing robots;
- automatic motion based only on hardware probing;
- fabrication of measurements a robot cannot actually make.

## Design principles

### 1. Standardize meaning, not machinery

Student/application blocks describe physical intent. The adapter translates that intent into pins, bus transactions, motor polarity, encoder logic and device-specific services.

### 2. Truth before convenience

Missing or degraded capabilities must be reported honestly. A timed motor guess must not be mislabeled as measured distance or velocity.

### 3. Physical units at the public boundary

The stable API uses units such as:

- mm;
- ms;
- mm/s;
- mg;
- centidegrees;
- centidegrees/s;
- millivolts;
- normalized permille.

Raw ticks, duty cycles and registers remain diagnostic details.

### 4. Library first; native only when measured

Calibration, filtering, profiles and behaviors belong in inspectable MicroBlocks libraries. Native VM primitives are justified only by traceable timing/latency evidence that a portable implementation cannot meet an advertised requirement.

### 5. Safety beneath behavior

Student code may request movement, but a distinct safety service can revoke motor ownership and clear outputs.

### 6. Evidence is a first-class output

A robot run should return requested, measured and error values, state transitions and faults. The robot is therefore a mobile experimental platform, not just a remotely controlled toy.

### 7. Progressive capability

A simple robot can honestly implement the Core level; a better instrumented robot can add measured motion and richer sensors without inventing a second application language.

## Layered architecture

| Layer | Responsibility | Portability |
|---|---|---|
| Behavior | learner projects, investigations, reusable activities | portable |
| Service | motion, sensing, odometry, safety, calibration, telemetry | portable where capabilities permit |
| Universal API | public vocabulary, units, states, errors, events | stable contract |
| Profile | dimensions, limits, calibration, advertised capabilities | robot-specific data |
| Adapter | motor/encoder/sensor/bus translation | robot-specific code |
| VM / primitive | bytecode and timing-critical hardware access | board-family implementation |

Application code depends on hardware only through the Universal API during a portability test.

## Three kinds of portability

MURL distinguishes:

1. **Source portability** — the same MicroBlocks project loads without editing.
2. **Semantic portability** — the same public command denotes the same physical intention.
3. **Evidential portability** — trial records use the same field names, units and error meanings.

The third category is especially important educationally: two robots can be compared because they leave compatible evidence even when their internals differ.

## Capability model

Every active profile gives a definitive answer for each named capability.

Required Core capabilities include:

- `profile.valid`
- `drive.effort`
- `safety.stop`
- `telemetry.basic`

Optional examples include:

- `sensor.encoder`
- `drive.velocity`
- `motion.distance`
- `motion.turn`
- `sensor.range.front`
- `sensor.line`
- `sensor.imu`
- `sensor.force`
- `power.voltage`
- BLE / Wi-Fi transport capability.

**No false precision:** if `drive.velocity` is absent, timed effort may still be useful, but it remains explicitly an approximation.

## Coordinate and unit conventions

The v0.1 public convention is:

- +X forward from drive axle center;
- +Y to robot left;
- positive rotation counterclockwise from above;
- left/right from the robot's own forward viewpoint;
- positive corrected encoder motion means forward.

Scaled integers are used deliberately so numerical behavior remains predictable across MicroBlocks targets.

## Robot profile as machine-readable passport

A profile connects the universal meaning to one build. Important fields include:

- stable profile ID and semantic version;
- MURL API major version;
- robot name;
- board family;
- drive geometry;
- wheel diameter and track width when measured motion is advertised;
- encoder counts/revolution;
- motor polarity;
- conservative speed/acceleration/watchdog limits;
- verified capabilities;
- sensor records;
- dated calibration values.

Profile selection must be explicit **before powered motion**. Hardware discovery may suggest a profile, but discovery alone must never authorize motion.

## Small public API

The conceptual API includes identity/init, motion, sensing, safety and evidence operations such as:

- select profile;
- initialize robot;
- query readiness/identity/capability/status;
- set wheel effort;
- command speed when genuinely measured;
- finite `move [mm] at [mm/s]`;
- finite `turn [cdeg] at [cdeg/s]`;
- normal stop / safe stop / emergency stop;
- motion active/progress/cancel;
- wheel encoder and measured wheel speed;
- range, line, heading, acceleration and bumper queries;
- battery voltage;
- limits/watchdog;
- begin/end a robot trial;
- export a standardized trial report.

The public vocabulary is intentionally smaller than the underlying hardware surface.

## Safety state machine

Operating states are:

- `IDLE`
- `MOVING`
- `FAULT`
- `E_STOP`

Before initialization the runtime is effectively `UNINITIALIZED` with outputs off.

The safety contract requires, among other things:

- zero motor output at boot, profile selection and failed initialization;
- independent output-clear path;
- profile speed/acceleration/watchdog limits;
- limits enforced beneath application behavior;
- measured stall detection when encoder motion is advertised;
- latched E_STOP;
- no automatic resumption after fault recovery;
- explicit diagnostic motor ownership when bypassing normal services.

MURL is **not** represented as a certified safety system. The purpose is to make the safest available educational behavior automatic, visible and testable.

## Timing and concurrency

MicroBlocks concurrency is treated as a strength: motion, sensing, display and telemetry can remain separate services. Blocking finite-motion operations must yield so safety and evidence collection continue.

Prototype timing numbers in v0.1 are targets to be measured, not claims about untested hardware. Native optimization must be evidence-driven.

## Package architecture

The proposed implementation remains modular:

- MURL Core
- Motion Service
- Safety Service
- Evidence Service
- robot-specific adapter
- robot-specific profile
- Robot Passport conformance project

The objective is to add a new adapter without forking the universal services or VM.

## Robot Passport

Robot Passport is both demonstration and enforcement mechanism: **the identical conformance project runs against every profile.**

The v0.1 test sequence includes:

- P0 identity;
- P1 safe boot;
- P2 polarity;
- P3 open-loop motion + safe stop;
- P4 encoders;
- P5 distance;
- P6 turning;
- P7 advertised sensors;
- P8 watchdog;
- P9 persistence;
- P10 evidence export.

Unavailable capabilities are recorded as unavailable rather than failures. An adapter advertises only the capability/accuracy tier it actually passes.

A standardized trial record carries identity/version, capabilities, calibration geometry, requested vs measured motion, error, duration, battery evidence where available, watchdog/stop latency, state transitions, faults and test-surface/payload notes.

This makes **reproducibility part of the robot interface**.

## Reference targets and roadmap

Initial architecture targets include:

- Cutebot Pro / micro:bit v2 — adapts an opaque I²C-oriented robot controller;
- XRP / RP2040 — reuses visible encoded-motor/PID composition;
- generic Pico/Pico W differential robot — transparent pins, encoders and sensors;
- generic ESP32/M5Stack motor base — ESP32 family + wireless evidence.

The recommended first build is a transparent two-motor RP2040 robot before wrapping many commercial platforms. That keeps mechanics visible first; commercial adapters then test whether the abstraction survives different hardware paths.

Roadmap:

- 0.1 — freeze specification;
- 0.2 — Core + Safety + effort-only adapter;
- 0.3 — measured encoded motion on transparent RP2040 target;
- 0.4 — additional robot adapters without changing Robot Passport;
- 0.5 — standardized evidence/dashboard/transports;
- 1.0 — portable reference across at least three robots and MCU families.

## Prospector reference mission

The companion Prospector manual asks a learner to build an evidence-producing mission rather than simply drive a robot.

A representative mission:

1. explore in finite measurable movements;
2. stop/avoid when front range crosses a documented threshold;
3. sample a “resource” signal;
4. record each finite movement/turn in a path log;
5. detect the resource against a threshold established in a separate baseline trial;
6. reverse/invert the recorded actions to return;
7. safe-stop and generate a trial report.

The reference manual deliberately uses a **path log before a full map** because finite actions are inspectable. A learner can see exactly what was commanded and what reverse action should correspond to it. Later work can compare this dead-reckoning return with IMU or landmark assistance.

The Prospector model includes operational modes such as teaching/rehearsal, sample mission, traverse, scan, dig, user-script operation and safe-stop, but the manual is explicit that it is a **reference design**. A complete implementation package would still need installable modules, examples, robot profiles, a simulator/test harness and validated adapters.

## Relationship to Bargain Bin Robotics

MURL supplies a durable software spine to the “clever instead of expensive” philosophy.

The cheap robot is allowed to remain cheap. Precision is not invented in software. Instead:

- hardware differences are disclosed;
- capabilities are measured;
- approximations are labeled;
- evidence is preserved;
- the same behavior can be compared across machines.

The educational promise is concise:

> **Learn one robot language. Investigate many machines. Preserve the evidence needed to understand why they behave differently.**

## Preservation status

This Markdown file preserves the architecture and Prospector concept in searchable form. The original manuals remain in the Intellectual Estate Library. The recovered v0.1 documents are **design/reference documents**, not proof that all adapters, Robot Passport tests or Prospector mission modules existed as a validated release.
