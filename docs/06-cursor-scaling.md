# Maus-Cursor bei 4K/8K: Keine Pixel mehr

## Das Problem

Bei 4K/8K Displays wird der Mauszeiger als Bitmap (16x16 oder 32x32 Pixel)
gerendert und dann hochskaliert. Ergebnis: 4-8 grobe Pixel sichtbar.

```
1080p:   Cursor 32x32px = 1:1 Pixel = scharf
4K:      Cursor 32x32px = 1:4 Pixel = 4x grob
8K:      Cursor 32x32px = 1:8 Pixel = 8x grob

Typisches Symptom:
  - Mauszeiger sieht "treppenartig" aus
  - 4-8 grobe Pixel sichtbar
  - Auf dem Fernseher noechtiger als am Monitor
```

## Ursachen

```
1. BITMAP CURSOR (传统):
   - OS rendert 32x32 Bitmap
   - GPU skaliert auf Display-Aufloesung
   - Display zeigt grobe Pixel
   - KEIN antialiasing

2. DPI-SCALING PROBLEM:
   - Windows: 100%/125%/150%/200% Skalierung
   - Linux: X11/Wayland unterschiedlich
   - Bei 200%: 2x2 Pixel pro Cursor-Pixel
   - Bei 300%: 3x3 Pixel pro Cursor-Pixel
   - Ergebnis: immer noch Pixel sichtbar

3. TV vs MONITOR:
   - TVs haben anderen Pixel-Pitch als Monitore
   - 55" 4K TV: 0.315mm Pixel-Pitch
   - 27" 4K Monitor: 0.155mm Pixel-Pitch
   - Cursor auf TV wirkt 2x grober

4. GPU TREIBER:
   - Manche Treiber rendern Cursor auf CPU
   - Dann: GPU skaliert (schlecht)
   - Besser: GPU rendert native (gut)
```

## Loesung: GTA6 Europa Cursor-System

```
SPECIFICATION:
==============

1. VECTOR CURSOR (SVG/Path):
   - Mauszeiger als Vektorgrafik definiert
   - GPU rendert nativ in Display-Aufloesung
   - Skaliert verlustfrei von 1x bis 8x
   - Kein Pixel-Raster, immer scharf

2. GPU-NATIVE RENDERING:
   - Cursor wird auf GPU gerendert (nicht CPU)
   - Direkt in Display-Aufloesung (4K/8K)
   - Antialiased (glatter Rand)
   - Farbabweichung: Subpixel-Rendering

3. DPI-AWARE DESIGN:
   - Cursor-Groesse passt sich an DPI an
   - 96 DPI:  32px (Normal)
   - 192 DPI: 64px (4K)
   - 384 DPI: 128px (8K)
   - Immer scharf durch Vektor-Rendering

4. MULTIPLE CURSOR STYLES:
   - Standard:  Pfeil (45 Grad)
   - Text:      I-Beam
   - Link:      Hand
   - Move:      4-Richtungs-Pfeil
   - Wait:      Kreis/Rad
   - Resize:    Doppelpfeil
   Alle als Vektoren, alle nativ gerendert

5. LOW-LATENCY CURSOR:
   - Cursor-Update mit 1000Hz (vs 125-250Hz heute)
   - Input-Lag: <1ms (vs 4-8ms heute)
   - Fuer Gaming + VR kritisch
```

## Technische Implementierung

```
HARDWARE-SEITE:
  - GPU mit nativer Cursor-Unterstuetzung
  - DisplayPort 2.1 EDID fuer korrekten Pixel-Pitch
  - 1000Hz USB Polling Rate (Maus)
  
SOFTWARE-SEITE:
  - Linux: Wayland + wlroots Cursor-Protocol
  - Windows: DirectCursor API
  - FreeType + HarfBuzz fuer Vektor-Cursor
  - OpenGL/Vulkan Shader fuer Rendering
  
DISPLAY-SEITE:
  - EDID korrekt auslesen (Pixel-Pitch, DPI)
  - Kein Upscaling des Cursors
  - Native Auflösung verwenden
  - CEA-861 fuer TV-Kompatibilitaet
```

## Vergleich

```
HEUTE (Bitmap Cursor):
  1080p:  Scharf (1:1)
  4K:     Grob (4 Pixel breit)
  8K:     Sehr grob (8 Pixel breit)
  
GTA6 EUROPA (Vector Cursor):
  1080p:  Scharf (Vektor, antialiased)
  4K:     Scharf (Vektor, nativ)
  8K:     Scharf (Vektor, nativ)
  IMMER:  Glatter Rand, keine Pixel
```

## Fuer TVs

```
TV-SPECIFISCHE PROBLEME:
  - TVs haben anderen Gamma-Kurve als Monitore
  - Manche TVs skalieren alles auf ihre native Aufloesung
  - Cursor wird doppelt skaliert (OS + TV)
  
GTA6 EUROPA LOESUNG:
  - DisplayPort 2.1 mit korrektem EDID
  - TV erkennt native Aufloesung
  - Kein Upscaling noetig
  - Cursor wird nativ gerendert
  - Ergebnis: 0 Pixel sichtbar
```
