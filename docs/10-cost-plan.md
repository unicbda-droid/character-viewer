# Stueckliste & Kostenplan - DDR5 32GB SO-DIMM 5600MHz

## Vollstaendige BOM (Bill of Materials)

```
┌────┬─────────────────────────────────┬──────┬─────────┬─────────┬──────────────────────┐
│ Nr │ Komponente                     │ Stk  │ Stueck  │ Gesamt  │ Lieferant            │
│    │                                 │      │ Preis   │ Preis   │                      │
├────┼─────────────────────────────────┼──────┼─────────┼─────────┼──────────────────────┤
│  1 │ Samsung 16GB DDR5 B-Die        │  2   │ $4.50   │ $9.00   │ Digiera/Shenzhen     │
│  2 │ PMIC TPS53819 (Power Mgmt)     │  1   │ $1.20   │ $1.20   │ Digikey              │
│  3 │ SPD Hub ST504LP                │  1   │ $0.60   │ $0.60   │ Digikey              │
│  4 │ PCB 8-Layer SO-DIMM (ENIG)     │  1   │ $2.50   │ $2.50   │ JLCPCB               │
│  5 │ 0201 Resistor Array 100K       │ 12   │ $0.01   │ $0.12   │ Digikey/Mouser       │
│  6 │ 0201 MLCC 100nF Capacitor      │ 20   │ $0.01   │ $0.20   │ Digikey/Mouser       │
│  7 │ Aluminum Heatspreader          │  1   │ $1.00   │ $1.00   │ Bykski/Alibaba       │
│  8 │ Thermal Pad (0.5mm)            │  1   │ $0.20   │ $0.20   │ Alibaba              │
│  9 │ Anti-Static Bag                │  1   │ $0.10   │ $0.10   │ Alibaba              │
│ 10 │ Label + Manual                 │  1   │ $0.15   │ $0.15   │ Selbst/Digital       │
│ 11 │ SMT Assembly (JLCPCB)          │  1   │ $2.00   │ $2.00   │ JLCPCB               │
│ 12 │ Testing (MemTest86, 4h)        │  1   │ $0.50   │ $0.50   │ In-House             │
│ 13 │ Packaging (Carton)             │  1   │ $0.30   │ $0.30   │ Alibaba              │
├────┼─────────────────────────────────┼──────┼─────────┼─────────┼──────────────────────┤
│    │ GESAMT HERSTELLUNG             │      │         │$17.87   │                      │
└────┴─────────────────────────────────┴──────┴─────────┴─────────┴──────────────────────┘
```

## Alternative: Komplett OEM (Digiera/NexaRAM)

```
┌────┬─────────────────────────────────┬──────┬─────────┬─────────┬──────────────────────┐
│ Nr │ Komponente                     │ Stk  │ Stueck  │ Gesamt  │ Lieferant            │
│    │                                 │      │ Preis   │ Preis   │                      │
├────┼─────────────────────────────────┼──────┼─────────┼─────────┼──────────────────────┤
│  1 │ Komplettes DDR5 32GB SO-DIMM   │  1   │ $12-18  │ $15.00  │ Digiera (OEM)        │
│    │ (fertig bestueckt, getestet)   │      │         │         │                      │
│  2 │ Custom Heatspreader (Brand)    │  1   │ $1.50   │ $1.50   │ Digiera              │
│  3 │ Custom Label + Packaging       │  1   │ $0.50   │ $0.50   │ Digiera              │
├────┼─────────────────────────────────┼──────┼─────────┼─────────┼──────────────────────┤
│    │ GESAMT (OEM Komplett)          │      │         │$17.00   │                      │
└────┴─────────────────────────────────┴──────┴─────────┴─────────┴──────────────────────┘
```

## Gesamtkosten inkl. Nebenkosten

```
HERSTELLUNG (pro Modul):
  BOM (selbst):               $17.87
  ODER OEM (Digiera):         $17.00
  ─────────────────────────────────────
  Durchschnitt:               $17.44

LOGISTIK (pro Modul):
  Versand China->DE (DHL):    $2.50  (bei 100 Stk)
  Zoll/Einfuhr:               $0.80  (5% von $16)
  Lagerhaltung:               $0.20  (pro Monat)
  ─────────────────────────────────────
  Gesamt Logistik:            $3.50

MARKETING + ADMIN (pro Modul):
  eBay/Amazon Gebuehr (15%):  variabel
  Fotos + Listing:            $0.50  (amortisiert)
  Kundenservice:              $0.30  (amortisiert)
  Garantie-Ruecklagen (5%):   variabel
  ─────────────────────────────────────
  Gesamt Admin:               $0.80

GESAMTKOSTEN PRO MODUL:
  Herstellung:                $17.44
  Logistik:                   $3.50
  Admin:                      $0.80
  ─────────────────────────────────────
  GESAMT:                     $21.74
```

## Verkaufspreise & Gewinn

```
┌──────────────────────┬──────────┬──────────┬──────────┬──────────┐
│ Verkaufskanal        │ Verkauf- │ Gebuehr  │ Netto-   │ Gewinn/  │
│                      │ preis    │ (15%)    │ Einnahme │ Modul    │
├──────────────────────┼──────────┼──────────┼──────────┼──────────┤
│ eBay.de              │ $80.00   │ $12.00   │ $68.00   │ $46.26   │
│ Amazon.de (FBA)      │ $90.00   │ $13.50   │ $76.50   │ $54.76   │
│ Kleinanzeigen        │ $75.00   │ $0.00    │ $75.00   │ $53.26   │
│ Eigener Webshop      │ $85.00   │ $2.55*   │ $82.45   │ $60.71   │
│ B2B (Systemhaeuser)  │ $60.00   │ $0.00    │ $60.00   │ $38.26   │
│ Grosshandel          │ $45.00   │ $0.00    │ $45.00   │ $23.26   │
├──────────────────────┼──────────┼──────────┼──────────┼──────────┤
│ DURCHSCHNITT         │ $72.50   │          │ $68.66   │ $46.92   │
└──────────────────────┴──────────┴──────────┴──────────┴──────────┘
  * Eigener Webshop: Stripe/PayPal ~3%
```

## Skalierung (Gewinn pro Menge)

```
┌──────────┬──────────┬──────────┬──────────┬──────────┐
│ Menge    │ Gesamt-  │ Gesamt-  │ Umsatz   │ Reiner   │
│ (Stk)    │ kosten   │ gewinn   │          │ Gewinn   │
├──────────┼──────────┼──────────┼──────────┼──────────┤
│       10 │   $217   │   $469   │   $725   │   $469   │
│       50 │ $1,087   │ $2,346   │ $3,625   │ $2,346   │
│      100 │ $2,174   │ $4,692   │ $7,250   │ $4,692   │
│      250 │ $5,435   │$11,730   │$18,125   │$11,730   │
│      500 │$10,870   │$23,460   │$36,250   │$23,460   │
│    1,000 │$21,740   │$46,920   │$72,500   │$46,920   │
│    5,000 │$108,700  │$234,600  │$362,500  │$234,600  │
└──────────┴──────────┴──────────┴──────────┴──────────┘

AB 500 STUECK:
  OEM-Preis bei Digiera: ~$10-12 (statt $15-18)
  Gesamtkosten: ~$16/Modul
  Gewinn/Modul: ~$55
  Gewinn bei 500: ~$27,500
```

## Preisvergleich mit Marktpreisen

```
PRODUKT: DDR5 32GB SO-DIMM 5600MHz

  Corsair Vengeance:    $432  (Amazon)
  G.Skill Trident:      $600  (Amazon)
  Kingston Fury:        $380  (Amazon)
  Samsung Original:     $350  (Distributor)
  ────────────────────────────────────
  DURCHSCHNITT MARKT:   $440

  GTA6 Europa:          $80   (eBay/Amazon)
  GTA6 Europa B2B:      $60   (Systemhaeuser)
  ────────────────────────────────────
  Ersparnis Kaeufer:    80-86%
  Gewinn pro Modul:     $38-58
```

## Erster Einkauf - Empfehlung

```
STARTER-PAKET (100 Module):
============================

Option A: Selbst bauen
  DRAM Chips (200x Samsung 16GB):    $900  (Shenzhen Kedun)
  PCB (100x 8-Layer):               $250  (JLCPCB)
  PMIC + SPD (100x):                $180  (Digikey)
  Passives (100x):                   $32  (Digikey)
  SMT Assembly (100x):              $200  (JLCPCB)
  Heatspreader (100x):              $100  (Alibaba)
  Testing + Packaging:              $100  (In-House)
  ──────────────────────────────────────────
  GESAMT:                          $1,762
  PRO MODUL:                         $17.62

Option B: OEM (empfohlen)
  Digiera: 100x DDR5 32GB SO-DIMM  $1,500  ($15/Modul)
  Custom Label + Packaging:          $200
  Versand nach DE:                   $250
  Zoll:                              $80
  ──────────────────────────────────────────
  GESAMT:                          $2,030
  PRO MODUL:                         $20.30

VERKAUF (100 Module):
  eBay (100x $80):                $8,000
  Gebuehr (15%):                  -$1,200
  Netto:                           $6,800
  Gesamtkosten:                   -$2,030
  ──────────────────────────────────────────
  GEWINN:                          $4,770
  ROI:                             235%
```

## Kontakt fuer Erstbestellung

```
DIGIERA (empfohlen fuer Erstbestellung):
  Website: digieraglobal.com
  E-Mail: sales@digieraglobal.com
  Betreff: "DDR5 32GB SO-DIMM 5600 OEM Quote Request - 100 Units"
  
  Nachricht:
  "We are interested in ordering 100x DDR5 32GB SO-DIMM 5600MHz modules
   with custom branding and heatspreader. Please provide:
   1. Unit price for 100pcs
   2. Lead time
   3. Customization options (label, heatspreader color)
   4. Shipping cost to Germany
   5. Payment terms"

SHENZHEN KEDUN (fuer schnellen Start):
  Website: keduntech.en.alibaba.com
  Direkt bestellen: ab 2 Stueck
  Preis: $11.33/Modul
```
