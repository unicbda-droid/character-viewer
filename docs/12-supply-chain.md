# Lieferkette - Woher kommt das Material?

## Kurzantwort

```
DRAM-Chips:    Korea (Samsung/Hynix), USA (Micron) - NICHT China
PCB:           China (50%), Taiwan (30%), Korea/EU (20%)
Kondensatoren: Japan (Murata), China
Widerstaende:  China (60%), Japan (30%)
Gehaeuse:      China
Heatspreader:  China oder lokal
```

**DRAM-Chips sind KEIN chinesisches Produkt.** Die Big 3 sind
Korea und USA. Aber: Die MONTAGE (Module zusammenbauen) findet
grossenteils in China statt.

---

## Komponenten im Detail

### 1. DRAM-Chips (Speicherzellen)

```
HERSTELLER       SITZ         WERKE                    WELTWEITER ANTEIL
Samsung          Korea        Pyeongtak, Hwaseong       ~40%
SK Hynix         Korea        Icheon, Cheongju           ~30%  
Micron           USA/Idaho    Boise, Manassas            ~25%
CXMT             China        Hefei                       ~5%

Alle ausser CXMT haben KEINE FABs in China (ausser Taiwan).
Samsung + SK Hynix = 70% der Weltproduktion.
```

**Fazit:** Die Speicher-Chips sind KOREANISCH, nicht chinesisch.

### 2. PCB (Leiterplatte)

```
HERSTELLER          SITZ           NOTEN
JLCPCB             China          Gut
PCBWay             China          Gut
Eurocircuits       Niederlande    Sehr gut
Wurth              Deutschland    Sehr gut
AT&S               Oesterreich    Premium
Ibiden             Japan          Premium

PCB-Bruttoanteil: 50% China, 30% Taiwan, 10% EU, 10% Japan
```

### 3. Passive Bauteile

```
TYP               FUEHRENDE HERSTELLER       ORT
Kondensatoren     Murata                      Japan
                  TDK                         Japan
                  Samsung EM                  Korea
                  Yageo                       Taiwan
                  Sunlord                     China

Widerstaende      Vishay                      USA
                  KOA                         Japan
                  Yageo                       Taiwan
                  Rohm                        Japan

Induktivitaeten   Murata                      Japan
                  TDK                         Japan
                  Coilcraft                   USA
                  Sunlord                     China

MIX: ~60% Japan/Korea/Taiwan, ~40% China
```

### 4. PMIC (Power Management IC)

```
HERSTELLER       SITZ         WERKE
Dialog (Renesas) Japan        + UK
MPS              USA          + China
Richtek          Taiwan       
Texas Instruments USA

PMIC: ~90% ausser China (Japan/Taiwan/USA)
```

### 5. SPD/Hub IC

```
HERSTELLER       SITZ         AUSWAHL
Inphi/Marvell    USA          Haufig
Renesas          Japan        Haufig
Montage:         China        (Fertigung)
```

### 6. Heatspreader / PCB-Tray

```
HERSTELLER       SITZ         MATERIAL
JMT             China         Aluminium
Shenzhen DDP    China         Kupfer
Lokale           EU           Aluminium (CNC)
```

---

## Zusammenfassung nach Material

```
MATERIAL          HERKUNFT        ANTEIL "CHINA"
─────────────────────────────────────────────────
DRAM-Chips        Korea/USA       0% (CXMT ~5%)
PCB               Global          50%
Kondensatoren     Japan/Taiwan    25%
Widerstaende      Japan/Taiwan    30%
PMIC              Japan/USA       10%
Heatspreader      China           90%
Gehaeuse          China           80%
─────────────────────────────────────────────────
GESAMT                          ~25-30%
```

**25-30% des Materials kommt aus China, aber die teuersten
Teile (DRAM-Chips) sind 0% China.**

---

## EU-Alternative: Geht das?

```
KOMPONENTE         EU-MOEGLICHKEIT           PREISAUFCHLAG
────────────────────────────────────────────────────────────
DRAM-Chips         Nein (keine EU-Fabs)      N/A
PCB                Ja (Eurocircuits)          +100-200%
Kondensatoren      Nein (keine EU-Fabs)      N/A
Widerstaende       Teilweise (Vishay EU)     +50-100%
PMIC               Nein                      N/A
Heatspreader       Ja (lokale CNC)           +50-100%
────────────────────────────────────────────────────────────
GESAMT             Unmoeglich 100% EU       +150-300%
```

**Realitaet:** 100% EU-Herstellung ist derzeit unmöglich fuer
DRAM-Module. Korea und USA dominieren die Chip-Produktion.
EU kann nur bei Montage, PCB und mechanischen Teilen helfen.

---

## Was das fuer GTA6 Europa bedeutet

```
ZIEL: Kosten offenlegen, nicht "Made in EU" erzwingen

WAS WIR SAGEN:
  "Die DRAM-Chips sind von Samsung Korea, nicht von Xiaomi China.
   Das Modul wird in China montiert. Deshalb kostet es $15 statt
   $400. Der Aufschlag von Corsair/G.Skill ist reiner Marketing-
   und Vertriebsaufschlag, kein Materialwert."

WAS WIR NICHT SAGEN:
  "Kaft chinesisch!" oder "Kauft europaeisch!"
  Sondern: "Wisst, was ihr kauft."
```

---

## Lieferketten-Risiko

```
RISIKO                  STATUS          ALTERNATIVE
Taiwan-Krise            Hoch            Samsung/Hynix (Korea)
China-Exportkontrolle   Mittel          CXMT (China) als Backup
Logistik-Engpaesse      Niedrig         Lokale Lagerhaltung
DRAM-Mangel (AI)        Hoch            Lange Vertraege mit Samsung
```

**Wichtigste Erkenntnis:** Der einzige echte geopolitische
Risiko ist Taiwan (TSMC fuer Logikchips). DRAM-Fabs stehen
in Korea - relativ sicher.
