# SOXIM Analysis Scripts

Python tools for analyzing soxim simulation results.

## Installation

```bash
cd scripts
pip install -r requirements.txt
```

## Scripts

### 1. analyze.py - Basic Analysis

Analyze a single simulation run and generate visualizations.

```bash
# Basic analysis
./analyze.py traffic/TrafficInformation.csv

# Generate plots
./analyze.py traffic/ -o results.png

# Print statistics only
./analyze.py traffic/ --stats-only
```

**Features:**
- Packet reception statistics
- Latency distribution histogram
- Latency vs time scatter plot
- Throughput per node
- Source-destination latency heatmap

### 2. sweep.py - Parameter Sweep

Run multiple simulations with varying injection rates to generate saturation curves.

```bash
# Manual rate list
./sweep.py -c ../configs/example.toml --rates 0.01 0.05 0.1 0.2 0.3 -o saturation.png

# Automatic rate range
./sweep.py -c ../configs/example.toml --min-rate 0.01 --max-rate 0.5 -n 10

# Using different config
./sweep.py -c ../configs/mesh_8x8.toml -o mesh_saturation.png
```

**Output:**
- CSV file with results
- Saturation curve plot (throughput vs injection rate)
- Latency vs injection rate plot

### 3. compare.py - Compare Runs

Compare results from multiple simulation configurations.

```bash
# Compare two runs
./compare.py run1/traffic/ run2/traffic/ -l "Mesh" "Torus"

# Compare multiple algorithms
./compare.py dor/traffic/ romm/traffic/ mad/traffic/ -l "DOR" "ROMM" "MAD" -o comparison.png
```

## Output Files

All scripts generate:
- **Console output**: Statistics and summary tables
- **PNG plots**: Visualizations (if `-o` flag used)
- **CSV files**: Raw data for further analysis (sweep.py)

## Examples

### Complete Workflow

```bash
# 1. Run a simulation
../build/src/soxim ../configs/example.toml

# 2. Analyze results
./analyze.py traffic/TrafficInformation.csv -o my_analysis.png

# 3. Run parameter sweep
./sweep.py -c ../configs/example.toml --rates 0.01 0.05 0.1 0.2 0.3 0.4 0.5

# 4. Compare different topologies
./compare.py mesh_results/ torus_results/ -l "Mesh" "Torus" -o topo_compare.png
```

### Quick Start

```bash
# Analyze latest run
cd /tmp/soxim/build/src
./soxim ../../configs/example.toml
cd ../../scripts
./analyze.py traffic/TrafficInformation.csv
```

## Notes

- Scripts expect `TrafficInformation.csv` format from soxim
- All scripts support both file paths and directory paths
- Output directory (`traffic/`) is created automatically by soxim
- Use `-h` flag on any script for detailed help
