# GTA6 Europa

**Offene Blaupausen für Gaming-Hardware zum Herstellkostenpreis.**

## Mission

Hardware-Preise durch offene Transparenz drücken. Wir veröffentlichen vollständige
Blaupausen (KiCad, FreeCAD, STL) für DDR5-Module, GPU-Boards und Einplatinenrechner
– mit dokumentierten Lieferanten, BOMs und Fertigungsanleitungen.

Das Ergebnis: Jeder kann Gaming-Hardware zum Herstellkostenproduzieren oder kaufen.

## Was hier drin ist

```
blueprints/     Fertige Blaupausen (KiCad PCB + BOM + Fertigungsanleitung)
  ddr5-32gb-sodimm/   DDR5 SO-DIMM 32GB Modul
  gpu-board/          GPU-Board mit DisplayPort 2.1
  sbc-console/        Einplatinen-Rechner (PS6-Alternative)
  cable-spec/         Abgeschirmte Kabel-Spezifikation
  cooling-system/     Wasserkühlung (Quick-Connect)

docs/           Dokumentation, Kostenanalysen, Marktanalyse
suppliers/      Lieferanten-Datenbank mit Filtern
tools/          Preis-Rechner, Lieferanten-Filter
cad-models/     3D-Modelle (Blender, FreeCAD, STL)
```

## Preisvergleich

| Produkt | Marktpreis | GTA6 Europa | Ersparnis |
|---|---|---|---|
| DDR5 32GB 5600 SO-DIMM | $350-432 | ~$18-30 | 92-95% |
| GPU Board (32GB VRAM) | $1,500-2,000 | ~$300-400 | 75-80% |
| SBC Console (DP 2.1) | $600-700 (PS6) | ~$350-500 | 30-45% |
| Wasserkühlung (Custom) | $300-500 | ~$50-80 | 75-85% |
| **Komplettes Kit** | **$2,500-3,500** | **~$700-1,000** | **65-75%** |

## Montage-Prinzip: "LEGO für Hardware"

```
Level 0: Fertige Module kaufen    → Kein Werkzeug nötig
Level 1: Module einstecken         → Kein Werkzeug (Click & Go)
Level 2: Kühlblock schrauben      → 1 Schraubenzieher (mitgeliefert)
Level 3: Kabel crimpen            → Zange + Crimp-Tool
Level 4: PCB selbst bestücken     → Lötkolben (nur für Entwickler)
```

## Lieferanten-Filter

Waehle deine Lieferkette:

```bash
# Nur europaeische Hersteller
python3 tools/supplier-filter.py --origin europe

# Nur Open-Source Hardware
python3 tools/supplier-filter.py --license oss

# Asien, bester Preis
python3 tools/supplier-filter.py --origin asia --quality consumer

# EU + Industrie-Qualitaet
python3 tools/supplier-filter.py --origin europe --quality industrial
```

## Preisrechner

```bash
# Berechne Gewinn fuer DDR5-Module
python3 tools/price-calculator.py --product ddr5-32gb --supplier ddr5-shenzhen-kedun --qty 100

# BOM-Kosten berechnen
python3 tools/price-calculator.py --bom blueprints/gpu-board/bom-gpu.csv --qty 50
```

## Technologie

- **Anzeige:** DisplayPort 2.1 (80 Gbps) statt HDMI 2.1 (48 Gbps)
- **RAM:** DDR5 SO-DIMM mit custom XMP/EXPO Profil
- **Kühlung:** Quick-Connect Wasserkühlung (kein Loeten, kein Schlauch schneiden)
- **Gehaeuse:** Parametrisch (FreeCAD), 3D-druckbar
- **SoC:** RISC-V (Tenstorrent Blackhole) oder AMD RDNA3 APU

## Lizenz

CERN Open Hardware Licence Version 2 - Strongly Reciprocal (CERN-OHL-S-2.0)

Siehe [LICENSE](LICENSE) fuer Details.

## GitHub

https://github.com/svenp/gta6-europa (in Vorbereitung)
