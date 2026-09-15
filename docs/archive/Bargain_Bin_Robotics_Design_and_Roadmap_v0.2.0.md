# Bargain Bin Robotics — Design & Roadmap v0.2.0

**Provenance:** repository-native text preservation of `Bargain_Bin_Robotics_Design_and_Roadmap_v0.2.0.pdf` from the owner’s Library. The original formatted PDF remains preserved separately. Text and validation boundaries are retained as historical engineering documentation.

---

## Capability by computation: the plan behind the software

Arduino-IDE-first, RP2040/RP2350 carrier + ESP32-S3 supervisory architecture

Version 0.2.0 — Engineering documentation

## 1. The idea

Bargain Bin Robotics deliberately treats cost as an engineering constraint. The project asks what information an expensive subsystem provides, then seeks a cheaper way to recover enough of that information through motion, repeated measurements, statistical filtering, calibration and computation.

> **Design sentence:** Preserve the capability, not the expensive implementation.

## 2. Architecture

```text
ESP32-S3 supervisor: communications, history, maps, missions
             |
             v
RP2040/RP2350 carrier: motor control, encoders, local safety
             |
             v
cheap motors + cheap sensors + calibrated mechanics
```

## 3. v0.2 capability substitutions

| Expensive approach | Bargain Bin approach | Trade |
|---|---|---|
| 2-D LiDAR | single ultrasonic + controlled scan + statistics | time for money |
| precision obstacle system | repeated range + uncertainty-aware stopping | more conservative |
| large mapping computer | small local int8 grid on carrier/S3 | lower resolution |
| many range sensors | one sensor + robot/servo motion | latency |
| perfect mechanics | encoders + closed-loop control + calibration | tuning effort |

## 4. What was added now

- robust median/MAD range estimator with sigma and confidence
- Lazy LiDAR robot-pivot scan
- 4 KB maximum confidence-weighted occupancy grid
- dynamic stopping-distance/speed envelope
- simple confidence-aware open-corridor selector
- two XRP Beta demonstration sketches
- native PC tests
- full User and Programmer documentation

## 5. Validation sequence

1. compile all Arduino examples with Arduino-Pico
2. XRP Beta wheels-up motor/encoder verification
3. measure encoder counts per wheel revolution
4. tune straight movement and repeated turns
5. characterize sonar error/failure by distance and target material
6. compare Lazy LiDAR map to a simple measured course
7. port the now-measured behavior to the Cytron platform

## 6. Release roadmap

| Stage | Purpose |
|---|---|
| 0.3 | hardware truth |
| 0.4 | logging, replay and calibration repeatability |
| 0.5 | larger ESP32-S3 map and planning |
| 1.0 | stable, teacher-ready Bargain Bin Robotics platform |

## 7. Documentation contract

Every significant subsystem should arrive with its reason for existing, hardware assumptions, memory/time cost, minimal example, calibration procedure, expected output, failure modes and explicit non-guarantees. Documentation is treated as part of the feature.
