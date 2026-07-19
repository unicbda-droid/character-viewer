# Mission: Hardware-Preise druecken

## Warum dieses Projekt existiert

Gaming-Hardware ist kuenstlich teuer. Die Herstellerkosten betragen einen Bruchteil
des Verkaufspreises. Dieses Projekt macht die Kosten transparent und liefert die
Werkzeuge, um die Preise zu druecken.

## Das Problem

```
DDR5 32GB SO-DIMM:
  Herstellungskosten:    $10-18
  Marktpreis:            $350-432
  Aufschlag:             2,000-4,000%

GPU Board (32GB VRAM):
  Herstellungskosten:    $200-400
  Marktpreis:            $1,500-2,000
  Aufschlag:             300-400%

PS5 Pro Konsole:
  BOM (TechInsights):    $420-460
  Verkaufspreis:         $699
  Aufschlag:             50-65%
```

## Die Loesung

1. **Offene Blaupausen** - Jeder kann die Designs sehen, aendern, nutzen
2. **Dokumentierte Lieferanten** - Kein Geheimnis woher die Teile kommen
3. **Fertigungsanleitungen** - Schritt fuer Schritt zur eigenen Hardware
4. **Preisrechner** - Zeigt das echte Markup jedes Produkts
5. **Lieferanten-Filter** - Waehle EU, Asien, Open-Source, Industrie

## Der Jim Keller Ansatz

> "First Principles. Keine kuenstlichen Grenzen. Nur Physik und Kosten."

Wie Jim Keller bei Tenstorrent:
- Jedes Detail ist dokumentiert
- Keine proprietären Geheimnisse
- Kosten transparent bis zur einzelnen Komponente
- Design fuer Fertigbarkeit, nicht fuer Marketing

## Display-Engpass

Der wahre Flaschenhals ist nicht die Rechenleistung, sondern die Ausgabe:

```
HDMI 2.1:    48 Gbps  → 4K@120Hz maximum
DisplayPort 2.1:  80 Gbps  → 8K@60Hz oder 4K@240Hz+
                       ↑
               67% mehr Bandbreite
               Kuenstliche Limitierung durch Hersteller
```

Konsolen sind softwareseitig auf 120Hz limitiert, obwohl die Hardware mehr kann.
Unsere Blaupause nutzt DisplayPort 2.1 und entfernt diese Limitierung.

## Montage-Philosophie

```
HEUTE:                          GTA6 EUROPA:
Komplett SMT bestueckt          Modul-System (LEGO-Prinzip)
Loeten noetig                   Quick-Connect Kupplungen
Spezialwerkzeug                 Schraubenzieher (mitgeliefert)
Stundenlanges Debugging         Plug & Play
```

## Wie du mitmachst

1. Lese die Blueprints in `blueprints/`
2. Nutze den Preisrechner in `tools/`
3. Waehle deine Lieferkette mit dem Filter
4. Bestelle bei den dokumentierten Lieferanten
5. Verkaufe oder baue dein eigenes System
6. Pushe deine Aenderungen zurueck auf GitHub

## Zahlen, die sprechen

| Was | Hersteller | Verkauf | Gewinn |
|---|---|---|---|
| DDR5 32GB | $15 | $400 | $385 (2,567%) |
| RTX 5090 | $400 | $2,000 | $1,600 (400%) |
| PS5 Pro | $420 | $699 | $279 (66%) |
| Custom Loop | $50 | $400 | $350 (700%) |

Das sind keine Verschwoerungstheorien. Das sind TechInsights BOM-Analysen,
OEM-Preise von Alibaba und verifizierte Herstellerkosten.

**GTA6 Europa macht Schluss mit dem Geheimnis.**
