# SBC Console Blueprint - PS6 Alternative

## Spezifikation

```
SoC:           Tenstorrent Blackhole p150a ($1,399)
               ODER AMD RDNA3 APU (Custom, ~$120 OEM)
RAM:           DDR5 SO-DIMM (32GB, upgradebar)
Storage:       M.2 NVMe (2TB)
Ausgabe:       2x DisplayPort 2.1, 1x HDMI 2.1, 1x USB-C
I/O:           USB3, Ethernet 2.5G, WiFi 6E, BT 5.3
Power:         12V DC Input (150W max)
Kuehlung:      Quick-Connect Wasserkuehlung
Gehaeuse:      3D-druckbar (FreeCAD parametrisch)
```

## BOM

```
Komponente                  | Stueck | Einzelpreis | Gesamt
----------------------------|--------|-------------|-------
SoC (Tenstorrent/RDNA3)     |   1    |  $120.00    | $120.00
DDR5 SO-DIMM 32GB           |   1    |   $15.00    | $15.00
NVMe SSD 2TB                |   1    |   $90.00    | $90.00
PCB 8-Layer Mainboard       |   1    |   $35.00    | $35.00
DisplayPort 2.1             |   2    |    $3.00    | $6.00
HDMI 2.1                    |   1    |    $2.00    | $2.00
USB-C + USB-A Controller    |   1    |    $4.00    | $4.00
WiFi 6E + BT 5.3            |   1    |    $6.00    | $6.00
Ethernet 2.5G PHY           |   1    |    $3.00    | $3.00
Power Supply 12V DC          |   1    |   $10.00    | $10.00
Passive Components          |   1    |   $12.00    | $12.00
Assembly + Testing           |   1    |   $20.00    | $20.00
Packaging                    |   1    |    $5.00    | $5.00
-----------------------------|--------|-------------|-------
GESAMT                       |        |             |$328.00
```

## Display-Ausgabe (PS5 Vergleich)

```
PS5 Pro:           HDMI 2.1 (48 Gbps)  → max 4K@120Hz
GTA6 Europa SBC:   DP 2.1 (80 Gbps)   → 4K@240Hz, 8K@60Hz

PAL-Kompatibilitaet:
  50Hz:  720p, 1080p, 1440p, 4K
  100Hz: 720p, 1080p, 1440p, 4K
  VRR:   50-240Hz variabel

Cursor-Skalierung:
  Vector-Cursor (kein Pixel-Raster)
  Native Rendering in Display-Aufloesung
  Immer scharf bei 4K/8K
```

## Montage (LEGO-Prinzip)

```
LEVEL 1 (0 Werkzeug):
  1. DDR5 SO-DIMM in Slot (30° Winkel, clippen)
  2. NVMe SSD in M.2 Slot (Schraube, mitgeliefert)
  3. Fertig

LEVEL 2 (1 Schraubenzieher):
  4. Waterblock auf SoC (4 Schrauben)
  5. Radiator am Gehaeuse (4 Schrauben)
  6. Gehaeuse zusammenschrauben (6 Schrauben)

LEVEL 3 (Quick-Connect):
  7. Waterblock → Quick-Connect → Pumpe → Quick-Connect → Radiator
  8. Eindruecken, Klick, fertig

GESAMT: 30 Minuten, kein Loeten
```
