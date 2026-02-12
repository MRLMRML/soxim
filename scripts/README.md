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

### 4. visualize.py - Comprehensive Visualization

Enhanced visualization with 9-panel analysis including heatmaps, CDFs, and saturation curves.

```bash
# Basic visualization
./visualize.py ../build/src/traffic/ -o docs/figures/visualization_results.png

# Statistics only
./visualize.py ../build/src/traffic/ --stats-only

# Save processed data
./visualize.py ../build/src/traffic/ --save-data -o results.png
```

**Features:**
- Latency distribution histogram
- Latency vs time scatter plot
- Throughput per node bar chart
- Latency heatmap (source-destination)
- Network topology visualization
- Contention analysis
- Saturation curve
- Latency CDF
- Summary statistics table
- Professional matplotlib/seaborn styling

### 5. topology_viz.py - Network Topology Visualization

Visualize network topology with traffic overlay and routing paths.

```bash
# 2D Mesh topology
./topology_viz.py --topology MESH --x 8 --y 8 -o topology.png

# 3D Mesh topology
./topology_viz.py --topology MESH --x 4 --y 4 --z 2 -o topology_3d.png

# Torus topology with traffic overlay
./topology_viz.py --topology TORUS --x 8 --y 8 \
  --input ../build/src/traffic/ --show-routing \
  -o topology_with_traffic.png

# Highlight specific nodes
./topology_viz.py --topology MESH --x 8 --y 8 \
  --highlight-nodes 0 7 56 63 -o highlighted.png
```

**Features:**
- 2D/3D Mesh and Torus visualization
- Traffic throughput overlay
- Routing path visualization
- Node highlighting
- Configurable dimensions
- Professional styling

### 6. saturation.py - Saturation Curve Analysis

Analyze saturation points and compare routing algorithms.

```bash
# Single algorithm analysis
./saturation.py ../build/src/traffic/ -o saturation.png

# Compare multiple algorithms
./saturation.py results/ --compare -o comparison.png

# Find saturation points
./saturation.py results/ --find-saturation --compare

# Filter by algorithm or pattern
./saturation.py results/ --algorithm DOR --traffic-pattern "random uniform"
```

**Features:**
- Throughput vs injection rate plots
- Latency comparison
- Saturation point detection
- Algorithm comparison
- Statistical analysis
- Multiple result comparison

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
