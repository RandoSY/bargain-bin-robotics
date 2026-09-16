# **Read this first**

| **STATUS** MicroBlocks is usable today. MURL v0.1.0 is a reference architecture and block contract, not yet an installable universal-robot package. The programs in this manual specify the intended MURL experience and show how existing Cutebot Pro and XRP libraries map into it. |
|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## **Safety boundary**

This manual is for small educational differential-drive robots. MURL is
not a certified safety system. During setup and calibration, the
operator remains responsible for physical clearance, battery isolation
and an immediately reachable power switch.

- Raise the drive wheels before the first motor or polarity test.

- Use conservative speed and acceleration limits before placing the
  robot on the floor.

- Keep hair, clothing, cables and fingers away from wheels, gears and
  shafts.

- Use safe stop robot before relying on the editor Stop control; a
  stopped script does not prove every external motor controller has
  cleared its last command.

- After an emergency stop, remove the cause and perform the explicit
  reset procedure. Never allow automatic motion resumption.

- Do not test on tables, stairs, near people or where a runaway robot
  could cause damage.

## **Document control**

| **Field**               | **Value**                                                          |
|-------------------------|--------------------------------------------------------------------|
| Document                | MURL Prospector Robot - Operator's Manual and Programming Guide    |
| Release                 | v0.1.0 reference programming edition                               |
| Audience                | Teacher, student, maker, adapter developer and laboratory operator |
| Primary robot class     | Small two-wheel differential-drive educational robots              |
| Reference targets       | Cutebot Pro 2.1, XRP and a transparent Pico/Pico W build           |
| Companion specification | MURL Architecture and Block Specification v0.1.0                   |
| Date                    | 14 August 2026                                                     |

# **Contents and programming path**

<table>
<colgroup>
<col style="width: 25%" />
<col style="width: 74%" />
</colgroup>
<thead>
<tr class="header">
<th><strong>Part</strong></th>
<th><strong>What you will do</strong></th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td>START HERE</td>
<td>1. Manual status and safety<br />
2. Ten-minute quick start<br />
3. MicroBlocks operating model</td>
</tr>
<tr class="even">
<td>PROGRAM MURL</td>
<td>4. Profiles and initialization<br />
5. Motion and coordinates<br />
6. Sensors, capabilities and concurrency<br />
7. Safety, events and evidence</td>
</tr>
<tr class="odd">
<td>BUILD THE MISSION</td>
<td>8. First safe move<br />
9. Obstacle avoidance<br />
10. Prospect, record and return<br />
11. Calibration and Robot Passport</td>
</tr>
<tr class="even">
<td>MOVE BETWEEN ROBOTS</td>
<td>12. Cutebot Pro<br />
13. XRP<br />
14. Generic Pico/Pico W<br />
15. Troubleshooting and quick reference</td>
</tr>
</tbody>
</table>

| **FASTEST PATH** Read the safety page, perform the ten-minute quick start, then build Programs 1-3 in order. Use the platform pages only when selecting or diagnosing an adapter. |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## **Reading labels**

| **Label**          | **Meaning**                                                                                          |
|--------------------|------------------------------------------------------------------------------------------------------|
| AVAILABLE NOW      | A verified MicroBlocks editor, VM or existing robot-library operation.                               |
| MURL 0.1 REFERENCE | A block or behavior defined by the architecture but not yet released as an installable MURL package. |
| ADAPTER WORK       | Robot-specific translation or validation required before claiming the capability.                    |
| OPERATOR ACTION    | A physical setup, observation or measurement that software cannot perform for you.                   |

# **1 \| Ten-minute quick start**

<img src="media/image2.png" title="MURL quick-start loop"
style="width:5.45in;height:3.41101in"
alt="Eight-step quick-start flow from making the robot safe through connection, initialization, live testing and evidence verification." />

*Figure 1. The first session is a short, closed safety-and-evidence
loop.*

| **OUTCOME** At the end of this procedure the robot has moved only 250 mm at a 100 mm/s limit, stopped through the MURL safety path and produced a named trial record. |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------|

1.  Place the robot on a stable stand so both drive wheels can rotate
    without touching the surface. Switch the robot power off.

2.  Open the MicroBlocks web editor in Chrome or Edge, or open the
    installed desktop editor.

3.  Connect a data-capable USB cable. Use the plug icon and choose
    connect (USB). A green connection indicator and board name confirm
    the connection.

4.  If prompted that the firmware is incompatible, use the gear menu to
    update the firmware on the board before proceeding.

5.  Add the required MURL libraries and exactly one robot
    profile/adapter. Until those packages exist, add the native Cutebot
    Pro or XRP library and use the platform mapping in Part IV.

6.  Select the profile, initialize the robot and confirm robot ready?
    while the wheels remain raised.

7.  Click a single low-energy motion block to test it live. Confirm both
    wheels interpret positive motion as forward, then click safe stop
    robot.

8.  Place the robot on a clear floor, run the 250 mm first-move project,
    measure the result and inspect the trial report.

# **2 \| How MicroBlocks changes the programming rhythm**

MicroBlocks combines a blocks editor on the host, a bytecode virtual
machine on the board and a communication path that updates the board
incrementally. That creates a different operating rhythm from
edit-compile-download environments: test one physical idea immediately,
observe it, then assemble it into a persistent autonomous project.

| **MicroBlocks property** | **Operator consequence**                                                                                                                 |
|--------------------------|------------------------------------------------------------------------------------------------------------------------------------------|
| Live programming         | Click an individual block or script and it runs immediately on the connected board. Use this for low-energy tests and sensor inspection. |
| Autonomous operation     | Code is stored on the board as it is edited. Scripts under when started run when the robot is powered without the editor.                |
| Parallel scripts         | Motion, sensing, safety and evidence can run as separate scripts. MicroBlocks supports multiple concurrent scripts.                      |
| Portable bytecode        | The VM runs equivalent project code across multiple supported processor families; MURL standardizes robot meaning above that layer.      |
| Read back                | Reconnect a programmed board and the editor can read its scripts back, preserving the inspectable educational object.                    |

## **The operating rule**

| **ONE PHYSICAL QUESTION AT A TIME** First ask: Does this block produce the intended physical effect? Then ask: Does the assembled behavior remain safe, repeatable and measurable? |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## **Editor controls you will use**

| **Control**    | **Use**                                                                                                                         |
|----------------|---------------------------------------------------------------------------------------------------------------------------------|
| Plug / Connect | Choose USB, supported BLE or Boardie. A physical robot requires the correct board connection and firmware.                      |
| Add Library    | Load bundled robot/sensor libraries. A custom .ubl file may be dragged into the IDE for development and testing.                |
| Start          | Starts all when started and conditional scripts, simulating power-on.                                                           |
| Stop           | Stops project scripts and deallocates variables. Use the robot-specific safe-stop path before assuming motor output is cleared. |
| Graph          | Plots values sent through the graph block; useful for range, speed, effort and calibration evidence.                            |

# **3 \| The MURL programming model**

<img src="media/image3.png" title="MURL reference architecture"
style="width:5.05in;height:3.51291in"
alt="Six layers from student behavior through robot services, universal API, profile and adapter, MicroBlocks VM and physical robot." />

*Figure 2. Students program stable meanings; the profile and adapter
preserve hardware truth.*

The application should never write motor pins or controller registers.
It asks the universal API for a physical intention. The profile supplies
measured robot facts and conservative limits. The adapter translates the
request into the robot-specific library or driver.

| **Layer**      | **What the programmer relies on**                                                   |
|----------------|-------------------------------------------------------------------------------------|
| Behavior       | Explore, avoid, prospect, return and report.                                        |
| Robot services | Motion, sensing, safety and evidence own their respective responsibilities.         |
| Universal API  | Stable block labels, units, states, errors and events.                              |
| Profile        | Robot identity, geometry, polarity, limits, calibration and verified capabilities.  |
| Adapter        | Translation to Cutebot Pro I2C services, XRP libraries or transparent Pico drivers. |
| MicroBlocks VM | Live bytecode execution, persistence and concurrency.                               |
