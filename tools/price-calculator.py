#!/usr/bin/env python3
"""
GTA6 Europa - Price Calculator
Berechnet Herstellungskosten, Verkaufspreis und Gewinn.

Usage:
  python3 price-calculator.py --product ddr5-32gb --qty 100
  python3 price-calculator.py --bom blueprints/gpu-board/bom-gpu.csv --qty 50
  python3 price-calculator.py --product ddr5-32gb --supplier ddr5-shenzhen-kedun --qty 100
"""

import json
import argparse
import csv
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SUPPLIERS_FILE = os.path.join(SCRIPT_DIR, "..", "suppliers", "all-suppliers.json")

# Predefined product BOMs
PRODUCT_BOMS = {
    "ddr5-8gb": {
        "name": "DDR5 8GB SO-DIMM 4800",
        "components": [
            {"name": "DRAM Die (8GB DDR5)", "unit_cost": 2.50},
            {"name": "PMIC (Power Management IC)", "unit_cost": 0.80},
            {"name": "SPD Hub", "unit_cost": 0.40},
            {"name": "PCB (6-Layer SO-DIMM)", "unit_cost": 1.20},
            {"name": "Resistors + Capacitors", "unit_cost": 0.30},
            {"name": "Heatspreader (Aluminum)", "unit_cost": 0.50},
            {"name": "Assembly + Testing", "unit_cost": 1.00},
            {"name": "Packaging", "unit_cost": 0.30},
        ],
    },
    "ddr5-16gb": {
        "name": "DDR5 16GB SO-DIMM 5600",
        "components": [
            {"name": "DRAM Die (16GB DDR5)", "unit_cost": 4.50},
            {"name": "PMIC", "unit_cost": 1.00},
            {"name": "SPD Hub", "unit_cost": 0.50},
            {"name": "PCB (6-Layer SO-DIMM)", "unit_cost": 1.50},
            {"name": "Resistors + Capacitors", "unit_cost": 0.40},
            {"name": "Heatspreader", "unit_cost": 0.70},
            {"name": "Assembly + Testing", "unit_cost": 1.50},
            {"name": "Packaging", "unit_cost": 0.40},
        ],
    },
    "ddr5-32gb": {
        "name": "DDR5 32GB SO-DIMM 5600",
        "components": [
            {"name": "DRAM Die (16GB x2)", "unit_cost": 8.00},
            {"name": "PMIC", "unit_cost": 1.20},
            {"name": "SPD Hub", "unit_cost": 0.60},
            {"name": "PCB (8-Layer SO-DIMM)", "unit_cost": 2.50},
            {"name": "Resistors + Capacitors", "unit_cost": 0.50},
            {"name": "Heatspreader (Aluminum)", "unit_cost": 1.00},
            {"name": "Assembly + Testing", "unit_cost": 2.00},
            {"name": "Packaging", "unit_cost": 0.50},
        ],
    },
    "gpu-board": {
        "name": "GPU Board (32GB VRAM, DP 2.1)",
        "components": [
            {"name": "GPU Chip (Tenstorrent/RDNA3)", "unit_cost": 150.00},
            {"name": "VRAM (8x 4GB GDDR6)", "unit_cost": 80.00},
            {"name": "VRM (Voltage Regulator)", "unit_cost": 18.00},
            {"name": "PCB (12-Layer, HDI)", "unit_cost": 45.00},
            {"name": "DisplayPort 2.1 Connectors", "unit_cost": 6.00},
            {"name": "Passive Components", "unit_cost": 8.00},
            {"name": "Heatsink + Fans", "unit_cost": 22.00},
            {"name": "Assembly + Testing", "unit_cost": 15.00},
            {"name": "Packaging", "unit_cost": 4.00},
        ],
    },
    "sbc-console": {
        "name": "SBC Console (DP 2.1, DDR5)",
        "components": [
            {"name": "SoC (RISC-V / RDNA3 APU)", "unit_cost": 120.00},
            {"name": "RAM (DDR5 SO-DIMM)", "unit_cost": 15.00},
            {"name": "NVMe SSD (2TB)", "unit_cost": 90.00},
            {"name": "PCB (8-Layer)", "unit_cost": 35.00},
            {"name": "DisplayPort 2.1 + HDMI 2.1", "unit_cost": 8.00},
            {"name": "WiFi 6E + Bluetooth", "unit_cost": 6.00},
            {"name": "USB Controllers", "unit_cost": 4.00},
            {"name": "Power Supply (12V DC)", "unit_cost": 10.00},
            {"name": "Passive Components", "unit_cost": 12.00},
            {"name": "Assembly + Testing", "unit_cost": 20.00},
            {"name": "Packaging", "unit_cost": 5.00},
        ],
    },
    "cooling-loop": {
        "name": "Custom Water Cooling Loop",
        "components": [
            {"name": "CPU Waterblock (Copper)", "unit_cost": 6.00},
            {"name": "GPU Waterblock", "unit_cost": 10.00},
            {"name": "Pump (D5/DC-LT)", "unit_cost": 4.00},
            {"name": "Radiator 240mm", "unit_cost": 10.00},
            {"name": "Quick-Connect Fittings (4x)", "unit_cost": 6.00},
            {"name": "G1/4 Fittings (6x)", "unit_cost": 4.00},
            {"name": "Soft-Tube (1m)", "unit_cost": 1.50},
            {"name": "Coolant (500ml)", "unit_cost": 2.50},
            {"name": "Packaging", "unit_cost": 3.00},
        ],
    },
}

MARKET_PRICES = {
    "ddr5-8gb": 60,
    "ddr5-16gb": 150,
    "ddr5-32gb": 400,
    "gpu-board": 1800,
    "sbc-console": 699,
    "cooling-loop": 400,
}


def load_suppliers():
    with open(SUPPLIERS_FILE, "r") as f:
        data = json.load(f)
    return {s["id"]: s for s in data["suppliers"]}


def calculate_bom_cost(components, qty):
    total = sum(c["unit_cost"] for c in components)
    # Volume discount: 5% at 100+, 10% at 500+, 15% at 1000+
    if qty >= 1000:
        total *= 0.85
    elif qty >= 500:
        total *= 0.90
    elif qty >= 100:
        total *= 0.95
    return total


def calculate_marketing_costs(qty):
    """Marketing + Logistics per unit"""
    return 5.00 + (1000.00 / max(qty, 1))


def print_product_analysis(product_id, bom, suppliers, qty, supplier_id=None):
    bom_cost = calculate_bom_cost(bom["components"], qty)
    market_price = MARKET_PRICES.get(product_id, 0)

    print(f"\n{'='*70}")
    print(f"  GTA6 Europa - Preisrechner")
    print(f"{'='*70}")
    print(f"\n  Produkt:     {bom['name']}")
    print(f"  Menge:       {qty} Stueck")

    print(f"\n  {'Komponente':<40} {'Stueck':>10} {'Gesamt':>12}")
    print(f"  {'-'*62}")
    for c in bom["components"]:
        print(f"  {c['name']:<40} ${c['unit_cost']:>8.2f} ${c['unit_cost'] * qty:>10.2f}")
    print(f"  {'-'*62}")
    print(f"  {'BOM Gesamt':<40} {'':>10} ${bom_cost * qty:>10.2f}")
    print(f"  {'BOM pro Stueck':<40} {'':>10} ${bom_cost:>10.2f}")

    if supplier_id and supplier_id in suppliers:
        s = suppliers[supplier_id]
        price_key = f"{product_id}-32gb" if "32gb" in product_id else product_id
        supplier_price = s.get("pricing", {}).get(price_key, 0)
        if supplier_price:
            print(f"\n  Lieferant:   {s['name']}")
            print(f"  OEM-Preis:   ${supplier_price:.2f}/Stueck")
            print(f"  OEM Gesamt:  ${supplier_price * qty:.2f}")

    mfg_cost_per_unit = bom_cost
    mkt_cost = calculate_marketing_costs(qty)
    total_cost_per_unit = mfg_cost_per_unit + mkt_cost

    print(f"\n  --- Kalkulation ---")
    print(f"  Herstellung/Stueck:     ${mfg_cost_per_unit:>10.2f}")
    print(f"  Marketing+Logistik:     ${mkt_cost:>10.2f}")
    print(f"  Gesamtkosten/Stueck:    ${total_cost_per_unit:>10.2f}")

    if market_price:
        retail_commission = market_price * 0.15
        net_price = market_price - retail_commission
        profit_retail = net_price - total_cost_per_unit
        markup_retail = ((net_price / total_cost_per_unit) - 1) * 100

        print(f"\n  --- Verkaufspreise ---")
        print(f"  Marktpreis (heute):     ${market_price:>10.2f}")
        print(f"  Plattform-Gebühr (15%): ${retail_commission:>10.2f}")
        print(f"  Netto-Einnahme:         ${net_price:>10.2f}")
        print(f"  Gewinn (vs Marktpreis): ${profit_retail:>10.2f} ({markup_retail:.0f}%)")

    target_prices = [market_price * 0.5, market_price * 0.3, market_price * 0.2]
    labels = ["50% des Marktes", "30% des Marktes", "20% des Marktes"]
    print(f"\n  --- GTA6 Europa Verkaufspreise ---")
    for label, price in zip(labels, target_prices):
        commission = price * 0.15
        profit = price - commission - total_cost_per_unit
        print(f"  {label:<25} ${price:>8.2f}  Gewinn: ${profit:>8.2f}/Stueck")

    print(f"\n  --- Zusammenfassung ---")
    print(f"  Ersparnis fuer Kunden:  50-80% weniger als Marktpreis")
    print(f"  Gewinn pro Stueck:      $50-{int(market_price * 0.3)}")
    print(f"  Gewinn bei {qty} Stueck:    ${50 * qty}-{int(market_price * 0.3) * qty}")
    print()


def main():
    parser = argparse.ArgumentParser(
        description="GTA6 Europa - Price Calculator",
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("--product", choices=list(PRODUCT_BOMS.keys()),
                        help="Product to analyze")
    parser.add_argument("--supplier", help="Supplier ID for OEM pricing")
    parser.add_argument("--qty", type=int, default=100, help="Quantity (default: 100)")
    parser.add_argument("--bom", help="Path to custom BOM CSV file")

    args = parser.parse_args()

    if not args.product and not args.bom:
        parser.print_help()
        sys.exit(1)

    suppliers = load_suppliers()

    if args.bom:
        with open(args.bom, "r") as f:
            reader = csv.DictReader(f)
            components = [{"name": row["name"], "unit_cost": float(row["unit_cost"])} for row in reader]
        bom = {"name": os.path.basename(args.bom), "components": components}
        product_id = "custom"
    else:
        bom = PRODUCT_BOMS[args.product]
        product_id = args.product

    print_product_analysis(product_id, bom, suppliers, args.qty, args.supplier)


if __name__ == "__main__":
    main()
