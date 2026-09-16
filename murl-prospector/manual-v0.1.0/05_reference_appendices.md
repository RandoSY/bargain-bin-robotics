# **22 \| Honest degradation**

| **Missing capability** | **Permitted behavior**                                  | **Forbidden claim**                                  |
|------------------------|---------------------------------------------------------|------------------------------------------------------|
| No encoders            | Timed low-effort motion labeled estimated               | Measured speed, distance accuracy or stall detection |
| No front range         | Operator-supervised course or alternate verified sensor | Automatic obstacle avoidance                         |
| No IMU heading         | Encoder-based turn if verified                          | Absolute heading                                     |
| No current sensing     | Time and motion evidence only                           | Energy or effort comparison based on guessed current |
| No BLE/Wi-Fi           | Local USB/editor report or on-device summary            | Remote telemetry                                     |
| Brake unavailable      | Coast stop plus warning                                 | Brake-mode performance                               |
| Sensor timeout         | Safe inhibit/stop and ERR_SENSOR                        | Treat timeout as a distant clear path                |

## **Capability-guard template**

if robot has capability [motion.distance]?  
  move [250 mm] at [120 mm/s]  
else  
  begin estimated motion  
  drive effort [300 permille]  
  wait [estimated interval]  
  safe stop robot  
  record warning [estimated-distance]

| **THE UNIVERSAL PROMISE** Universal does not mean every robot has every feature. It means the same project can discover the difference, choose an honest branch and report what happened in common terms. |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **23 \| Troubleshooting**

| **Symptom**                   | **Action**                                                                                                                    |
|-------------------------------|-------------------------------------------------------------------------------------------------------------------------------|
| No green connection indicator | Confirm a data-capable USB cable; close other serial/BLE clients; reconnect through the plug menu; verify the selected board. |
| Firmware incompatible         | Use update firmware on board from the gear menu. MicroBlocks 2.0 requires VM firmware 300 or later.                           |
| BLE robot not discoverable    | Disconnect USB/editor sessions that already own the board; confirm the board/profile supports BLE; power-cycle cleanly.       |
| Library blocks missing        | Use Add Library; for a development .ubl, drag the file into the IDE; confirm dependencies and version names.                  |
| Robot ready? is false         | Read robot status code and last error; verify profile/API compatibility, motor power and required controller.                 |
| One wheel reversed            | Raise wheels; correct that motor polarity in the profile. Do not reverse commands in every behavior.                          |
| Robot veers                   | Check wheel diameter, friction, payload and encoder polarity; compare measured wheel speeds before tuning control.            |
| Move overshoots               | Lower speed/acceleration; verify encoder scale; calibrate on the documented surface and load.                                 |
| Sonar jumps or times out      | Use a broad perpendicular target; respect sensor range; inspect surfaces and electrical timing; never treat timeout as clear. |
| FAULT will not clear          | Remove the underlying condition, then use clear recoverable robot fault. A current fault cannot be cleared.                   |
| E_STOP will not clear         | Release the physical trigger and perform the explicit reset procedure. Fresh motion command required.                         |
| No trial report               | Confirm begin robot trial and end robot trial both execute; check telemetry.basic and evidence service status.                |

# **24 \| Operator checklists**

## **Before power**

- Correct robot profile selected; release and calibration IDs recorded.
- Battery secure; motor switch reachable; cables clear of wheels.
- Course clear; robot cannot fall from an edge or reach a person.
- First motion or changed adapter: wheels raised.
- Speed, acceleration and watchdog limits conservative.

## **Before floor run**

- robot ready? true and state IDLE.
- Positive wheel polarity verified.
- safe stop robot verified.
- Required capability guards passed.
- Named trial started; surface, payload and battery noted.

## **After run**

- Robot in IDLE with outputs zero; physical power off before handling drivetrain.
- Trial ended and report saved with unique release/trial name.
- Requested and measured values both preserved.
- Faults, missed detections and degraded capabilities retained, not edited away.
- Calibration changes made only after repeated trials and followed by Robot Passport.

| **FILE NAMING** Use visible release and trial identifiers, for example: Prospector_Cutebot_Profile_v0.1.1.ubl and Prospector_Trial_0042_2026-08-14.csv. |
|---------------------------------------------------------------------------------------------------------------------------------------------------------|

# **25 \| MURL block quick reference**

| **Block**                      | **Operator meaning**                                |
|--------------------------------|-----------------------------------------------------|
| use robot profile [name]     | Select profile; does not energize motors            |
| initialize robot               | Validate and start services with outputs zero       |
| robot ready?                   | True only after required checks pass                |
| robot has capability [id]?   | Truthful optional-feature test                      |
| robot status code / last error | Machine code and operator explanation               |
| drive effort [permille]      | Continuous equal-wheel open-loop effort             |
| drive speed [mm/s]           | Continuous closed-loop speed                        |
| move [mm] at [mm/s]        | Finite measured linear move                         |
| turn [cdeg] at [cdeg/s]    | Finite measured in-place turn                       |
| safe stop robot                | Cancel motion and clear outputs through safety path |

## **Emergency control**

| **Block**            | **Operator meaning** |
|----------------------|----------------------|
| emergency stop robot | Latch emergency stop |

## **Sensors, limits and evidence**

| **Block**                      | **Operator meaning**                                 |
|--------------------------------|------------------------------------------------------|
| range [channel]              | Valid range in mm or sensor error                    |
| wheel speed / encoder          | Measured corrected motion evidence                   |
| robot heading / acceleration   | Robot-frame IMU evidence                             |
| set speed / acceleration limit | Clamp later commands below profile maxima            |
| set / feed watchdog            | Bound unattended continuous motion                   |
| robot safety state             | IDLE, MOVING, FAULT or E_STOP                        |
| begin / end robot trial        | Open and freeze a standardized record                |
| robot trial report             | Return identity, conditions, measurements and faults |

# **26 \| Status and error quick reference**

| **Code** | **Symbol**         | **Meaning**                                         |
|----------|--------------------|-----------------------------------------------------|
| 0        | OK                 | Completed normally                                  |
| 1        | ERR_UNINITIALIZED  | No valid initialized profile                        |
| 2        | ERR_BUSY           | Conflicting command or service owner                |
| 10       | ERR_CAPABILITY     | Required capability not advertised                  |
| 11       | ERR_PARAMETER      | Value outside permitted range                       |
| 12       | ERR_PROFILE        | Missing, incompatible or invalid profile field      |
| 20       | ERR_MOTION_TIMEOUT | Finite move exceeded allowed time                   |
| 21       | ERR_STALL          | Commanded motion without adequate measured response |
| 22       | ERR_OBSTACLE       | Safety policy stopped motion for obstacle           |
| 30       | ERR_SENSOR         | Sensor missing, timed out or invalid                |
| 31       | ERR_LOW_POWER      | Power policy prevented or stopped motion            |
| 32       | ERR_COMMUNICATION  | Required device/transport failed                    |
| 40       | ERR_WATCHDOG       | Heartbeat or control loop expired                   |
| 50       | ERR_ESTOP          | Emergency stop latched                              |
| 99       | ERR_INTERNAL       | Unexpected runtime or adapter failure               |

| **RECOVERY ORDER** Stop and make the scene safe; read state and error; remove the physical cause; clear only a recoverable fault; issue a fresh command. Never restart the interrupted motion automatically. |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

# **Appendix A \| Complete startup and mission outline**

**MURL V0.1 REFERENCE PSEUDOCODE**

when started  
use robot profile [selected-profile]  
initialize robot  
if not robot ready?  
  report robot last error  
  safe stop robot  
  stop this script  
set speed limit [120 mm/s]  
set acceleration limit [300 mm/s2]  
set watchdog interval [500 ms]  
broadcast [operator-ready]  

when I receive [operator-ready]  
clear path-log  
begin robot trial [prospector-01]  
repeat until resource-found?  
  feed robot watchdog  
  if range [front] < [180 mm]  
    safe stop robot  
    turn [9000 cdeg] at [4500 cdeg/s]  
    add [turn-left-90] to path-log  
    record event [avoidance]  
  else  
    move [250 mm] at [120 mm/s]  
    add [move-250] to path-log  
    sample and record resource signal  
reverse and execute path-log  
safe stop robot  
end robot trial  
report robot trial report

*This outline is intentionally readable and inspectable. Exact final block labels and list-handling details require usability testing and the first MURL package implementation.*

# **Appendix B \| Glossary and sources**

| **Term**       | **Meaning**                                                                             |
|----------------|-----------------------------------------------------------------------------------------|
| Adapter        | Private translation between MURL services and a robot-specific library or driver.       |
| Capability     | A named physical or communication feature that has been verified for a profile release. |
| Finite motion  | A bounded distance or turn operation that completes, faults or is cancelled.            |
| MURL           | Working title: MicroBlocks Universal Robotics Layer.                                    |
| Profile        | Machine-readable robot identity, geometry, limits, calibration and capabilities.        |
| Robot Passport | Common test project used to support each capability and accuracy claim.                 |
| Trial          | Named evidence interval with identity, conditions, requests, measurements and outcome.  |
| Watchdog       | Timer that stops continuous motion when the expected heartbeat or control loop expires. |

## **Official MicroBlocks sources consulted**

- MicroBlocks - What Is MicroBlocks
- MicroBlocks Wiki - User Guide
- MicroBlocks Wiki - Libraries
- MicroBlocks - MicroBlocks 2.0 launch and firmware transition
- MicroBlocks Wiki - Cutebot Pro Library
- MicroBlocks Wiki - XRP Library

*MicroBlocks source descriptions were checked on 14 August 2026. MURL-specific block labels, states, units and conformance rules come from the companion MURL Architecture and Block Specification v0.1.0.*
