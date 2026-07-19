# Lieferanten-Filter Konfiguration

## Filter-Profile

```bash
# NUR EUROPÄISCH
python3 tools/supplier-filter.py --region europe
# Eurocircuits (NL), Alphacool (DE), Aquacomputer (DE)

# NUR ASIEN (bester Preis)
python3 tools/supplier-filter.py --region asia
# JLCPCB, PCBWay, Digiera, NexaRAM, Bykski, etc.

# NUR OPEN-SOURCE
python3 tools/supplier-filter.py --type pcb-fab
# Alle PCB-Fabs (viele sind OSHW-friendly)

# NUR INDUSTRIE
python3 tools/supplier-filter.py --quality industrial
# Innodisk, Apacer, Eurocircuits

# EU + INDUSTRIE
python3 tools/supplier-filter.py --region europe --quality industrial

# ASIEN + OEM
python3 tools/supplier-filter.py --region asia --type oem

# KEIN CHINA
python3 tools/supplier-filter.py --no-china

# DDR5 LIEFERANTEN
python3 tools/supplier-filter.py --product ddr5-sodimm

# PCB FERTIGUNG
python3 tools/supplier-filter.py --product pcb

# WASSERKUEHLUNG
python3 tools/supplier-filter.py --product waterblock-gpu
```

## Filter-Definitionen

```
--region:    europa, asia, global, us
--country:   CN, TW, US, DE, NL, KR
--type:      oem, odm, brand, pcb-fab, cooling, cable, industrial
--product:   ddr5-sodimm, pcb, pcba, waterblock-gpu, dp-cable, etc.
--quality:   consumer, industrial, automotive
--no-china:  Schliesst alle chinesischen Lieferanten aus
--verbose:   Zeigt Preise im Detail
```
