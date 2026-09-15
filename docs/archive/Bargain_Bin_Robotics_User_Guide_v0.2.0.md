# Bargain Bin Robotics — User Guide v0.2.0

**Provenance:** repository-native text preservation of `Bargain_Bin_Robotics_User_Guide_v0.2.0.pdf` from the owner’s Library. The formatted PDF remains preserved separately. Text has been normalized to Markdown; engineering claims and validation boundaries are preserved rather than upgraded.

---

## Using low-cost robots with Alvik-style software and confidence-aware sensing

Arduino-IDE-first, RP2040/RP2350 carrier + ESP32-S3 supervisory architecture

Version 0.2.0 — Engineering documentation

## Contents

1. What Bargain Bin Robotics is
2. System architecture and supported robot paths
3. Install and first build in Arduino IDE
4. Safe bench bring-up
5. Everyday Alvik-style motion API
6. Calibrating cheap hardware
7. Lazy LiDAR: mapping with one ultrasonic sensor
8. Confidence-aware Safe Approach
9. Reading the occupancy map
10. ESP32-S3 supervisor
11. Troubleshooting and failure modes
12. What v0.2 does and does not promise

## 1. What Bargain Bin Robotics is

Bargain Bin Robotics is a cost-control robotics architecture. Its rule is simple: preserve the useful capability, not the expensive implementation. When a premium robot buys a better sensor, this project first asks whether deliberate motion, repeated observation, calibration, statistics and computation can recover enough of the same behavior using much cheaper hardware.

> **Core idea:** Substitute being clever for being rich. A cheap sensor is never declared equivalent to an expensive one; the software records uncertainty and accepts slower or more conservative behavior when that is the honest trade.

Version 0.2.0 keeps the Alvik-style motion API as the stable front end and adds optional Bargain Bin modules for robust range statistics, Lazy LiDAR scanning, local occupancy mapping, confidence-aware stopping and simple reactive steering.

| Layer | Purpose | Typical hardware |
|---|---|---|
| Supervisor | Wireless UI, mission logic, larger maps, logging | ESP32-S3 |
| Carrier | Real-time motor control, encoders, odometry, local safety | RP2040 or RP2350 |
| Power / chassis | Motors and driver sized for the robot | Maker Pi/Cytron or XRP |
| Cheap sensing | Range, line, IMU, optional camera/landmarks | Ultrasonic, LSM6DSO, line sensors |

## 2. System architecture and supported robot paths

```text
Phone / tablet / PC
        |
     Wi-Fi / BLE
        |
    ESP32-S3 supervisor
        | UART / simple commands
        v
RP2040 / RP2350 carrier
  - 200 Hz motor loop
  - encoder capture and PID
  - odometry / MOVE / ROTATE
  - local E-stop and deadman
  - sensor drivers
        |
     motors + cheap sensors
```

### Cytron / Maker Pi RP2040 path

This path is the most flexible and potentially the cheapest. The Maker Pi RP2040 supplies the RP2040, motor channels, servo ports and Grove connectors. To obtain accurate MOVE, ROTATE and odometry, add wheel encoders. For larger motors, keep the RP2040 software but use an appropriately rated Cytron power driver.

### SparkFun XRP Beta RP2040 path

The XRP Beta is the quickest full-featured target because the chassis already supplies wheel encoders, a two-motor driver, ultrasonic range, line sensors, servos and an LSM6DSO IMU. It is therefore the preferred first physical validation target for Lazy LiDAR.

### Current XRP RP2350 path

A preliminary RP2350 adapter is included. Treat it as a secondary validation target until its actual board behavior is compiled, flashed and checked.

## 3. Install and first build in Arduino IDE

1. Install Arduino IDE 2.x.
2. Install the Earle Philhower Arduino-Pico core for RP2040/RP2350 boards.
3. Copy the complete `AlvikRP_BargainBin_v0.2.0` folder into the Arduino libraries directory, then restart Arduino IDE.
4. Open **File > Examples** and select the carrier example for your board.
5. Choose the correct board and USB port. Compile before connecting motors to the floor.
6. Upload the carrier sketch and open Serial Monitor at 115200 baud.

> **Do not skip calibration.** The default geometry and encoder counts are starting values. A robot can drive with wrong values and still produce a very convincing but wrong map.

## 4. Safe bench bring-up

Raise the drive wheels clear of the table. Perform the following checks before any autonomous test.

```text
STATUS?
ENC?
WHEELS 15 15
WHEELS?
ENC?
STOP
```

Roll each wheel forward by hand. Encoder counts should increase. Then command a small positive wheel speed and confirm both wheels turn in the robot-forward direction. Correct encoder inversion separately from motor inversion.

### First geometric tests

```text
MOVE 100
TARGET?
POSE?
ROTATE 90
POSE?
```

Measure the actual 100 mm move and actual 90 degree turn. Wheel diameter primarily affects distance. Track width strongly affects turns. Tune using repeated measurements, not one lucky trial.

## 5. Everyday Alvik-style motion API

The friendly API deliberately resembles Arduino Alvik so sketches can focus on robot behavior rather than board-specific pins.

```cpp
#include <Arduino_Alvik.h>

robot.drive(150, 0, MM_S, DEG_S);
robot.rotate(90, DEG, true);
robot.move(300, MM, true);
robot.brake();

float x, y, heading;
robot.get_pose(x, y, heading, MM, DEG);
```

| Operation | Needs encoders? | Notes |
|---|---|---|
| drive / wheel speed | No, but better with them | Open-loop fallback exists on simple Maker Pi builds. |
| move / rotate | Yes | Refuses to fake closed-loop position. |
| pose / odometry | Yes | Wheel odometry; slip still causes drift. |
| IMU / line / distance | Depends on board | Unsupported hardware reports unsupported rather than invented values. |

## 6. Calibrating cheap hardware

### Encoder counts per wheel revolution

Send `ENC?`, rotate one wheel exactly one revolution by hand, and record the absolute tick change. Use that measured value as `encoderCountsPerWheelRev`. Quadrature libraries differ in whether they count pulses, cycles or every transition.

### Wheel diameter

Command several long straight moves, measure actual distance, and adjust effective wheel diameter. A longer test reduces ruler and stopping error.

### Track width

Command repeated 360 degree turns and compare the accumulated actual angle with the commanded angle. Adjust `wheelTrackMm` until rotation is repeatable.

### Ultrasonic characterization

Measure several surfaces at several distances. Hard flat walls are easy; curtains, angled walls, chair legs and soft surfaces can be difficult. Record mean error, scatter and failure rate. Those observations should eventually replace generic confidence constants.

## 7. Lazy LiDAR: mapping with one ultrasonic sensor

Lazy LiDAR is deliberately slow mapping. The robot rotates to known bearings, pauses, takes repeated ultrasonic measurements, removes gross outliers using median absolute deviation, and stores a mean range, standard deviation and confidence score for each bearing.

```cpp
#include <AlvikLazyLidar.h>
#include <AlvikOccupancyGrid.h>

AlvikLazyLidar scan(robot);
AlvikOccupancyGrid grid;

grid.begin(32, 32, 50.0f);
scan.scanRobotPivot();
grid.updateScan(x, y, heading, scan, 2000.0f);
grid.printAscii(Serial, robotCellX, robotCellY);
```

Open the example `BargainBin_LazyLidar_XRP_Beta`. Type `S` to make a slow scan and print the local map. The robot returns to its starting heading after the scan.

| Symbol | Meaning |
|---|---|
| `#` | strong occupied evidence |
| `+` | weaker occupied evidence |
| `.` | strong free-space evidence |
| `,` | weaker free-space evidence |
| blank | unknown / not sufficiently observed |
| `R` | robot cell |

> **Ultrasonic is not a laser ray.** Sonar has a broad beam and can reflect specularly. The v0.2 grid uses a simple ray model as an educational and engineering starting point. Repeated scans and confidence help, but the geometry is still an approximation.

## 8. Confidence-aware Safe Approach

The Safe Approach example does not stop at one fixed distance. It calculates a stopping clearance from current speed, configured deceleration, reaction time, fixed margin, measured range scatter and confidence. Weak measurements make the robot more conservative.

```cpp
AlvikRangeEstimate e = ranger.sampleForward();
float allowed = safety.limitForwardSpeed(180.0f, e);
if (allowed <= 1) robot.brake();
else robot.drive(allowed, 0, MM_S, DEG_S);
```

This illustrates the Bargain Bin rule: inexpensive sensing can still support useful safety behavior if uncertainty is explicitly included. It is not a certified safety system and must not be treated as one.

## 9. Reading the occupancy map

Each cell stores signed evidence rather than a permanent yes/no label. Free observations push evidence negative; obstacle endpoints push it positive. Repeated consistent observations therefore strengthen a conclusion, while unobserved cells remain unknown.

The grid is intentionally local and small. A 64 × 64 maximum grid uses 4096 bytes for evidence. Larger/global maps belong naturally on the ESP32-S3 supervisor in later versions.

## 10. ESP32-S3 supervisor

The S3 is not asked to generate motor PWM. It supervises the carrier. This separation keeps timing and local safety deterministic even when Wi-Fi or application code is busy.

```text
PING
DRIVE 150 0
MOVE 300
ROTATE 90
POSE?
DIST?
STOP
ESTOP 1
```

The supplied ESP32-S3 example bridges commands over Wi-Fi while keeping USB Serial available for bench work. The protocol is intentionally human-readable at this stage.

## 11. Troubleshooting and failure modes

| Symptom | Likely cause | First check |
|---|---|---|
| Robot drives backward | motor polarity/invert flag | small `WHEELS` command with wheels raised |
| Pose runs backward | encoder sign | roll wheel forward and read `ENC?` |
| MOVE distance wrong | CPR or wheel diameter | one-revolution count, then long straight test |
| ROTATE angle wrong | track width or wheel slip | repeated 360 degree turns |
| Lazy LiDAR map looks empty | invalid range or poor geometry | print per-bearing ranges/confidence |
| Map shows false wall | sonar reflection / broad beam | repeat from another pose and inspect confidence |
| Safe Approach will not move | range invalid/low confidence | test robust sample and raw sensor |
| Robot stops during DRIVE | deadman timeout | send heartbeat/commands continuously |

### Common sonar traps

- angled hard walls can reflect the echo away
- soft fabric can absorb sound
- narrow poles may be missed
- near-field readings can be invalid
- multiple surfaces can create multipath
- motion before the echo settles increases scatter

## 12. What v0.2 does and does not promise

v0.2 is an engineering drop. The advanced algorithms have native PC tests, but the complete package still needs Arduino-Pico compilation and physical tuning on the actual robots. The new modules are intentionally labeled experimental until that occurs.

| Present now | Not claimed yet |
|---|---|
| Alvik-style motion API | full SLAM / loop closure |
| encoder PID and odometry | global path planner |
| robust ultrasonic estimator | LiDAR-equivalent geometry |
| confidence-weighted local grid | certified collision safety |
| simple reactive corridor choice | camera perception / semantic mapping |
| ESP32-S3 supervision path | finished fleet protocol |

**Recommended next action:** validate XRP Beta first, measure its encoder and ultrasonic behavior, then carry the same software to the Cytron robot. That gives one known-good reference while the low-cost custom platform is brought up.
