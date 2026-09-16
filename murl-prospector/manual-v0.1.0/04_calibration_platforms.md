# **16 \| Calibrate distance and turning**

<img src="media/image9.png" title="MURL calibration loop" style="width:4.65in;height:3.12281in" alt="Four-step cycle: command, measure, adjust profile, repeat three times under the same conditions." />

*Figure 8. Calibration is a repeated measurement loop, not a one-run correction.*

## **Distance calibration**

1. Mark a straight start line and a 1000 mm target line on a consistent floor surface.
2. Use the same nose or axle reference point for every start and finish measurement.
3. Run three 1000 mm moves at 150 mm/s and record actual travel.
4. Compute mean measured distance. Adjust only the profile distance scale or wheel-diameter calibration field.
5. Repeat three trials and preserve both pre- and post-calibration data.

## **Turn calibration**

1. Mark the robot's initial heading with a long floor line.
2. Run three +18000 cdeg turns and three -18000 cdeg turns.
3. Measure final heading error in each direction.

### **Turn-scale adjustment**

1. Adjust the track-width or turn-scale calibration field, not the student mission code.
2. Repeat under the documented surface, payload and battery condition.

| **DO NOT HIDE ASYMMETRY** If clockwise and counterclockwise errors differ, preserve both. A single scale factor may not explain friction, motor mismatch or mechanical alignment. |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **17 \| Robot Passport checkout**

| **ID** | **Test**    | **Operator action**                                |
|--------|-------------|----------------------------------------------------|
| P0     | Identity    | Read profile, API version and capabilities         |
| P1     | Safe boot   | Observe outputs before and during initialization   |
| P2     | Polarity    | Raise wheels; command low positive effort          |
| P3     | Open loop   | Short straight effort pulse, then safe stop        |
| P4     | Encoders    | Rotate each wheel and verify corrected counts      |
| P5     | Distance    | Three 1000 mm moves at 150 mm/s                    |
| P6     | Turning     | Three positive and three negative 180-degree turns |
| P7     | Sensors     | Exercise every advertised sensor channel           |
| P8     | Watchdog    | Interrupt heartbeat during controlled motion       |
| P9     | Persistence | Restart; verify no motion and correct profile      |
| P10    | Evidence    | Export complete standardized report                |

## **Initial claim tiers**

| **Claim**          | **Initial acceptance criterion**                                      |
|--------------------|-----------------------------------------------------------------------|
| MURL Core          | Safe boot, drive effort, stop and basic telemetry                     |
| Motion Standard    | Distance error <= max(100 mm, 10%); turn error <= 10 degrees        |
| Motion Precision   | Distance error <= max(50 mm, 5%); turn error <= 5 degrees           |
| Evidence           | Identity, requested/measured/error, timing, states and faults present |
| Portable Reference | Same project on three adapters across at least two MCU families       |

| **CLAIM ONLY WHAT PASSES** A skipped optional capability is not a failure. Advertising an untested capability is. |
|-------------------------------------------------------------------------------------------------------------------|

# **18 \| Move the same project between robots**

<img src="media/image10.png" title="MURL platform mapping" style="width:5.05in;height:3.2101in" alt="One Prospector behavior project maps through adapters to Cutebot Pro, XRP and a generic Pico robot." />

*Figure 9. Profiles state the truth; adapters translate the machinery.*

| **Item**     | **Portability rule**                                                                   |
|--------------|----------------------------------------------------------------------------------------|
| Project      | Do not edit behavior blocks or physical units.                                         |
| Profile      | Select one profile explicitly before initialization.                                   |
| Capabilities | Run the same guards; different robots may choose different honest branches.            |
| Passport     | Use the identical conformance project for every adapter.                               |
| Report       | Compare profile identity, capability set, error and effort - not just completion time. |

## **What may change**

- The selected profile name and its measured calibration values.
- Whether an optional behavior branch is available.
- Accuracy tier and telemetry fields.
- Low-level adapter implementation and native library dependencies.

## **What must not change**

- Meaning of millimetres, millimetres per second, centidegrees and permille.
- Safety-state meanings and recovery rules.
- Student mission logic simply to accommodate a different motor API.
- Evidence field names for the same physical quantity.

# **19 \| Cutebot Pro 2.1 operator mapping**

| **AVAILABLE NOW** MicroBlocks has an existing Cutebot Pro library with encoder-aware wheel speed, distance moves, turns, front sonar, line tracking, lights, servos and low-level encoder/orientation access. The MURL adapter described here has not yet been released. |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

| **MURL intention**   | **Native starting point**                  | **Adapter responsibility**                                 |
|----------------------|--------------------------------------------|------------------------------------------------------------|
| drive speed 150 mm/s | CBPro set wheel speed 15 cm/s per wheel    | Convert mm/s to cm/s; correct left/right convention        |
| move 1000 mm         | CBPro move 100 cm in selected direction    | Convert mm to cm; enforce MURL limits and status           |
| turn +9000 cdeg      | CBPro turn 90 degrees, direction corrected | Convert centidegrees; reconcile sign convention            |
| range front          | CBPro distance in cm                       | Convert cm to mm; propagate timeout/invalid status         |
| wheel speed          | CBPro speed of wheel in cm/s               | Convert to mm/s; normalize wheel names                     |
| safe stop            | CBPro stop all / stop wheel                | Use strongest repeatable stop path and verify output clear |
| encoder evidence     | _cbpro_readEncoders variables             | Keep private; expose corrected standard reporters          |

## **First native checkout**

1. Insert the micro:bit v2 and power the Cutebot Pro main board; several components require the battery and main power switch.
2. Add the Cutebot Pro library. Raise the wheels.
3. Click CBPro set wheel speed with a small equal forward value. Verify both wheels and stop immediately.
4. Clear encoder/orientation values, perform a short move and read encoder values.
5. Read sonar against a broad target at known distances before enabling obstacle avoidance.

| **SIGN CONVENTION** The native Cutebot Pro turn documentation describes positive angles as clockwise. MURL defines positive rotation as counterclockwise. The adapter must correct this; the student project must not. |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **20 \| XRP operator mapping**

| **AVAILABLE NOW** The MicroBlocks XRP library already uses millimetres and millimetres per second for wheel motion, includes encoded motors and exposes its PID-based control in MicroBlocks. It is the closest existing native match to the MURL motion semantics. |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

| **MURL intention**         | **Native XRP starting point**      | **Adapter responsibility**                            |
|----------------------------|------------------------------------|-------------------------------------------------------|
| drive speed 150 mm/s       | drive at speed 150 mm/s            | Map wheel sign convention and service ownership       |
| move 1000 mm at 150 mm/s   | drive distance 1000 mm at 150 mm/s | Wrap status, timeout, progress and safety             |
| turn +9000 cdeg            | turn angle 90 degrees              | Convert centidegrees and verify sign                  |
| safe stop                  | stop both wheels                   | Verify output clear and state transition              |
| wait for finite completion | wait for wheels to stop            | Yield while safety and evidence scripts continue      |
| range front                | read distance sensor in cm         | Convert centimetres to millimetres                    |
| IMU turn rate              | read roll/pitch/yaw rate           | Normalize axes and centidegrees per second if exposed |
| encoder configuration      | Encoded DC Motors + PID libraries  | Keep tuning private to adapter/profile release        |

## **Why XRP is educationally valuable**

The XRP library is written in MicroBlocks down to the PID loop, so students and adapter developers can inspect the control path rather than treating the robot as an opaque motor appliance. MURL should preserve that inspectability while stabilizing the public semantics.

| **DO NOT TUNE LIVE DURING A SCORED TRIAL** PID values belong to a documented profile/calibration release. Change them in a dedicated tuning trial, then rerun Robot Passport. |
|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **21 \| Generic Pico/Pico W reference robot**

| **ADAPTER WORK** The transparent Pico/Pico W robot is the recommended first MURL prototype because every motor pin, encoder edge, sensor timing choice and calibration constant can be inspected. It is not a completed adapter in v0.1.0. |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

| **Profile field** | **Operator / adapter input**                         |
|-------------------|------------------------------------------------------|
| board.family      | rp2040 or rp2040w                                    |
| drive.geometry    | differential                                         |
| motor pins        | PWM and direction pins for left/right channels       |
| encoder pins      | A/B or single-channel pins plus counts/revolution    |
| wheel.diameter_mm | Measured loaded rolling diameter                     |
| wheel.track_mm    | Centre-to-centre drive-wheel spacing                 |
| polarity          | +1 or -1 for each motor and encoder                  |
| limits            | Conservative speed, acceleration and watchdog values |
| front range       | HC-SR04 or ToF port plus timing/valid range          |
| evidence          | Encoder, time, battery/current if available          |

## **Bring-up order**

1. Prove safe output-low boot with the motor power disconnected.
2. Verify each direction pin and PWM channel at low effort with wheels raised.
3. Correct motor polarity in the profile; do not swap signs throughout application code.
4. Capture encoder edges and verify positive forward counts on both wheels.
5. Add the lowest reliable stop path and watchdog before finite motion.
6. Calibrate distance, then turns, then speed regulation.
7. Add front range and evidence telemetry only after motion remains stable.

| **NATIVE PRIMITIVES** Add SmallVM primitives only when repeatable traces show that a portable MicroBlocks library cannot meet the advertised timing or accuracy. Keep those changes minimal and board-family specific. |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
