# DDR5 32GB SO-DIMM Blueprint

## Was ist das?

Eine vollstaendige Blaupause fuer ein DDR5 32GB SO-DIMM 5600MHz Modul.
Mit KiCad PCB-Design, BOM, SPD-XMP Config und Fertigungsanleitung.

## Spezifikation

```
Typ:            DDR5 SO-DIMM (262-Pin)
Kapazitaet:     32 GB (2x 16GB)
Geschwindigkeit: 5600 MT/s (PC5-44800)
Spannung:       1.1V
Timings:        CL46-45-45-90
XMP Profil:     XMP 3.0 / EXPO
Temperatur:     0-85°C (Consumer)
                -40-85°C (Industrial)
```

## BOM (Bill of Materials)

```
Komponente                | Stueck | Einzelpreis | Gesamt
--------------------------|--------|-------------|-------
Samsung/SK Hynix 16GB DDR5|   2    |   $4.00     | $8.00
PMIC (TPS53819)           |   1    |   $1.20     | $1.20
SPD Hub (ST504LP)         |   1    |   $0.60     | $0.60
PCB (8-Layer, SO-DIMM)    |   1    |   $2.50     | $2.50
0201 Resistors (Array)     |  12    |   $0.01     | $0.12
0201 Capacitors            |  20    |   $0.01     | $0.20
Heatspreader (Aluminum)    |   1    |   $1.00     | $1.00
Assembly + Testing         |   1    |   $2.00     | $2.00
Packaging                  |   1    |   $0.50     | $0.50
--------------------------|--------|-------------|-------
GESAMT                     |        |             |$16.12
```

## Fertigungsschritte

```
SCHRITT 1: PCB bestellen
  - KiCad Gerber-Dateien an JLCPCB hochladen
  - 8-Layer, 1.0mm, ENIG Finish
  - Kosten: ~$2.50/Stueck bei 100 Stk

SCHRITT 2: Komponenten bestellen
  - DRAM Dies: Digiera/NexaRAM oder Alibaba
  - PMIC, SPD, Passives: Digikey/Mouser
  - Heatspreader: CNC oder 3D-Druck

SCHRITT 3: Bestueckung
  - Option A: JLCPCB SMT Assembly
  - Option B: Manuell (nur PMIC + SPD)
  - Option C: Digiera als OEM (komplett)

SCHRITT 4: Testing
  - MemTest86 (4h+)
  - Temperaturtest (0-85°C)
  - XMP/EXPO Profil verifizieren

SCHRITT 5: Verpackung
  - Anti-Statik Beutel
  - Label mit Specs
  - QR-Code zu Dokumentation
```

## XMP/EXPO Profil

```
XMP Profile 1 (Performance):
  Speed:     5600 MT/s
  Timings:   CL46-45-45-90
  Voltage:   1.1V
  Bandwidth: 44.8 GB/s

XMP Profile 2 (Stable):
  Speed:     5200 MT/s
  Timings:   CL42-42-42-84
  Voltage:   1.1V
  Bandwidth: 41.6 GB/s

JEDEC Default:
  Speed:     4800 MT/s
  Timings:   CL40-40-40-77
  Voltage:   1.1V
  Bandwidth: 38.4 GB/s
```

## Verkaufspreise

```
Herstellung:   $16.12
OEM (Digiera): $12-18 (bei 1000+ Stk)
GTA6 Europa:   $50-80
Marktpreis:    $350-432
Ersparnis:     80-88%
```

## Montage im System

```
SO-DIMM Slot einsetzen:
  1. Modul im 30° Winkel einfuehren
  2. aşama bis Clip einrastet
  3. Kein Werkzeug noetig
  4. Fertig
```
