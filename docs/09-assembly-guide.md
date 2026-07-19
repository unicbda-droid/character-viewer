# Assembly Guide - LEGO-Prinzip

## Uebersicht

GTA6 Europa Hardware ist modular aufgebaut. Jedes Teil passt in einen Slot
oder wird per Quick-Connect verbunden. Kein Loeten, kein Schlauch schneiden.

## Level 0: Fertige Module kaufen

```
Was:    Fertige DDR5-Module, GPU-Board, Kuehlung kaufen
Werkzeug: Keins
Zeit:    0 Minuten (nur Auspacken)

Beispiel:
  - DDR5 32GB SO-DIMM bei Digiera/NexaRAM bestellen
  - Fertig bestueckt, getestet, verpackt
  - Preis: $12-18 pro Modul
```

## Level 1: Module einstecken (Click & Go)

```
Werkzeug: Keins
Zeit:     15 Minuten

Schritte:
  1. DDR5 SO-DIMM: 30° Winkel einfuehren, hasta clippen
  2. NVMe SSD: M.2 Slot, Schraube (mitgeliefert)
  3. GPU Board: PCIe x16 Slot, clippen
  4. Quick-Connect Wasserkuehlung: 3x anklicken
  5. Fertig!

Beispiel-Setup:
  ┌──────────────────────────────────────┐
  │ Mainboard                            │
  │                                      │
  │  [DDR5] ← einrasten                 │
  │  [DDR5] ← einrasten                 │
  │                                      │
  │  [GPU Board] ← einrasten            │
  │                                      │
  │  [NVMe SSD] ← aufschrauben          │
  │                                      │
  │  [Wasserkuehlung] ← anklicken       │
  │    Waterblock → Pumpe → Radiator    │
  └──────────────────────────────────────┘
```

## Level 2: Schraubenzieher (mitgeliefert)

```
Werkzeug: 1x Phillips Schraubenzieher (im Kit enthalten)
Zeit:     30 Minuten

Zusaetzliche Schritte:
  1. Waterblock auf CPU/SoC schrauben (4 Schrauben)
  2. Radiator am Gehaeuse befestigen (4 Schrauben)
  3. Gehaeuse zusammenschrauben (6 Schrauben)
  4. Luefter anschliessen (Stecker)
```

## Level 3: Kabel crimpen (optional)

```
Werkzeug: Crimp-Zange ($15 bei Amazon)
Zeit:     45 Minuten

Nur noetig fuer:
  - Custom-Length PCIe Kabel
  - Custom-Length EPS Kabel
  - Kabel-Management

Standard-Kabel:
  - 24-Pin ATX: Fertig, included
  - 8-Pin PCIe: Fertig, included
  - SATA Power: Fertig, included
```

## Level 4: PCB selbst bestuecken (nur Entwickler)

```
Werkzeug: Loetkolben, Heissluftstation, Mikroskop
Zeit:     2-4 Stunden

Nur noetig fuer:
  - Prototypen-Testing
  - Custom PCB-Modifikationen
  - Bugfixing

Fuer alle anderen: JLCPCB SMT Assembly nutzen ($2-5/Board)
```

## Checkliste

```
VOR DEM START:
  [ ] Alle Teile vorhanden (BOM checken)
  [ ] Anti-Statik-Armband tragen
  [ ] Sauberer, trockener Arbeitsplatz
  [ ] Schraubenzieher bereit (Level 2+)

MONTAGE:
  [ ] RAM-Module einstecken
  [ ] NVMe SSD einsetzen
  [ ] GPU Board einsetzen
  [ ] Waterblock montieren (Level 2+)
  [ ] Quick-Connect anschliessen
  [ ] Gehaeuse schliessen (Level 2+)
  [ ] Kabel verlegen

TESTING:
  [ ] Strom anschliessen
  [ ] BIOS/UEFI pruefen
  [ ] RAM-Erkennung verifizieren
  [ ] GPU-Erkennung verifizieren
  [ ] Temperaturtest (Idle + Load)
  [ ] DisplayPort 2.1 testen
  [ ] PAL 50Hz/100Hz testen
  [ ] 4K@120Hz+ testen
  [ ] Wasserkuehlung auf Leckage pruefen

ABSCHLUSS:
  [ ] Alle Schrauben festziehen
  [ ] Kabel-Management
  [ ] Betriebssystem installieren
  [ ] Treiber installieren
  [ ] Benchmark laufen lassen
```
