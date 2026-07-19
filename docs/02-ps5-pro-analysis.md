# PS5 Pro BOM-Analyse (TechInsights)

## Technische Daten

```
SoC:
  CPU: AMD Zen 5, 8 Cores, 3.85 GHz (5.0 GHz Boost)
  GPU: RDNA 4, 16 CUs, 16.7 TFLOPS
  Fertigung: TSMC 4nm

RAM:
  Typ: GDDR6
  Kapazitaet: 16 GB
  Bandbreite: 448 GB/s
  Unified Memory Architecture

Storage:
  Typ: NVMe SSD
  Kapazitaet: 2 TB
  Geschwindigkeit: ~5,500 MB/s

Ausgabe:
  HDMI 2.1 (48 Gbps) - SOFTWARESEITIG AUF 120Hz LIMITIERT!
  
  Fuer 8K waere DisplayPort 2.1 (80 Gbps) noetig,
  aber Sony hat sich fuer HDMI 2.1 entschieden.
```

## BOM-Detail (TechInsights 2024)

```
Komponente                    | Kosten    | Anteil
──────────────────────────────|───────────|───────
SoC (Zen 5 + RDNA 4)         | $80-110   | 22%
DRAM (16GB GDDR6)             | $40-60    | 13%
SSD (2TB NVMe)                | $80-120   | 24%
Blu-Ray Drive                 | $15-20    | 4%
WiFi 6E + Bluetooth           | $7-11     | 2%
USB Controller                | $3-5      | 1%
Power Supply                  | $15-20    | 4%
Gehaeuse + Cooling            | $20-30    | 6%
Controller (DualSense)        | $15-20    | 4%
PCB + Montage                 | $10-15    | 3%
Verpackung + Zubehoer         | $8-12     | 2%
──────────────────────────────|───────────|───────
GESAMT BOM                    | $420-460  | 100%
Verkaufspreis                 | $699      |
Aufschlag                     | $239-279  | 55-64%
```

## Kostenstruktur

```
Sony PS5 Pro Kosten pro Einheit:
================================

Materialkosten (BOM):         $420-460
Fertigung (TSMC, Montage):    $30-50
Logistik + Versand:           $15-25
Garantie-Ruecklagen:          $10-20
Marketing:                    $20-30
─────────────────────────────────────
Totale Kosten:                $515-595
Verkaufspreis:                $699
Rohgewinn:                    $104-184
OpEx (Entwicklung, etc.):     ~$50-100 pro Einheit
Netto-Gewinn:                 $4-134 pro Einheit

ANMERKUNG: Sony verkauft die Konsole fast auf Kost.
Der echte Gewinn kommt durch:
  - PS Plus Abonnements ($60/Jahr)
  - Spielverkauf (30% Kommission)
  - Accessories (Controller, etc.)
```

## Display-Limitierung

```
PS5 Pro Display-Ausgabe:
=========================

HDMI 2.1 Port:
  Bandbreite: 48 Gbps
  Max 4K: 120Hz
  Max 8K: 30Hz (nur 4:2:0)
  
  Fuer 4K@144Hz: NICHT MOEGLICH
  Fuer 8K@60Hz:  NICHT MOEGLICH (nur 4:2:0)
  
  Software-Lock: Alle Spiele auf max 120fps limitiert
  Selbst bei 1080p: max 120fps

Waere DisplayPort 2.1 verfuegbar:
  Bandbreite: 80 Gbps
  Max 4K: 240Hz+
  Max 8K: 60Hz (4:4:4)
  
  Fuer VR brillen: Besser (mehr Bandbreite)
  Fuer Monitore: 240Hz+ moeglich
  
SONY HAT DISPLAYPORT 2.1 NICHT VERWENDET.
Grund: HDMI ist im TV-Bereich dominierend.
Das ist eine KUENSTLICHE Limitierung.
```

## GTA6 Europa Alternative

```
SBC-Konsole mit DisplayPort 2.1:
=================================

SoC: Tenstorrent Blackhole p150a ($1,399) ODER
     AMD RDNA3 APU (Custom, ~$100-200 OEM)
     
RAM: DDR5 SO-DIMM (nutzt GTA6 Europa Blueprints)
     32GB @ 5600MT/s
     
Storage: M.2 NVMe (2TB, ~$80-120)
     
Ausgabe: DisplayPort 2.1 (80 Gbps)
         - 4K@240Hz
         - 8K@60Hz (4:4:4)
         - Kein Software-Lock
         
Kuehlung: Quick-Connect Wasserkuehlung
          (kein Loeten, Plug & Play)
          
Gehaeuse: 3D-druckbar (FreeCAD parametrisch)

Kosten:
  SoC:                $100-200
  RAM (32GB DDR5):    $15-25
  SSD (2TB NVMe):     $80-120
  PCB + Rest:         $50-100
  Kuehlung:           $50-80
  Gehaeuse:           $20-30
  ──────────────────────
  Gesamt:             $315-555
  
  Verkaufspreis:      $500-800
  Ersparnis vs PS5:   $200-400 (30-55%)
  
  PLUS: Kein Vendor-Lock, Open-Source,
        Upgradebar, DisplayPort 2.1
```
