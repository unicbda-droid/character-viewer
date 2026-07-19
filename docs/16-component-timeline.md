# Wie lange dauert es bis alles da ist?

## Kurzantwort

```
BESTELL HEUTE:       7-14 Tage bis alles da ist
SCHNELLSTE MOGLICH:  5-7 Tage (Express alles)
WAS AM LAENGSTEN DAUERT: PCB (3-7 Tage Bauzeit)
```

---

## Komplette Bestellliste mit Lieferzeiten

### DRAM-Chips (Samsung)

```
TEILNUMMER:        K4RAH086VB-BCQK (16Gb x8, DDR5-4800)
                   K4RAH086VB-BCWM (16Gb x8, DDR5-5600)
BENOTIGT:          8 Stueck pro 16GB Modul
PREIS:             ~$7-8/Stueck = $56-64 fuer 8 Stueck

WHER BESTELLEN:
  MemorySolution.de (DE):  1-2 Tage, ~€8/Stueck
  DigiKey (US -> DE):      48 Stunden, ~$7-8/Stueck
  Mouser (US -> DE):       2-3 Tage, ~$7-8/Stueck
  LCSC (China):            5-10 Tage, ~$7/Stueck (OFT OUT OF STOCK!)
  FusionWW (Singapore):    7-14 Tage, MOQ 1,280!
```

### PMIC (Power Management)

```
TEILNUMMER:        ISL95338IRTZ (Renesas) - BESTER LAGERSTATUS
                   P8911-Y0Z001FNG (Renesas) - OFT AUSVERKAUFT
BENOTIGT:          1 pro Modul
PREIS:             ~$2-6/Stueck

WHER:
  DigiKey:         48 Stunden, ISL95338 9M+ Stueck auf Lager!
  Mouser:          2-3 Tage
  Avnet:           3-5 Tage
```

### SPD Hub

```
TEILNUMMER:        SPD5118-Y1B000NCG8 (Renesas)
BENOTIGT:          1 pro Modul
PREIS:             ~$1.50-3/Stueck

WHER:
  Avnet:           3-5 Tage, 82,850 Stueck auf Lager!
  Unikeyic:        5-10 Tage, Montage Alternative
```

### Passive Bauteile (Kondensatoren, Widerstaende)

```
BENOTIGT:          ~30-50 Stueck pro Modul (0201/0402)
PREIS:             ~$1-3 pro Modul

WHER:
  DigiKey:         48 Stunden
  Mouser:          2-3 Tage
  LCSC:            5-10 Tage
```

### SO-DIMM Connector (262-Pin)

```
TEILNUMMER:        90415-4015SR (UMAX)
BENOTIGT:          1 pro Modul
PREIS:             ~$1-2

WHER:
  Mouser:          2-3 Tage
  DigiKey:         48 Stunden
```

### Blank PCB (DDR5 SO-DIMM)

```
WHER:
  JLCPCB (China): 3-7 Tage Bauzeit + 7-14 Tage Versand = 10-21 Tage
  PCBWay (China): 3-7 Tage + 7-14 Tage = 10-21 Tage
  Eurocircuits:   2-5 Tage + 2-5 Tage Versand = 4-10 Tage
  Aisler (DE):    2-5 Tage = 2-5 Tage
  Beta Layout:    1-7 Tage = 1-7 Tage

PREIS: $2-10 (China) oder €12-30 (EU)
```

### Stencil (Loetpaste-Schablone)

```
WHER:
  CK Ottobrunn (DE):  1-2 Tage, €30-60
  Becktronic (DE):     1-2 Tage, €25-50
  Beta Layout (DE):    1 Tag, €20-40
  JLCPCB (China):      +3-7 Tage

EMPFEHLUNG: Becktronic oder CK - am naechsten, schnellsten
```

### SPD Programmer

```
PRODUKT:           ElmorLabs SPD5-PROG-X1 ($150)
                   Unified DDR Flasher ($150, aus Ungarn)
WHER:
  ElmorLabs:       3-7 Tage
  UDF (Ungarn):    3-5 Tage mit DHL
```

---

## Timeline

```
TAG 1:   ALLES BESTELLEN
         - DRAM Chips (DigiKey/MemorySolution)
         - PMIC (DigiKey)
         - SPD Hub (Avnet)
         - Passives (DigiKey)
         - Connector (Mouser)
         - PCB (JLCPCB oder Aisler)
         - Stencil (Becktronic)
         - SPD Programmer (ElmorLabs)

TAG 2-3: ERSTE PAKETE DA
         - DRAM Chips (48h DHL)
         - PMIC (48h)
         - Passives (48h)
         - Connector (48h)

TAG 3-5: ZWEITE WELLE
         - SPD Hub (Avnet, 3-5 Tage)
         - SPD Programmer (3-5 Tage)
         - Stencil (1-2 Tage DE)

TAG 7-14: LETZTE TEILE
         - PCB (JLCPCB: 10-21 Tage)
         - ODER PCB (Aisler: 2-5 Tage)
```

---

## Gesamtkosten (1 Modul)

```
KOMPONENTE              PREIS
──────────────────────────────
8x Samsung DDR5 Chip     $56-64
1x PMIC                  $2-6
1x SPD Hub               $1.50-3
Passives                 $1-3
Connector                $1-2
PCB                      $2-10
Stencil                  $5-10 (anteilig)
──────────────────────────────
GESAMT                   $68-98

SPD Programmer:          $150 (einmalig, fuer alle Module)
```

---

## Empfehlung

```
SCHNELLSTES VORGEHEN:
  1. Heute: Alles bei DigiKey/Mouser/Avnet bestellen (48h)
  2. Heute: PCB bei Aisler (DE) bestellen (2-5 Tage)
  3. Heute: Stencil bei Becktronic bestellen (1-2 Tage)
  4. Heute: SPD Programmer bei ElmorLabs bestellen (3-5 Tage)
  
  => 5-7 TAGE bis alles da ist

GUENSTIGSTES VORGEHEN:
  1. DRAM Chips bei MemorySolution.de (DE)
  2. PCB bei JLCPCB (China, guenstig)
  3. Alles andere bei DigiKey
  
  => 10-14 TAGE, aber $10-20 guenstiger
```
