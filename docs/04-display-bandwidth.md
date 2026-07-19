# Display-Ausgabe: 4K/8K ohne Ruckler

## Das Problem

Konsolen und viele GPUs ruckeln bei hohen Aufloesungen weil:
1. HDMI 2.1 nur 48 Gbps kann (nicht genug fuer 4K@144Hz)
2. Software-Limits (PS5: max 120fps, auch bei 1080p)
3. GDDR6 Bandbreite zu gering fuer hohe VRAM-Anforderungen
4. Kein DisplayPort 2.1 = kein 8K@60Hz Full RGB

## Loesung: DisplayPort 2.1 + DDR5

```
AUFLUESE / REFRESH-RATE / BENOETIGTE BANDBREITE:
=================================================

4K (3840x2160):
  @60Hz  4:4:4 10-bit:   36 Gbps  (HDMI 2.1 reicht)
  @120Hz 4:4:4 10-bit:   72 Gbps  (HDMI 2.1 NICHT reicht!)
  @144Hz 4:4:4 10-bit:   86 Gbps  (nur DP 2.1)
  @240Hz 4:4:4 10-bit:  144 Gbps  (DP 2.1 mit DSC)
  
8K (7680x4320):
  @30Hz  4:4:4 10-bit:   72 Gbps  (HDMI 2.1 NICHT reicht!)
  @60Hz  4:4:4 10-bit:  144 Gbps  (DP 2.1 mit DSC)
  @60Hz  4:2:0 10-bit:   48 Gbps  (HDMI 2.1, aber 4:2:0!)
  @120Hz 4:4:4 10-bit:  288 Gbps  (DP 2.1 mit DSC, zukunftssicher)

BANDBREITE VERGLEICH:
  HDMI 2.1:    48 Gbps  → 4K@120Hz max (4:2:0)
  DP 2.1:      80 Gbps  → 4K@144Hz (4:4:4)
  DP 2.1 UHBR20: 80 Gbps → 8K@60Hz mit DSC
```

## GTA6 Europa Display-Ausgabe

```
SPECIFICATIONS:
  2x DisplayPort 2.1 (UHBR20, 80 Gbps)
  1x HDMI 2.1 (48 Gbps, fuer TV-Kompatibilitaet)
  1x USB-C (DP Alt Mode, 40 Gbps)

SUPPORTED MODES:
  4K@60Hz    4:4:4 10-bit   ← Native, kein DSC
  4K@120Hz   4:4:4 10-bit   ← Native, kein DSC
  4K@144Hz   4:4:4 10-bit   ← Native, kein DSC
  4K@240Hz   4:4:4 10-bit   ← Mit DSC
  8K@60Hz    4:4:4 10-bit   ← Mit DSC
  8K@30Hz    4:4:4 10-bit   ← Native, kein DSC
  
  50Hz/100Hz PAL: Komplett unterstuetzt
  - 1080i@50Hz (PAL Interlaced)
  - 1080p@50Hz
  - 1080p@100Hz
  - 2160p@50Hz (4K PAL)
  - 2160p@100Hz (4K PAL @100Hz)
  - 720p@50Hz (PAL Standard)
```

## 50Hz/100Hz PAL-Kompatibilitaet

```
WARUM PAL (50Hz/100Hz)?
========================

Europa nutzt seit den 1960ern 50Hz Netzspannung.
Daraus entwickelte sich das PAL-Videoformat:
  - 50 Halbbilder/Sekunde (25 Volllbilder)
  - 100Hz (Deinterlacing fuer flüssige Bilddarstellung)
  
Moderne TVs und Monitore in Europa:
  - Alle nativ 50Hz+100Hz unterstuetzen
  - Viele TVs haben "PAL-Modus" fuer Originalcontent
  - CRT-Retro-Gaming braucht 50Hz
  
PS5 PROBLEM:
  - Kein 50Hz-Modus bei Spielen
  - Nur 60Hz/120Hz
  - PAL-Retro-Spiele laufen falsch (30fps statt 25fps)
  - European Speedrun-Communities nutzen 50Hz

GTA6 EUROPA LOESUNG:
  - Native 50Hz/100Hz Output
  - Fuer alle Aufloesungen: 720p, 1080p, 1440p, 4K
  - VRR (Variable Refresh Rate) fuer 50-240Hz
  - FreeSync / G-Sync kompatibel
  - Kein Frame-Pacing-Problem
```

## 4K Performance Target

```
MINIMUM SPECS fuer 4K@120Hz (GTA6 Europa):
============================================

RAM:    32GB DDR5-5600 (44.8 GB/s)
        ODER 64GB DDR5-6400 (51.2 GB/s)

GPU:    Tenstorrent Blackhole p150a
        ODER AMD RDNA3 APU (12+ CUs)

VRAM:   16GB+ GDDR6/6X
        ODER 32GB Unified (DDR5 Shared)

STORAGE: NVMe SSD (5,000+ MB/s)
         Fuer Texture-Streaming noetig

DISPLAY: DisplayPort 2.1 (80 Gbps)
         Kein HDMI-Limit

THERMAL: Wasserkuehlung
         CPU/GPU max 75°C unter Last
```

## 8K Performance Target

```
MINIMUM SPECS fuer 8K@60Hz (GTA6 Europa):
============================================

RAM:    64GB DDR5-6400 (51.2 GB/s)

GPU:    Tenstorrent Blackhole p150a (2x)
        ODER zukuenftiges Flagship

VRAM:   32GB+ GDDR6X/HBM

STORAGE: NVMe Gen5 (10,000+ MB/s)

DISPLAY: DisplayPort 2.1 UHBR20
         Mit DSC (Display Stream Compression)

THERMAL: Dual-Radiator Wasserkuehlung
         CPU/GPU max 80°C unter Last
```

## Fazit

```
OHNE GTA6 Europa:
  4K@120Hz:   Nur mit HDMI 2.1 (4:2:0, komprimiert)
  4K@144Hz:   Nicht moeglich
  8K@60Hz:    Nur 4:2:0 (Farbinformation halbiert)
  8K@120Hz:   Nicht moeglich
  50Hz/100Hz: Nicht unterstuetzt

MIT GTA6 Europa:
  4K@120Hz:   Native, 4:4:4, 10-bit (DP 2.1)
  4K@144Hz:   Native, 4:4:4, 10-bit (DP 2.1)
  4K@240Hz:   Mit DSC, 4:4:4 (DP 2.1)
  8K@60Hz:    Mit DSC, 4:4:4 (DP 2.1)
  8K@120Hz:   Mit DSC (DP 2.1 UHBR20)
  50Hz/100Hz: Komplett nativ unterstuetzt
```
