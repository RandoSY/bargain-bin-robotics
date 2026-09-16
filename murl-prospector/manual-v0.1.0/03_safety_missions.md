# **10 \| Safety states, watchdog and events**

| **State**     | **Meaning**                                    | **Exit**                                       |
|---------------|------------------------------------------------|------------------------------------------------|
| UNINITIALIZED | Outputs off; no valid profile                  | Select profile and initialize                  |
| IDLE          | Ready; outputs zero                            | Valid motion command                           |
| MOVING        | Motion service owns drive output               | Completion, stop, fault or e-stop              |
| FAULT         | Recoverable condition latched; outputs cleared | Remove cause, then clear recoverable fault     |
| E_STOP        | Emergency stop latched; outputs cleared        | Physical release plus explicit reset procedure |

## **Continuous-motion watchdog pattern**

set watchdog interval [500 ms]  
drive speed [120 mm/s]  
repeat until mission-state = "stop"  
  feed robot watchdog  
  wait [100 ms]  
safe stop robot

| **Broadcast**         | **Receiving script action**                 |
|-----------------------|---------------------------------------------|
| murl.ready            | Initialization succeeded                    |
| murl.motion.complete  | Finite distance or turn completed           |
| murl.obstacle         | Obstacle policy stopped or inhibited motion |
| murl.fault            | Runtime entered FAULT; read status code     |
| murl.estop            | Runtime entered E_STOP                      |
| murl.telemetry.sample | New standardized evidence sample available  |

# **11 \| Program 2 - Front obstacle avoidance**

This program adds a range-dependent branch to the first safe move. It uses finite segments rather than an unbounded drive command, so each cycle creates a natural decision point and a measurable unit of work.

when I receive [operator-ready]  
begin robot trial [avoidance-01]  
set speed limit [120 mm/s]  
repeat until mission-state = "complete"  
  if range [front] < [180 mm]  
    safe stop robot  
    record event [obstacle]  
    turn [9000 cdeg] at [4500 cdeg/s]  
  else  
    move [250 mm] at [120 mm/s]  
safe stop robot  
end robot trial

## **Test progression**

| **Stage**           | **Operator evidence**                                                                                             |
|---------------------|-------------------------------------------------------------------------------------------------------------------|
| A - Reporter only   | Click range [front] and move a large flat target from 500 mm to 100 mm. Confirm direction and timeout behavior. |
| B - Wheels raised   | Run one detection cycle. Confirm the turn direction and that a range timeout does not authorize forward motion.   |
| C - Low-speed floor | Place a broad target at least 500 mm ahead. Verify stop threshold with tape.                                      |
| D - Ten-cycle trial | Repeat with several target materials and angles; report missed detections and false stops.                        |

| **ULTRASONIC LIMIT** Soft, angled or narrow objects may absorb or redirect ultrasound. A front range value is a sensor observation, not a guarantee that the path is clear. |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **12 \| Program 3 - Complete Prospector mission**

<img src="media/image7.png" title="Prospector mission flow" style="width:4.65in;height:3.9086in" alt="Flow from base calibration through exploration segments, obstacle avoidance, resource detection, return and reporting." />

*Figure 6. The mission joins movement and measurement without hiding either.*

## **Mission hypothesis**

| **QUESTION** Which robot or control strategy finds the resource and returns with the best combination of accuracy, safety, time and effort? |
|---------------------------------------------------------------------------------------------------------------------------------------------|

## **Resource signal**

The first classroom implementation should use one simple, repeatable signal: a bright/dark light patch, a line-marker pattern, a magnetic marker, a force/contact station or another sensor already represented in the profile. The mission code treats it as resource signal and records the raw normalized value plus the decision threshold.

| **Parameter** | **Starting value / rule**                                                |
|---------------|--------------------------------------------------------------------------|
| Course        | Clear 2 m x 2 m floor area with a marked base and broad obstacle targets |
| Segment       | 250 mm measured move                                                     |
| Explore speed | 120 mm/s maximum                                                         |

### **Mission thresholds and return**

| **Parameter**      | **Starting value / rule**                                                    |
|--------------------|------------------------------------------------------------------------------|
| Obstacle threshold | 180 mm front range                                                           |
| Avoid turn         | +9000 cdeg (90 degrees left)                                                 |
| Resource threshold | Chosen from a separate baseline trial, not guessed during the scored mission |
| Return method      | Reverse the recorded sequence of finite moves and turns                      |

# **13 \| Build the mission controller**

when I receive [operator-ready]  
set mission-state to "explore"  
clear path-log  
begin robot trial [prospector-01]  
repeat until resource-found?  
  if front-obstacle?  
    safe stop robot  
    turn [9000 cdeg] at [4500 cdeg/s]  
    add [turn-left-90] to path-log  
    record event [avoidance]  
  else  
    move [250 mm] at [120 mm/s]  
    add [move-250] to path-log  
    sample resource signal  
    record mission sample  
set mission-state to "return"  
reverse and execute path-log  
safe stop robot  
end robot trial  
report robot trial report

## **Why a path log comes first**

A full map or simultaneous localization system would obscure the educational core. A list of finite actions is inspectable: students can see what was commanded, reverse the order and invert each action. Later versions may compare this dead-reckoning return with IMU- or landmark-assisted strategies.

| **Recorded action** | **Reverse action**            |
|---------------------|-------------------------------|
| move +250 mm        | move -250 mm                  |
| turn +9000 cdeg     | turn -9000 cdeg               |
| turn -9000 cdeg     | turn +9000 cdeg               |
| avoidance record    | evidence only; do not execute |

| **LIMIT** Reversing commands is not guaranteed to return to the same physical point. Wheel slip, turn error and obstacle geometry accumulate. That discrepancy is the experiment, not a programming failure to hide. |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **14 \| Evidence Before the Equation**

<img src="media/image8.png" title="MURL evidence pipeline" style="width:5.1in;height:2.74615in" alt="Requested motion flows through robot sensors and MURL units into a report and a cross-robot comparison." />

*Figure 7. Requested values are intentions; measured values are evidence.*

## **Minimum mission report**

| **Group**    | **Required content**                                                                       |
|--------------|--------------------------------------------------------------------------------------------|
| Identity     | MURL API, profile ID/version, board family and firmware version                            |
| Conditions   | Surface, payload, battery voltage if available, course version and calibration ID          |
| Requests     | Distance, speed, turn, threshold and watchdog settings                                     |
| Measurements | Encoder distance, wheel speed, range, resource signal, duration and effort where available |
| Outcomes     | Resource found, return error, obstacle events, faults and final safety state               |

### **Capability honesty**

| **Group** | **Required content**                                     |
|-----------|----------------------------------------------------------|
| Honesty   | Advertised capabilities plus skipped or estimated fields |

## **Six Measures in one mission**

| **Measure**      | **Prospector evidence**                                  |
|------------------|----------------------------------------------------------|
| Time             | Duration, sample interval, reaction and stop latency     |
| Temperature      | Environment or motor temperature when available          |
| Light            | Line reflectance or a light-coded resource               |
| Sound / distance | Ultrasonic range or acoustic event                       |
| Strain           | Motor current, load, pressure or contact force extension |
| Motion           | Distance, wheel speed, heading, acceleration and jerk    |

# **15 \| Read, graph and compare the trial**

## **Live values**

During development, click reporter blocks to see immediate talk-bubble values. Use the MicroBlocks graph window for continuous sensor values. Keep telemetry rates modest so display work does not destabilize motion or sensor timing.

| **Signal**       | **Starting rate** | **Reason**                                                            |
|------------------|-------------------|-----------------------------------------------------------------------|
| Range            | 10-20 Hz          | Enough to see approach and stop threshold on a slow educational robot |
| Wheel speed      | 10-20 Hz          | Shows acceleration, regulation and left/right mismatch                |
| Heading          | 10-20 Hz          | Shows turn response and accumulated error                             |
| Battery / effort | 2-5 Hz            | Context without flooding the communications path                      |
| Trial summary    | Once at end       | Stable comparison record                                              |

## **Compare fairly**

- Use the same course geometry, resource marker and obstacle placement.
- Record surface, payload and battery condition.
- Run at least three trials per condition; do not select only the best run.
- Separate completion time from return error and motor effort.
- Report unavailable fields rather than treating missing evidence as zero.

| **FAIR MOTION SCORE - FUTURE EXTENSION** Your existing Motion + Effort idea can become a Robot EDU comparison: motion quality divided by normalized effort, with baselines and explicit capability limits. Keep the raw measurements beside any summary score. |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
