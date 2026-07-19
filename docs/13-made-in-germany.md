# Was kostet es in Deutschland zu bauen?

## Kurzantwort

```
DRAM-Chips in DE bauen:          NEIN (keine Fabs seit 2009)
Alles andere in DE bauen:        JA
Kosten pro Modul (Made in DE):   €45-95 (vs $15 China)
Aufpreis:                        +200-500%
```

---

## Geht's ueberhaupt?

```
KOMPONENTE         MADE IN DE?     ALTERNATIVE
─────────────────────────────────────────────────
DRAM-Chips         NEIN            Samsung Korea, Hynix Korea, Micron USA
PCB                JA              Wuerth, Schweizer, Multi-CB, LeitOn
SMT Assembly       JA              Zollner, BMK, LeitOn, NOA Labs
Heatspreader       JA              Protolabs CNC, Thermal Grizzly
Kondensatoren      NEIN            Japan (Murata, TDK)
Widerstaende       TEILWEISE       Vishay EU, Japan
PMIC               NEIN            Japan/USA
Testing/QC         JA              Lokal
```

---

## FABs in Deutschland

### DRAM-Chip-Fabs: KEINE MEHR

```
2006: Infineon spaltet Qimonda ab (Dresden)
2009: Qimonda pleite - Deutschland verliert DRAM-Produktion
2024: Infineon zaehlt €800M wegen Qimonda-Schiedsspruch
2026: Infineon eröffnet €5B Smart Power Fab (KEIN DRAM!)
```

**Aktuell:** Kein einziges Unternehmen in Deutschland/Europa
fertigt Standalone-DRAM-Chips.

**Ausnahme:** FMC + Neumonda (ferroelektrische "DRAM+" Technologie)
- Noch in R&D, kein Produktionsbeginn absehbar
- Koennte spaeter die Loesung sein

### Infineon in Dresden - was machen die?

```
INFINEON DRESDEN:
  - Leistungshalbleiter (MOSFETs, IGBTs) fuer E-Autos
  - Analog/Mixed-Signal Chips
  - Automotive Microcontroller
  - KEIN DRAM, KEIN Speicher
  
Smart Power Fab (Juli 2026):
  - Welt groesstes Leistungshalbleiter-Werk
  - €5 Milliarden Investition
  - Fuer E-Auto/Server/Erneuerbare
```

---

## Konkrete Kosten: DDR5 32GB SO-DIMM "Made in Germany"

### BOM (Materialkosten)

```
KOMPONENTE              CHINA      DEUTSCHLAND      GRUND
──────────────────────────────────────────────────────────
4x DRAM-Chips (8GB)     $8         $15-25           Import noetig
PCB 8-Layer             $1.50      €3-8             Wuerth/LeitOn
PMIC + SPD              $1.50      $3-5             Import noetig
Kondensatoren 28x       $0.80      €1.50-3          Import noetig
Widerstaende 15x        $0.30      €0.50-1          Teilweise EU
Heatspreader            $0.50      €5-15            CNC lokalisches Metall
Label + Packung         $0.20      €1-2             Lokal
──────────────────────────────────────────────────────────
BOM GESAMT              $12.80     €30-60           +200-400%
```

### Montage (Assembly)

```
KOSTENPOSTEN           CHINA      DEUTSCHLAND
────────────────────────────────────────────────
Setup/Programmierung   $25        €33-50
SMT Stencil            $7         €27-66
SMT Bestueckung        $2         €3-8
Pruefung/QC            $1         €2-5
Heatspreader Montage   $0.50      €2-3
────────────────────────────────────────────────
GESAMT                 $35.50     €67-132
```

### Gesamtkosten pro Modul

```
                       CHINA (OEM)   DEUTSCHLAND    DIFFERENZ
──────────────────────────────────────────────────────────────
Material               $12.80        €30-60         +200-400%
Montage                $3.50         €6-15          +100-300%
──────────────────────────────────────────────────────────────
PRO MODUL              $15-20        €45-95         +200-500%
```

---

## Preisszenarien

### Szenario 1: 100 Module Made in Germany

```
DRAM-Chips (Korea import):    €1,500
PCB (Wuerth, DE):               €600
Passive Bauteile (import):      €300
SMT Assembly (LeitOn Berlin):   €500
Heatspreader (CNC):             €800
Testing + QC:                   €300
Verpackung:                     €150
───────────────────────────────────────
GESAMT:                       €4,150
PRO MODUL:                     €41.50

Verkaufspreis (eBay DE):       €80-90
GEWINN pro Modul:              €38-48
```

### Szenario 2: 500 Module Made in Germany

```
DRAM-Chips (Korea import):    €6,500
PCB (Wuerth, DE):             €2,000
Passive Bauteile:             €1,200
SMT Assembly (Zollner):       €1,500
Heatspreader (CNC):           €2,500
Testing + QC:                 €1,000
Verpackung:                     €500
───────────────────────────────────────
GESAMT:                      €15,200
PRO MODUL:                     €30.40

Verkaufspreis:                €80-90
GEWINN pro Modul:             €49-59
```

### Szenario 3: 1000 Module Made in Germany

```
DRAM-Chips (Korea import):   €12,000
PCB (Wuerth, DE):             €3,500
Passive Bauteile:             €2,000
SMT Assembly (Zollner):       €2,500
Heatspreader (CNC):           €4,000
Testing + QC:                 €1,800
Verpackung:                     €800
───────────────────────────────────────
GESAMT:                      €26,600
PRO MODUL:                     €26.60

Verkaufspreis:                €80-90
GEWINN pro Modul:             €53-63
```

---

## Vergleich: Made in Germany vs Made in China

```
                     CHINA OEM    DE 100    DE 500    DE 1000
──────────────────────────────────────────────────────────────
Kosten/Modul:        $15-20      €41.50    €30.40    €26.60
Verkauf/Modul:       $80         €85       €85       €85
Gewinn/Modul:        $60-65      €43       €54       €58
Gewinn 100 Stk:      $6,000      €4,300    ---       ---
Gewinn 500 Stk:      $30,000     ---       €27,000   ---
Gewinn 1000 Stk:     $60,000     ---       ---       €58,000
```

---

## Die deutschen Fertiger

### PCB

```
WUERTH ELEKTRONIK (Deutschland)
  - 3 Werke in Deutschland
  - HDI, Rigid-Flex, Embedding
  - Prototypen bis Serie
  - Online-Rechner
  - Preis: 8-Layer ~€5-8 pro Board (100 Stk)
  
SCHWEIZER ELECTRONIC (Deutschland, seit 1849)
  - p2 Pack Embedding
  - Hochfrequenz, Automotive
  - Premium-Qualitaet
  - Preis: auf Anfrage

MULTI-CB (Bayern)
  - 1-48 Layer
  - Pooling-Service
  - Express 1-Tag moeglich
  - Preis: competitive

LEITON (Berlin)
  - Alle PCB-Typen
  - Online-Rechner
  - Express-Prototypen
```

### SMT Assembly

```
ZOLLNER ELEKTRONIK (Zandt, Bayern)
  - Europas groesster EMS
  - 10.000+ Mitarbeiter
  - 24 Standorte
  - Preis: ab €0.004/Lotstelle

BMK GROUP (3 Standorte DE)
  - Automotive, Telecom, Medical
  - Aerospace-zertifiziert
  - Preis: auf Anfrage

LEITON (Berlin)
  - Express-Montage
  - Prototypen bis Serie
  - "Made in Germany"
  - 10-Tage Express moeglich
  - Preis: €150-500/Board (Prototyp)

NOA LABS (Berlin + Shenzhen)
  - Full EMS
  - Startups bis Fortune 500

KRUSE ELECTRONIC (Duesseldorf)
  - Komponentenvertrieb + EMS
  - Seit 1951
```

### Heatspreader

```
PROTOLABS (Deutschland + EU)
  - CNC-fräsung Kupfer/Aluminium
  - Sofort-Online-Kalkulation
  - Kleine Serien moeglich
  - Preis: €20-50/Stk (Kleinstserie), €5-15 (1000+)

THERMAL GRIZZLY (Deutschland)
  - Bekannter Hersteller
  - Nickel-beschichtetes Kupfer
  - Preis: auf Anfrage
```

---

## Warum so teuer?

```
GRUND 1: Keine DRAM-Fabs in DE
  - Chips muessen aus Korea/USA importiert werden
  - Zoll + Versand + Wartezeit
  - Kein Mengenrabatt wie Samsung direkt

GRUND 2: Deutsche Lohnhoehen
  - SMT-Monteure: €25-40/Stunde (vs $3-5 in China)
  - CNC-Fraesen: €30-50/Stunde (vs $5-10 in China)
  - Qualitaetskontrolle: €20-35/Stunde

GRUND 3: Kleinere Mengen
  - Deutsche EMS-Firmen machen eher 100-10.000 Stk
  - China macht 100.000-1.000.000 Stk
  - Weniger Skaleneffekte

GRUND 4: Komplexere Logistik
  - DRAM-Chips aus Korea
  - Passive Bauteile aus Japan
  - Alles in DE zusammenbauen
  - Mehr Transportwege
```

---

## Empfehlung

```
WAS ZU MACHEN:
  1. DRAM-Chips in Korea kaufen (Samsung/Hynix)
  2. PCB bei Wuerth in DE bestellen
  3. SMT Assembly bei LeitOn Berlin oder Zollner
  4. Heatspreader bei Protolabs CNC fräsen
  5. Testing + Verpackung lokal

WAS NICHT ZU MACHEN:
  - DRAM-Chips in DE herstellen (geht nicht)
  - Alles in China kaufen und "Made in Germany" schreiben (Taeuschung)
  - Unter 100 Stk anfangen (zu teuer)

PREISBEWERTUNG:
  - 100 Module:  ~€42/Modul  (vs $15 China)
  - 500 Module:  ~€30/Modul  (vs $13 China)
  - 1000 Module: ~€27/Modul  (vs $11 China)
  
  Verkauf: €80-90 pro Modul
  Gewinn: €38-63 pro Modul (Made in Germany)
```
