#!/usr/bin/env python3
"""
GTA6 Europa - Supplier Filter
Filtert Lieferanten nach Herkunft, Lizenz, Qualitaet, etc.

Usage:
  python3 supplier-filter.py --region europe
  python3 supplier-filter.py --region asia --type oem
  python3 supplier-filter.py --product ddr5-sodimm
  python3 supplier-filter.py --quality industrial
"""

import json
import argparse
import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SUPPLIERS_FILE = os.path.join(SCRIPT_DIR, "..", "suppliers", "all-suppliers.json")


def load_suppliers():
    with open(SUPPLIERS_FILE, "r") as f:
        data = json.load(f)
    return data["suppliers"]


def filter_suppliers(suppliers, args):
    results = suppliers

    if args.region:
        region_map = {
            "europe": ["europe"],
            "asia": ["asia"],
            "global": ["global"],
            "us": ["global"],
            "cn": ["asia"],
            "de": ["europe"],
            "nl": ["europe"],
        }
        allowed = region_map.get(args.region.lower(), [args.region.lower()])
        results = [s for s in results if s["region"] in allowed]

    if args.country:
        results = [s for s in results if s["country"].upper() == args.country.upper()]

    if args.type:
        results = [s for s in results if s["type"] == args.type.lower()]

    if args.product:
        results = [s for s in results if args.product.lower() in s.get("products", [])]

    if args.quality:
        results = [s for s in results if args.quality.lower() in s.get("quality", [])]

    if args.no_china:
        results = [s for s in results if s["country"].upper() != "CN"]

    return results


def print_suppliers(suppliers, verbose=False):
    if not suppliers:
        print("Keine Lieferanten gefunden.")
        return

    print(f"\n{'='*70}")
    print(f"  {len(suppliers)} Lieferant(en) gefunden")
    print(f"{'='*70}\n")

    for s in suppliers:
        country_flags = {
            "CN": "CN", "TW": "TW", "US": "US",
            "DE": "DE", "NL": "NL", "KR": "KR",
        }
        flag = country_flags.get(s["country"], "")

        print(f"  [{s['id']}]")
        print(f"  Name:       {s['name']}")
        print(f"  Location:   {s['location']} ({flag})")
        print(f"  Type:       {s['type']}")
        print(f"  Products:   {', '.join(s['products'][:4])}")
        print(f"  MOQ:        {s['moq']}")
        print(f"  Lead Time:  {s['lead_time_days']} days")
        print(f"  Quality:    {', '.join(s['quality'])}")

        if verbose and s.get("pricing"):
            print(f"  Pricing:")
            for k, v in s["pricing"].items():
                print(f"    {k}: ${v:.2f}")

        if s.get("notes"):
            print(f"  Notes:      {s['notes']}")

        print()


def print_summary(suppliers):
    by_type = {}
    by_region = {}
    for s in suppliers:
        by_type[s["type"]] = by_type.get(s["type"], 0) + 1
        by_region[s["region"]] = by_region.get(s["region"], 0) + 1

    print("\nZusammenfassung:")
    print(f"  Nach Typ:     {by_type}")
    print(f"  Nach Region:  {by_region}")


def main():
    parser = argparse.ArgumentParser(
        description="GTA6 Europa - Supplier Filter",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --region europe
  %(prog)s --region asia --type oem
  %(prog)s --product ddr5-sodimm
  %(prog)s --quality industrial
  %(prog)s --no-china --verbose
  %(prog)s --summary
        """,
    )
    parser.add_argument("--region", help="Filter by region: europe, asia, global, us, cn, de, nl")
    parser.add_argument("--country", help="Filter by country code: CN, TW, US, DE, NL")
    parser.add_argument("--type", help="Filter by type: oem, odm, brand, pcb-fab, cooling, cable, industrial")
    parser.add_argument("--product", help="Filter by product: ddr5-sodimm, pcb, pcba, waterblock-gpu, etc.")
    parser.add_argument("--quality", help="Filter by quality: consumer, industrial, automotive")
    parser.add_argument("--no-china", action="store_true", help="Exclude Chinese suppliers")
    parser.add_argument("--verbose", "-v", action="store_true", help="Show pricing details")
    parser.add_argument("--summary", action="store_true", help="Show summary only")

    args = parser.parse_args()

    suppliers = load_suppliers()
    filtered = filter_suppliers(suppliers, args)

    if args.summary:
        print_summary(filtered)
    else:
        print_suppliers(filtered, verbose=args.verbose)

    print_summary(filtered)


if __name__ == "__main__":
    main()
