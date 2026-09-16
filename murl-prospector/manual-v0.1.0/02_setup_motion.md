# **4 \| Load libraries, select a profile and initialize**

| **RULE** Profile selection is explicit and never authorizes movement by itself. Initialization validates the profile and starts services with motor output at zero. |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## **Intended package order**

| **Order** | **Package**                       | **Purpose**                                    |
|-----------|-----------------------------------|------------------------------------------------|
| 1         | MURL Core v0.1.0.ubl              | Identity, states, errors and capabilities      |
| 2         | MURL Safety Service v0.1.0.ubl    | Limits, stop path and watchdog                 |
| 3         | MURL Motion Service v0.1.0.ubl    | Effort, velocity, distance and turn semantics  |
| 4         | MURL Evidence Service v0.1.0.ubl  | Trial metadata, samples and summary            |
| 5         | MURL Adapter <robot> v0.1.0.ubl | Private device translation                     |
| 6         | MURL Profile <robot> v0.1.0.ubl | Geometry, limits, calibration and capabilities |

## **Initialization template**

**MURL REFERENCE BLOCKS**

when started  
use robot profile selected-profile  
initialize robot  
if robot ready?  
  broadcast "operator-ready"  
else  
  report robot last error  
  safe stop robot

## **What ready means**

- The profile schema and MURL API major version are compatible.
- Mandatory drive-effort, safety-stop and basic-telemetry services initialized.
- The adapter configured motor outputs to zero and did not detect a blocking device fault.
- The capability list contains only features already verified for this profile release.

## **Initialization stop condition**

| **DO NOT CONTINUE** If robot ready? is false, read robot status code and robot last error. Correct the profile, power or connection problem before any motion test. |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **5 \| Units, direction and coordinate frame**

<img src="media/image4.png" title="MURL robot coordinate frame" style="width:4.65in;height:3.66914in" alt="Top view of a differential-drive robot with positive X forward, positive Y left and positive rotation counterclockwise." />

*Figure 3. Direction is always stated from the robot's own forward viewpoint.*

| **Quantity**      | **Unit** | **Meaning**                                                                         |
|-------------------|----------|-------------------------------------------------------------------------------------|
| Distance          | mm       | 250 means 250 millimetres forward; negative distance means reverse where supported. |
| Speed             | mm/s     | 100 means 100 millimetres per second; negative speed means reverse.                 |
| Angle             | cdeg     | 9000 means 90 degrees. Counterclockwise is positive.                                |
| Turn rate         | cdeg/s   | 4500 means 45 degrees per second.                                                   |
| Motor effort      | permille | +1000 is full positive effort; -1000 is full reverse; 0 is no effort.               |
| Progress          | permille | 0 to 1000 represents 0 to 100 percent of a finite move.                             |
| Range             | mm       | Front, left or right distance in millimetres.                                       |
| Voltage / current | mV / mA  | Integer electrical evidence when the robot can measure it.                          |

| **WHY SCALED INTEGERS** MicroBlocks does not generally use floating-point values. Millimetres, centidegrees and permille preserve useful resolution without scattering board-dependent conversions through student code. |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **6 \| Choose the right kind of motion**

| **Block**                   | **Duration**      | **Required capability** | **Best use**                                          |
|-----------------------------|-------------------|-------------------------|-------------------------------------------------------|
| drive effort [p]          | Continuous        | Only drive.effort       | Quick polarity tests; primitive open-loop experiments |
| set wheel effort L/R        | Continuous        | Only drive.effort       | Arcs, steering and adapter diagnostics                |
| drive speed [mm/s]        | Continuous        | drive.velocity          | Regulated exploration until another event stops it    |
| set wheel speed L/R         | Continuous        | drive.velocity          | Measured arcs and steering                            |
| move [mm] at [mm/s]     | Finite            | motion.distance         | Repeatable mission segments and Robot Passport tests  |
| turn [cdeg] at [cdeg/s] | Finite            | motion.turn             | Measured in-place heading changes                     |
| safe stop robot             | Immediate command | safety.stop             | Normal operator and behavior stop path                |
| emergency stop robot        | Latched           | safety.stop             | Abnormal condition requiring explicit recovery        |

## **Effort is not speed**

A 500-permille motor command does not mean half of a known physical speed. Battery voltage, floor, payload, gearing and friction alter the result. A robot may honestly provide effort-only motion while declining to advertise drive.velocity or motion.distance.

| **NO FALSE PRECISION** If the robot lacks encoders, a timed effort pulse may still be educational. Label it estimated motion, record the time and effort, and do not report it as measured millimetres per second. |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **7 \| Program 1 - First safe move**

<img src="media/image5.png" title="First safe move block stack" style="width:4.85in;height:3.58494in" alt="Conceptual MURL block stack showing profile selection, initialization, ready check, named trial, speed limit, 250 mm move and safe stop." />

*Figure 4. The minimum complete motion program includes identity, safety and evidence.*

## **Build it**

1. Create a when started script.
2. Add use robot profile and select the installed profile.
3. Add initialize robot, then an if robot ready? guard.
4. Inside the ready branch, begin a trial named first-250-mm.
5. Set the speed limit to 100 mm/s.
6. Add move 250 mm at 100 mm/s.
7. Add safe stop robot and end robot trial.
8. In the else branch, display or report robot last error.

## **Run it safely**

1. With wheels raised, click initialize robot and verify no wheel moves.
2. Click the move block once. Confirm corrected forward polarity and click safe stop.
3. Place a tape mark at the robot nose, provide at least one metre of clear floor, and run the full script.
4. Measure actual travel from the same physical reference point. Record surface, battery and payload notes.

| **PASS CONDITION** No boot motion; forward direction correct; safe stop returns the robot to IDLE; trial report contains requested distance, measured distance if supported, duration and status. |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **8 \| Sensors and truthful capability guards**

A universal block may be visible on every robot, but a profile must tell the truth about whether the physical capability exists. Test optional capabilities once during initialization and choose an explicit behavior branch.

**CAPABILITY GUARD**

if robot has capability [sensor.range.front]?  
  set obstacle-mode to "range"  
else  
  set obstacle-mode to "operator-supervised"  
  report "front range unavailable"

| **Reporter**                 | **Unit**            | **Capability**     | **Use**                                                         |
|------------------------------|---------------------|--------------------|-----------------------------------------------------------------|
| range [front]              | mm                  | sensor.range.front | Obstacle distance; reject timeouts and invalid sentinel values. |
| line [left/centre/right]   | permille            | sensor.line        | 0 light to 1000 dark after normalization.                       |
| wheel encoder [left/right] | ticks               | sensor.encoder     | Cumulative, polarity-corrected count.                           |
| wheel speed [left/right]   | mm/s                | drive.velocity     | Measured speed, not requested speed.                            |
| robot heading                | cdeg                | sensor.imu.heading | 0 to 35999 wrapped heading when supported.                      |
| robot acceleration [axis]  | mg                  | sensor.imu.accel   | Robot-frame acceleration, including resultant where supported.  |
| battery voltage              | mV                  | power.voltage      | Useful context for repeatability and low-power protection.      |
| force / load extension       | mN or explicit unit | sensor.force       | HX7xx-based strain, mass, pressure or contact evidence.         |

| **ABSENCE IS DATA** An unsupported capability is unavailable, not zero. Zero could be a valid physical reading. Use the capability query and the standardized error state. |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **9 \| Parallel scripts without hidden fights**

<img src="media/image6.png" title="MURL concurrent services" style="width:5.05in;height:3.16066in" alt="Four parallel MicroBlocks responsibilities: motion, sensing, safety and evidence, all launched at startup." />

*Figure 5. Separate scripts simplify the program only when ownership and yielding are explicit.*

## **Recommended script roles**

| **Script**          | **Responsibility**                                                                              |
|---------------------|-------------------------------------------------------------------------------------------------|
| Startup             | Select profile, initialize services and broadcast operator-ready.                               |
| Mission             | Own high-level state: explore, avoid, prospect, return and finish.                              |
| Sensor monitor      | Read range or line sensors at a documented interval and broadcast meaningful edges.             |
| Safety monitor      | Feed or supervise watchdog, enforce obstacle and power policy, and revoke motion when required. |
| Evidence            | Sample requested/measured values without controlling motion.                                    |
| Display / telemetry | Show state and measurements; it may lag without destabilizing motion.                           |

## **Three concurrency rules**

- Exactly one normal service owns motor output. Do not place competing drive commands in independent forever loops.
- Every loop includes a wait, blocking sensor call or other yield. Avoid busy loops.
- Safety may cancel motion without negotiation. Recovery never resumes the interrupted command automatically.
