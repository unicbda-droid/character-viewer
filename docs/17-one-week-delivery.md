# Alles in 1 Woche - Lieferplan

## Ziel

```
Bestelltag:   Tag 1 (heute)
Lieferung:    Tag 7 (maximal)
Strategie:    NUR deutsche/europaeische Lieferanten + Express
```

---

## Bestellplan Tag 1

### 1. Samsung DRAM Chips

```
LIEFERANT:    MemorySolution.de (Deutschland!)
TEILNUMMER:   K4RAH086VB-BCQK (16Gb x8, DDR5-4800)
              ODER
              K4RAH086VB-BCWM (16Gb x8, DDR5-5600)
MENGE:        8 Stueck (fuer 16GB Modul)
PREIS:        ~€8/Stueck = €64
LIEFERZEIT:   1-2 Tage DHL
TELEFON:      +49(0)7667 9469 0
KONTAKT:      Telefon oder E-Mail

ALTERNATIVE:
  CompuStocx (DE):  compustocx.de
  Avnet (EU-Lager):  avnet.com
```

### 2. PMIC

```
LIEFERANT:    DigiKey Europe (Filiale Niederlande!)
TEILNUMMER:   ISL95338IRTZ (Renesas)
MENGE:        1 Stueck
PREIS:        ~€6
LIEFERZEIT:   2-3 Tage DHL (EU-Versand)
URL:          digikey.de

ACHTUNG: P8911 ist oft ausverkauft -> ISL95338 nehmen!
```

### 3. SPD Hub

```
LIEFERANT:    Avnet Europe (EU-Lager)
TEILNUMMER:   SPD5118-Y1B000NCG8 (Renesas)
MENGE:        1 Stueck
PREIS:        ~€2-3
LIEFERZEIT:   2-4 Tage
URL:          avnet.com

ALTERNATIVE:
  Montage M88SPD5118 bei Unikeyic (China, 5-10 Tage - ZU LANGSAM)
```

### 4. Passive Bauteile + Connector

```
LIEFERANT:    Mouser Electronics (EU-Lager, Czech Republic)
TEILNUMMERN:  Kondensatoren 100nF 0402, 10µF 0402
              Widerstaende 10K 0402, diverses
              DDR5 SO-DIMM Connector 262-Pin
MENGE:        ~50 Stueck passives + 1 Connector
PREIS:        ~€5-10 gesamt
LIEFERZEIT:   2-3 Tage DHL
URL:          mouser.de

ALTERNATIVE:
  DigiKey Europe (2-3 Tage)
  Reichelt (DE, 1-2 Tage): reichelt.de
```

### 5. Blank PCB (DDR5 SO-DIMM)

```
OPTION A (SCHNELLSTER):
  LIEFERANT:    Aisler (Deutschland!)
  LIEFERZEIT:   2-5 Werktage
  PREIS:        ~€12-20 fuer 3 Stueck
  URL:          aisler.de
  KEEPER:       Ja, KiCad-kompatibel

OPTION B (EXPRESS):
  LIEFERANT:    Beta Layout (Deutschland!)
  LIEFERZEIT:   1-3 Werktage (Express)
  PREIS:        ~€20-40
  URL:          bfr-layout.de

OPTION C (GUENSTIGSTE):
  LIEFERANT:    Eurocircuits (Niederlande)
  LIEFERZEIT:   3-5 Werktage
  PREIS:        ~€30-60
  URL:          eurocircuits.de

EMPFEHLUNG: Aisler oder Beta Layout - beides in DE!
```

### 6. Stencil (Loetpaste-Schablone)

```
LIEFERANT:    Becktronic (Deutschland!)
LIEFERZEIT:   1-2 Werktage
PREIS:        ~€25-50
URL:          becktronic.de
BEMERKUNG:    BGA-faehig, fuer FBGA-82 DDR5 Chips

ALTERNATIVE:
  Christian Koenen (Ottobrunn bei Muenchen): koenig-metall.de
  Lieferzeit: 1-2 Tage
  Preis: €30-60
```

### 7. SPD Programmer

```
LIEFERANT:    ElmorLabs
PRODUKT:      SPD5-PROG-X1
PREIS:        $150 (~€140)
LIEFERZEIT:   3-5 Tage (DHL aus EU)
URL:          elmorlabs.com

ALTERNATIVE:
  Unified DDR Flasher (Ungarn)
  Preis: $150 + $25 Extension
  LIEFERZEIT: 3-5 Tage DHL
```

### 8. Loetpaste + Flux + Zubehoer

```
LIEFERANT:    Reichelt (Deutschland!)
LOETPASTE:    ChipQuik SMD291 (lead-free), ~€15
FLUX:         Amtech NC-559-V2-TF, ~€10
TWEEZERS:     ESD-safe Set, ~€10
IPA:          99%, ~€5
KIMWIPES:     ~€5
ESD MAT:      ~€15
LIEFERZEIT:   1-2 Tage (DHL)
URL:          reichelt.de
```

---

## Lieferplan-Uebersicht

```
TAG 1 (HEUTE):     ALLES BESTELLEN
                    └─ MemorySolution.de (DRAM)
                    └─ DigiKey Europe (PMIC)
                    └─ Avnet Europe (SPD Hub)
                    └─ Mouser Europe (Passives + Connector)
                    └─ Aisler (PCB)
                    └─ Becktronic (Stencil)
                    └─ ElmorLabs (SPD Programmer)
                    └─ Reichelt (Loetpaste + Zubehoer)

TAG 2-3:           ERSTE PAKETE
                    └─ Reichelt (Loetpaste) = 1-2 Tage
                    └─ Becktronic (Stencil) = 1-2 Tage
                    └─ MemorySolution (DRAM) = 1-2 Tage

TAG 3-4:           ZWEITE WELLE
                    └─ DigiKey (PMIC) = 2-3 Tage
                    └─ Mouser (Passives) = 2-3 Tage
                    └─ Avnet (SPD Hub) = 2-4 Tage

TAG 5-7:           LETZTE TEILE
                    └─ Aisler (PCB) = 2-5 Tage
                    └─ ElmorLabs (Programmer) = 3-5 Tage

TAG 7:             ALLES DA! ✅
```

---

## Gesamtkosten

```
KOMPONENTE              PREIS
──────────────────────────────
8x Samsung DDR5 Chip     €64
1x PMIC ISL95338         €6
1x SPD Hub SPD5118       €3
Passives (~50 Stk)       €5
SO-DIMM Connector        €2
PCB (3 Stueck Aisler)    €15
Stencil (Becktronic)     €35
SPD Programmer            €140
Loetpaste + Flux         €25
Zubehoer (Tweezers etc)  €30
──────────────────────────────
GESAMT                   €325

PRO MODUL (bei 3 PCBs):  ~€108/Modul
(Plus DRAM: €64 + Rest €44 = €108)
```

---

## Schnellste Option: ALLES aus Deutschland

```
WENN NUR DEUTSCHE LIEFERANTEN:
  MemorySolution.de (DRAM):     1-2 Tage
  Reichelt.de (Passives):       1-2 Tage
  Aisler.de (PCB):              2-5 Tage
  Becktronic.de (Stencil):      1-2 Tage
  
  => MAXIMUM 5 TAGE!
```

---

## Wichtigste Erkenntnis

```
DEUTSCHLAND HAT ALLES:
  - Samsung DRAM Chips: MemorySolution.de
  - PCBs: Aisler, Beta Layout, Wuerth
  - Stencils: Becktronic, CK
  - Passives: Reichelt, Mouser DE, DigiKey DE
  - Tools: Reichelt, Amazon.de
  
  KEIN GRUND auf China zu warten!
  Alles in 5-7 Tagen moeglich.
```
