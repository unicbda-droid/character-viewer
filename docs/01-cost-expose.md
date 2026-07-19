# Kostenanalyse: Was Hardware wirklich kostet

## Zusammenfassung

| Produkt | Herstellung | Verkaufspreis | Aufschlag |
|---|---|---|---|
| DDR5 32GB SO-DIMM | $10-18 | $350-432 | 2,000-4,000% |
| DDR5 32GB Kit (2x16) | $16-28 | $432-600 | 2,000-2,500% |
| GPU Board (32GB VRAM) | $200-400 | $1,500-2,000 | 300-400% |
| PS5 Pro Konsole | $420-460 | $699 | 50-65% |
| Custom Wasserkuehlung | $30-50 | $300-500 | 600-900% |
| DP 2.1 Kabel (1m) | $8-15 | $30-50 | 200-300% |

## DDR5 RAM - Detaillierte Analyse

### Was kostet die Herstellung wirklich?

```
DDR5 32GB SO-DIMM 5600 BOM (Bill of Materials):
===============================================

DRAM Chips (2x 16GB):
  Samsung B-Die / SK Hynix / Micron    $8-12   (50-60% der BOM)
  DDR5 PMIC (Power Management IC)      $1.50-2.50
  SPD Hub (Serial Presence Detect)     $0.50-1.00
  PCB (6-8 Layer SO-DIMM)             $2-4
  Widerstaende, Kondensatoren          $0.50-1.00
  Heatspreader (Aluminium)             $1-2
  Montage & Testing                    $2-4
  Verpackung                           $0.50-1.00
  ──────────────────────────────────────
  Gesamt Herstellungskosten:           $16-27

  OEM-Preis (bei 1000+ Stueck):        $10-18
  Wholesale (China, Alibaba):           $11-15
```

### Preisentwicklung DDR5

```
DDR5 32GB SO-DIMM 4800:
  Q1 2024:   $45-60     (normal)
  Q3 2024:   $50-70     (leicht steigend)
  Q1 2025:   $80-120    (AI-Engpass beginnt)
  Q3 2025:   $200-300   (Krise)
  Q1 2026:   $350-432   (Peak)
  Q3 2026:   $280-380   (leicht sinkend)
  Q1 2027:   $200-300   (erwartet)

DDR5 32GB SO-DIMM 5600:
  Aktuell:   $350-432
  Herstellung: $15-25
  OEM (1000+): $10-18
```

### Wer verdient an DDR5?

```
Samsung / SK Hynix / Micron:
  - Produzieren die DRAM-Dies
  - Verkaufen an Module-Bauer (Apacer, Kingston, etc.)
  - Margen: 40-60%

Module-Bauer (Apacer, Transcend, Kingston):
  - Kaufen Dies + montieren auf PCB
  - Verkaufen an Haendler/Distributoren
  - Margen: 20-30%

Distributoren (Digikey, Mouser, Arrow):
  - Kaufen gross und verkaufen kleiner
  - Margen: 10-20%

Haendler (Amazon, Newegg, Mindfactory):
  - Verkaufen an Endkunden
  - Margen: 15-25%

Endkunden-Preis: 3x-4x des OEM-Preises
```

## GPU - Detaillierte Analyse

### BOM eines High-End GPU Boards

```
GPU Board mit 32GB VRAM BOM:
=============================

GPU Chip (z.B. AMD RDNA3 / Tenstorrent Blackhole):
  AMD Navi 48 / Tenstorrent Blackhole p150a    $100-200
  VRAM (8x 4GB GDDR6/6X oder 4x 8GB HBM)       $60-120
  VRM (Voltage Regulator Module)                 $15-25
  PCB (12-16 Layer, Hochgeschwindigkeit)         $30-60
  Kuehlloesung (Heatsink + Fans)                 $15-30
  DisplayPort 2.1 + HDMI 2.1 Connectoren         $5-10
  Passive Komponenten (Kondensatoren, etc.)      $5-10
  Montage & Testing                              $10-20
  Verpackung                                     $3-5
  ──────────────────────────────────────────────
  Gesamt Herstellungskosten:                     $250-480

  OEM-Preis (bei 100+ Stueck):                   $200-400
  Verkaufspreis (NVIDIA/AMD):                     $1,500-2,000
  Aufschlag:                                     300-400%
```

### GPU Markup-Vergleich

```
NVIDIA RTX 5090:
  BOM (TechInsights):    ~$400
  Verkaufspreis:         $1,999
  Aufschlag:             400%

NVIDIA RTX 5080:
  BOM:                   ~$250
  Verkaufspreis:         $999
  Aufschlag:             300%

AMD RX 9070 XT:
  BOM:                   ~$200
  Verkaufspreis:         $599
  Aufschlag:             200%

GTA6 Europa GPU Board:
  Herstellung:           ~$300-400
  Verkaufspreis:         $600-800
  Aufschlag:             50-100%
```

## Konsole - PS5 Pro Analyse

### TechInsights BOM

```
PS5 Pro BOM (TechInsights, 2024):
=================================

SoC (AMD Custom):
  CPU: 8-Core Zen 5                         $30-40
  GPU: RDNA 4 (16 CUs)                     $50-70
  Fertigung: TSMC 4nm                      (inkl.)
  
RAM:
  16GB GDDR6 (Samsung)                      $40-60
  
SSD:
  2TB NVMe (Samsung)                        $80-120
  
Other:
  Blu-Ray Drive                             $15-20
  WiFi 6E Modul                             $5-8
  Bluetooth Modul                           $2-3
  USB-C / USB-A Controller                  $3-5
  Power Supply (450W)                       $15-20
  Gehaeuse + Cooling                        $20-30
  Controller (DualSense)                    $15-20
  PCB + Montage                             $10-15
  Verpackung                                $5-8
  ──────────────────────────────────────────
  Gesamt BOM:                               $420-460
  Verkaufspreis:                            $699
  Aufschlag:                                50-65%
  
  Sony Margin (nach OpEx):                  ~$50-100 pro Konsole
```

### PS5 vs PS5 Pro Kostenvergleich

```
                    PS5 (2020)    PS5 Pro (2024)
SoC:                $35           $70  (+100%)
RAM:                $25           $50  (+100%)
SSD:                $40           $100 (+150%)
Rest:               $120          $200 (+67%)
─────────────────────────────────────────────
BOM Gesamt:         $220          $420 (+91%)
Verkaufspreis:      $499          $699 (+40%)
Aufschlag:          127%          66%

Fazit: PS5 Pro kostet 91% mehr zu fertigen,
       kostet den Kunden aber nur 40% mehr.
       => Sony verdient PROZENTUAL weniger am Pro,
       aber absolut mehr ($279 vs $279).
```

## Wasserkuehlung - Detaillierte Analyse

### BOM eines Custom Loops

```
Custom Water Cooling BOM:
=========================

CPU Waterblock (Kupfer):
  Kupferplatte + Acryl top                $5-8
  G1/4 Connectionen                       $1-2
  Montage                                 $1-2
  
GPU Waterblock:
  Kupferplatte + Acryl/Borosilikat        $8-12
  VRAM Kuehlung                           $2-3
  Connectionen                            $1-2
  
Pumpe (D5/DC-LT):
  Motor + Impeller                        $3-5
  Gehaeuse (Acryl/POM)                    $2-3
  
Radiatoren (240mm):
  Aluminium/Kupfer Fins                   $5-8
  Gehaeuse                                $2-3
  Luefter (2x 120mm)                      $3-6
  
Schlaeuche + Fittings:
  Soft-Tube (1m)                          $1-2
  Quick-Connect Kupplungen (4x)           $4-8
  G1/4 Fittings (6x)                      $3-5
  
Kuehlmittel:
  Prefilled (500ml)                       $2-3
  
  ──────────────────────────────────────────
  Gesamt Herstellungskosten:              $45-75
  Verkaufspreis (EKWB/Alphacool):        $300-500
  Aufschlag:                             400-600%
  
  GTA6 Europa Verkaufspreis:             $80-150
  Aufschlag:                             50-100%
```

## Kabel - Detaillierte Analyse

```
DP 2.1 Cable (1m, Active, 80Gbps):
====================================

  Controller Chip (VESA certified)        $3-5
  Kabel (4-lane, shielded)               $2-4
  Connectoren (DP 2.1)                   $1-2
  Montage & Testing                      $1-2
  ──────────────────────────────────────
  Gesamt:                                $8-15
  Verkaufspreis:                         $30-50
  Aufschlag:                             200-300%
  
  GTA6 Europa:                           $15-25
```

## Gesamt-Marktgroesse

```
Gaming Hardware Markt (2026, geschaetzt):
==========================================

RAM-Module (DDR5 Gaming):    ~$15 Mrd.
GPU (Gaming):                ~$40 Mrd.
Konsolen:                    ~$25 Mrd.
Cooling + Zubehoer:          ~$5 Mrd.
Kabel + Peripherie:          ~$3 Mrd.
─────────────────────────────────────────
Gesamt:                      ~$88 Mrd.

Potenzielle Ersparnis bei 10% Marktanteil:
  Durchschnittlicher Aufschlag: 300%
  Reduktion auf: 50%
  Ersparnis fuer Kaeufer: ~$15-20 Mrd./Jahr
```
