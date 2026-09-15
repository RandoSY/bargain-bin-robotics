# Balance-Bot / M5Stack Bala2 exemplar

**Source repository:** https://github.com/RandoSY/balance-bot

**Source snapshot:** `258b4eb0ea6fe03cb43f2e954135f6a7cc325ed2` (4 Aug 2026)

**Disposition:** preserved Bargain Bin Robotics exemplar with firmware, telemetry, simulation, and test tooling.

This package comes from the M5Stack Bala2/Bala2-Fire self-balancing robot work. The legacy repository combines the original balance-controller firmware with later telemetry, simulation, dashboard, calibration, and testing work.

## Why it belongs here

Balance-Bot demonstrates several durable Bargain Bin Robotics ideas:

- expose control-state telemetry rather than treating the robot as a black box;
- make calibration parameters explicit;
- build a simulator before risking experiments on the physical machine;
- compare simulated and physical telemetry;
- keep safety-critical motor control local while observation can be remote;
- use inexpensive hardware as a platform for understanding feedback, sensing, and model error.

## Provenance and license

The source repository is not a GitHub fork, but its `LICENSE` identifies **M5Stack Technology CO LTD** as the 2026 MIT copyright holder for the software. That license is preserved in this migrated package. Estate-specific additions should therefore be understood as work layered on an M5Stack-derived base, not as a claim of exclusive authorship of the complete firmware.

## Migration scope

The canonical estate is copying the human-readable code and tooling. Large generated artifacts remain in the original repository, including the compiled `bala2.bin`, the large logo image, and the generated operator-manual PDF. The manual generator is source and is eligible for migration.

The original repository remains intact as the provenance record while this directory becomes the durable estate home for the exemplar.
