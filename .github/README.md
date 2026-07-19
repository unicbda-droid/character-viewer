# GTA6 Europa

Open-source hardware blueprints for gaming hardware at manufacturing cost.

## Quick Start

```bash
# Filter suppliers
python3 tools/supplier-filter.py --region europe
python3 tools/supplier-filter.py --product ddr5-sodimm

# Calculate prices
python3 tools/price-calculator.py --product ddr5-32gb --qty 100
python3 tools/price-calculator.py --product gpu-board --qty 50
```

## See Also

- [docs/00-mission.md](docs/00-mission.md) - Why this project exists
- [docs/01-cost-expose.md](docs/01-cost-expose.md) - Cost analysis
- [docs/04-display-bandwidth.md](docs/04-display-bandwidth.md) - 4K/8K without stutter
- [suppliers/](suppliers/) - Supplier database
- [blueprints/](blueprints/) - Hardware blueprints
