# DDR5 SO-DIMM PCB Layout

## ASCII Layout (Top View, 69.6mm x 30mm)

```
DDR5 SO-DIMM 32GB - PCB Layout (69.6mm x 30mm, 8-Layer)
================================================================

     Pin 1                                        Pin 262
     ▼                                              ▼
    ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
    │▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│  ← Gold-Pins (131)
    ├──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┤
    │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │
    │  │  ┌──────────────┐  │  │  │  │  │  │  │  ┌──────────────┐│
    │  │  │  DRAM 1      │  │  │  │  │  │  │  │  │  DRAM 3      ││
    │  │  │  Samsung     │  │  │  │  │  │  │  │  │  Samsung     ││
    │  │  │  16GB DDR5   │  │  │  │  │  │  │  │  │  16GB DDR5   ││
    │  │  │  (B-Die)     │  │  │  │  │  │  │  │  │  (B-Die)     ││
    │  │  └──────────────┘  │  │  │  │  │  │  │  └──────────────┘│
    │  │         │          │  │  │  │  │  │  │         │         │
    │  │    ┌────┴────┐     │  │  │  │  │  │  │    ┌────┴────┐    │
    │  │    │  PMIC   │     │  │  │  │  │  │  │    │  SPD    │    │
    │  │    │TPS53819 │     │  │  │  │  │  │  │    │ ST504LP │    │
    │  │    └─────────┘     │  │  │  │  │  │  │    └─────────┘    │
    │  │                    │  │  │  │  │  │  │                   │
    │  │  ┌──────────────┐  │  │  │  │  │  │  │  ┌──────────────┐│
    │  │  │  DRAM 2      │  │  │  │  │  │  │  │  │  DRAM 4      ││
    │  │  │  Samsung     │  │  │  │  │  │  │  │  │  Samsung     ││
    │  │  │  16GB DDR5   │  │  │  │  │  │  │  │  │  16GB DDR5   ││
    │  │  │  (B-Die)     │  │  │  │  │  │  │  │  │  (B-Die)     ││
    │  │  └──────────────┘  │  │  │  │  │  │  │  └──────────────┘│
    │  │                    │  │  │  │  │  │  │                   │
    │  │  ○ Heatspreader    │  │  │  │  │  │  │  ○ Heatspreader  │
    │  │    Mounting         │  │  │  │  │  │  │    Mounting      │
    │  │                    │  │  │  │  │  │  │                   │
    │  │  ═══════════════════════════════════════════════════════  │
    │  │            Signal Routing (DDR5 Bus)                      │
    │  │  ═══════════════════════════════════════════════════════  │
    │  │                    │  │  │  │  │  │  │                   │
    ├──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┼──┤
    │▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│▓▓│  ← Gold-Pins (131)
    └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
     ▲                                              ▲
     Pin 132                                      Pin 262

Layer Stack (8-Layer):
  ┌─────────────────────────────────────┐
  │ Layer 1:  Signal (Top)              │  ← DRAM, PMIC, SPD, R/C
  │ Layer 2:  GND Plane                │
  │ Layer 3:  Signal (Inner 1)          │  ← DDR5 Address/Command Bus
  │ Layer 4:  VDD Plane (1.1V)         │
  │ Layer 5:  VDD Plane (1.1V)         │
  │ Layer 6:  Signal (Inner 2)          │  ← DDR5 Data Bus (DQ/DQS)
  │ Layer 7:  GND Plane                │
  │ Layer 8:  Signal (Bottom)          │  ← Passives, Test Points
  └─────────────────────────────────────┘

Component Placement:
  U1, U2: Samsung 16GB DDR5 Die (Top, Left)
  U3, U4: Samsung 16GB DDR5 Die (Top, Right)
  U5:     PMIC TPS53819 (Top, Center-Left)
  U6:     SPD Hub ST504LP (Top, Center-Right)
  R1-R12: 0201 Resistor Arrays (Top, scattered)
  C1-C20: 0201 MLCC 100nF (Top, near ICs)
  TP1-TP8: Test Points (Bottom, edge)

Dimensions: 69.6mm x 30mm (JEDEC SO-DIMM Standard)
Trace Width: 0.075mm (signal), 0.15mm (power)
Via: 0.15mm drill, 0.25mm pad
Clearance: 0.075mm
Impedance: 50 Ohm single-ended, 100 Ohm differential
