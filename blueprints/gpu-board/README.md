# GPU Board Blueprint

## Spezifikation

```
GPU:           Tenstorrent Blackhole p150a (oder AMD RDNA3 APU)
VRAM:          32 GB GDDR6/6X (8x 4GB)
Ausgabe:       2x DisplayPort 2.1 (80 Gbps)
               1x HDMI 2.1 (48 Gbps)
               1x USB-C (DP Alt Mode)
Kuehlung:      Quick-Connect Waterblock mount
Leistung:      120-150W TDP
Interface:     PCIe 5.0 x16
```

## BOM

```
Komponente                  | Stueck | Einzelpreis | Gesamt
----------------------------|--------|-------------|-------
Tenstorrent Blackhole p150a |   1    |  $150.00    | $150.00
GDDR6 4GB (Micron/Samsung)  |   8    |   $10.00    | $80.00
VRM (DrMOS + Inductors)     |   1    |   $18.00    | $18.00
PCB 12-Layer HDI            |   1    |   $45.00    | $45.00
DP 2.1 Connector            |   2    |    $3.00    | $6.00
HDMI 2.1 Connector          |   1    |    $2.00    | $2.00
USB-C Connector              |   1    |    $1.50    | $1.50
Passive Components          |   1    |    $8.00    | $8.00
Heatsink (Base)             |   1    |   $12.00    | $12.00
Assembly + Testing           |   1    |   $15.00    | $15.00
Packaging                    |   1    |    $4.00    | $4.00
-----------------------------|--------|-------------|-------
GESAMT                       |        |             |$341.50
```

## Display-Ausgabe

```
DisplayPort 2.1 (UHBR20):
  4K@60Hz  4:4:4 10-bit   (36 Gbps, kein DSC)
  4K@120Hz 4:4:4 10-bit   (72 Gbps, kein DSC)
  4K@144Hz 4:4:4 10-bit   (86 Gbps, kein DSC)
  4K@240Hz 4:4:4 10-bit   (144 Gbps, mit DSC)
  8K@60Hz  4:4:4 10-bit   (144 Gbps, mit DSC)
  8K@30Hz  4:4:4 10-bit   (72 Gbps, kein DSC)

PAL-Kompatibilitaet:
  1080i@50Hz, 1080p@50Hz, 1080p@100Hz
  2160p@50Hz, 2160p@100Hz (4K PAL)
  720p@50Hz
```

## Fertigung

```
SCHRITT 1: Gerber + BOM an JLCPCB
SCHRITT 2: 12-Layer PCB bestellen ($45/Stk bei 50 Stk)
SCHRITT 3: SMT Assembly bei JLCPCB
SCHRITT 4: Waterblock montieren (4 Schrauben)
SCHRITT 5: Testing (GPU-Z, Unigine, 4h Stability)
```

## Montage im System

```
1. PCIe x16 Slot: Board einsetzen, clippen
2. Waterblock: 4x Schrauben (mitgeliefert)
3. Quick-Connect: Kuehlschlauch anklicken
4. Power: 2x 8-Pin PCIe (oder 1x 12VHPWR)
5. Fertig
```
