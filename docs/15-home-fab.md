# Silizium-Platten & Belichtungsmaschine - Home Fab

## Kurzantwort

```
Silizium-Wafer:         KAUFEN - $2-15/Stueck (eBay)
Belichtungsmaschine:    SELBER BAUEN - ~$3,000 (Hacker Fab V2)
Gesamte Home Fab:       $5,000-50,000
Was man machen kann:    NMOS-Transistoren, simple ICs (1980er Level)
Was NICHT geht:         DRAM, CPU, anything modern (40 Jahre hinterher)
```

---

## Silizium-Wafer kaufen

```
WHER: eBay, AliExpress, WaferPro, Wafer World, Universitaeten

PREISE:
  2 Zoll (50mm):    $2-8/Stueck
  4 Zoll (100mm):   $8-15/Stueck
  6 Zoll (150mm):   $15-25/Stueck
  8 Zoll (200mm):   $25-60/Stueck
  
Sam Zeloof kaufte 25x 200mm Wafer mit Oxid+Polysilikon fuer $45
=> fuer Hobbyisten: $50-200 reicht fuer ein ganzes Jahr
```

**Empfehlung:** 4 Zoll (100mm) Wafer - gross genug fuer Tests, billig genug zum verschrotten.

---

## Belichtungsmaschine (Lithographie)

### Hacker Fab V2 (Open Source, CMU)

```
KOSTEN:     ~$3,015
LOESUNG:    2 Mikrometer
AUSRICHTUNG: 5 Mikrometer
MAX WAFER:  2cm x 2cm
BAUZEIT:    ~6 Stunden

Basis: TI DLPDLCR471TPEVM Eval-Board
       + 410nm Nah-UV LED
       + 10x Mikroskop-Objektiv
       + LabView/Arduino Steuerung
       
DOKU: docs.hackerfab.org
PAPER: arXiv:2510.15082
LIZENZ: CERN-OHL-W (Open Source!)
```

### Sam Zeloof's Mark IV (Maskless Stepper)

```
KOSTEN:     ~$5,000-10,000 (geschetzt)
LOESUNG:    ~300 Nanometer (sub-micron!)
BASIS:      DLP-Projektor + Mikroskop-Objektiv
STEUERUNG:  LabView + Schrittmotoren
KAMERA:     fuer Ausrichtung (Computer Vision)

Sam's Chips:
  Z1 (2018): 6 PMOS Transistoren, Differenzverstaerker
  Z2 (2021): 100 NMOS Transistoren, 10µm Gate
```

### Einfachste Loesung: DLP-Projektor

```
KOSTEN:     $200-500
LOESUNG:    10-50 Mikrometer
BASIS:     alter DLP-Projektor + Reduzieroptik
FUER:       PCB-Exposure, einfache ICs
```

---

## Komplette Home Fab

### Tier 1: Minimum (~$1,000-3,000)

```
DIY Rohrofen:              $200-500
Kontakt-Lithographie:      $100-300
Chemikalien:               $200-400
Mikroskop:                 $100-300
Vakuumkammer (gerettet):   $100-500
Spin Coater (DIY):         $50-100
────────────────────────────────────────
GESAMT:                    $800-2,100

KANN HERSTELLEN:
  - Dioden, einfache Transistoren
  - Feature-Groesse: 20-50µm
  - ~1968er Technologie
```

### Tier 2: Serios (~$5,000-15,000)

```
Hacker Fab V2 Stepper:     $3,000
Rohrofen (Labor):          $2,000-5,000
Vakuum-Sputtering:         $1,000-3,000
Spin Coater:               $500-1,500
Chemikalien + Sicherheit:  $500-1,000
Mikroskop + Kamera:        $500-1,000
Probe Station:             $500-2,000
Cleanbox/Glovebox:         $200-500
────────────────────────────────────────
GESAMT:                    $8,200-17,000

KANN HERSTELLEN:
  - NMOS Transistoren, simple ICs
  - Ring-Oszillatoren, Logik-Gates
  - Feature-Groesse: 2-10µm
  - ~1975-1984er Technologie
```

### Tier 3: Advanced (~$30,000-50,000)

```
Alles aus Tier 2, plus:
RIE (reaktives Ionen-Aetzen):  $5,000-15,000
Turbomolekular-Pumpe:          $3,000-8,000
SEM (Rasterelektronenmikroskop): $5,000-20,000
Halbleiter-Parameter-Analyser:  $2,000-10,000
Thermische Verdampfung:         $3,000-8,000
────────────────────────────────────────
GESAMT:                         $30,000-50,000

KANN HERSTELLEN:
  - Polysilikon-Gate NMOS
  - 100+ Transistor ICs
  - Feature-Groesse: 0.5-5µm
  - ~1984-1990er Technologie
```

---

## Was man im Garden Shed machen kann

### Echte Ergebnisse (2017-2026)

```
JERI ELLSWORTH:
  - Discrete Transistoren im Garage
  - YouTube: "Making Semiconductors from Scratch"

SAM ZELOOF (Flemington, NJ):
  - Z1 (2018): 6 PMOS Transistoren
  - Z2 (2021): 100 NMOS Transistoren, 10µm
  - 300nm Aufloesung mit eigenem Stepper
  - 66 Fertigungsschritte, 12 Stunden pro Run
  - 80% Ausbeute
  - Mitgeruestet von eBay fuer ~$5,000

DR. SEMICONDUCTOR (YouTube, April 2026):
  - 20 Bits DRAM im Garden Shed Cleanroom
  - 5x4 Array, Capacitor 12pF
  - Refresh: 2ms (vs 64ms kommerziell)
  - Proof-of-Concept

HACKER FAB (CMU + 7 weitere Unis):
  - 10µm NMOS Transistoren
  - 94% Ausbeute
  - 2,000+ Discord-Mitglieder
  - Komplett Open Source
```

---

## Was du NICHT machen kannst

```
NICHT MOEGLICH IM GARAGE FAB:
  - DRAM (DDR5 hat 16 Milliarden Transistoren pro Chip)
  - CPU/GPU (Milliarden Transistoren)
  - Anything unter 2µm (praktisch)
  - CMOS (benoetigt n-well + p-well Prozess)
  - Mehrschicht-Metallisierung (2+ Ebenen)
  - CMP (Chemisch-Mechanisches Polieren)
  
GAP: Home Fab = 1980er vs Industrie = 2026
     => 40 Jahre Unterschied
     => ~1 Milliarde x weniger Dichte
```

---

## Chip DESIGN vs Chip FABRIKATION

```
DESIGN (Software):           FABRIKATION (Hardware):
  - Jeder kann es lernen       - Extrem schwierig
  - Open Source Tools           - Teure Ausruestung
  - $0 Kosten                  - $5,000-50,000
  - Computer genuegt           - Chemikalien, Ofen, Vakuum
  - GDSII Datei als Output     - Physikalischer Chip
  - Google Open MPW:           - Home Fab: 
    KOSTENLOSER Chip!            Simple Transistoren
```

**Empfehlung:** Fang mit DESIGN an (OpenLane, Yosys, Magic) - dann bekommst du
einen echten 130nm Chip fuer $0 von Google/Efabless!

---

## Open Source Chip Projekte

### Google/Efabless Open MPW

```
KOSTEN:     $0 (wenn Design akzeptiert)
PROZESS:    SkyWater SKY130 (130nm)
TOOLS:      OpenLane, Yosys, Magic, KLayout
OUTPUT:     Echter Silizium-Chip!
MITMACHEN:  250+ Designs produziert
            60% Erstteilnehmer sind Nicht-IC-Experten
```

### Was du Designen kannst

```
MOEGLICH:
  - RISC-V Prozessor
  - Einfache状态机 (FSM)
  - ADC/DAC
  - Sensor-Interface
  - Custom Beschleuniger
  
TOOLS:
  - OpenLane (RTL zu GDS)
  - Yosys (Synthese)
  - Magic (Layout)
  - Verilog/VHDL (Beschreibungssprache)
  
LEARNING CURVE: Wochen bis Monate
```

---

## Kosten-Uebersicht

```
WAS DU WILLST           KOSTEN          WANN FERTIG
─────────────────────────────────────────────────────
Silizium-Wafer kaufen   $2-15/Stueck    Sofort
Hacker Fab V2 bauen     $3,000          1-2 Wochen
Complete Home Fab       $5,000-50,000   3-6 Monate
Chip Design lernen      $0              1-3 Monate
Open MPW Chip           $0              3-6 Monate (Design + Wartezeit)
```

---

## Empfehlung

```
WENN du Silizium-Belichtung willst:
  1. Hacker Fab V2 kaufen/bauen ($3,000)
  2. 4 Zoll Wafer auf eBay ($10/Stueck)
  3. Chemikalien besorgen ($500)
  4. NMOS Transistoren herstellen
  5. Lernen wie der Prozess funktioniert

WENN du echte Chips willst:
  1. OpenLane/Magic lernen (kostenlos)
  2. Einfaches Design machen
  3. Bei Google Open MPW einreichen
  4. KOSTENLOSEN 130nm Chip bekommen!

WENN du DRAM willst:
  - Vergiss es (zu Hause)
  - Kaufe es fuer $15 bei Digiera
  - Oder designing it als Projekt bei Open MPW
```
