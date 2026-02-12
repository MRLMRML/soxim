# SOXIM CLI Options

## Overview

SOXIM now supports extensive command-line options for flexible simulation configuration and control.

## Usage

```bash
./soxim [OPTIONS] [CONFIG_FILE]
```

## General Options

### Help and Version

| Option | Description |
|--------|-------------|
| `-h, --help` | Show help message with all available options |
| `-v, --version` | Show version information |

### Output Control

| Option | Description |
|--------|-------------|
| `-q, --quiet` | Suppress detailed configuration output (show only results) |
| `-d, --debug` | Enable debug output (not yet implemented) |

## Simulation Options

### Configuration Overrides

| Option | Description | Example |
|--------|-------------|---------|
| `-o, --output DIR` | Specify output directory for traffic files | `-o /tmp/results/` |
| `-t, --topology TYPE` | Override topology type | `-t TORUS` or `-t MESH` |
| `-a, --algorithm ALGO` | Override routing algorithm | `-a MAD` or `-a ROMM` |
| `-r, --rate RATE` | Override injection rate (0.0-1.0) | `-r 0.05` |
| `-s, --size SIZE` | Override packet size in flits | `-s 50` |
| `-p, --pattern PATTERN` | Override traffic pattern | `-p "random uniform"` |
| `-c, --cycles CYCLES` | Override total cycles | `-c 20000` |
| `-w, --warmup CYCLES` | Override warmup cycles | `-w 5000` |
| `-m, --measure CYCLES` | Override measurement cycles | `-m 10000` |

### Routing Algorithms

Available algorithms for `-a, --algorithm`:
- `DOR` - Dimension-Order Routing (default)
- `ROMM` - Randomized Oblivious Multi-phase Minimal
- `MAD` - Minimal Adaptive
- `VAL` - Valiant's Randomized Algorithm
- `ODD_EVEN` - Odd-Even Adaptive

### Topology Types

Available types for `-t, --topology`:
- `MESH` - Mesh topology
- `TORUS` - Torus topology

### Traffic Patterns

Available patterns for `-p, --pattern`:
- `random uniform` - Random uniform traffic
- `permutation` - Permutation traffic

## Output Options

| Option | Description |
|--------|-------------|
| `--no-traffic` | Skip traffic generation (run simulation only) |
| `--no-analysis` | Skip traffic analysis after simulation |
| `--save-config FILE` | Save current configuration to file |
| `--dry-run` | Parse config and show settings, don't run simulation |

## Examples

### Basic Usage

```bash
# Run with default config
./soxim

# Run with custom config
./soxim my_config.toml

# Specify output directory
./soxim -o /tmp/results/ config.toml
```

### Override Configuration

```bash
# Override topology and algorithm
./soxim -t TORUS -a MAD -r 0.05

# Override multiple parameters
./soxim -t MESH -a ROMM -r 0.1 -s 30 -p "permutation"

# Override cycle counts
./soxim -c 20000 -w 5000 -m 10000
```

### Dry Run (Preview Configuration)

```bash
# Show effective configuration without running
./soxim config.toml --dry-run

# With overrides
./soxim config.toml -t TORUS -a MAD -r 0.05 --dry-run
```

### Save Configuration

```bash
# Save configuration with overrides
./soxim config.toml -t TORUS -a MAD -r 0.05 --save-config saved_config.toml

# Then run with saved config
./soxim saved_config.toml
```

### Quiet Mode

```bash
# Suppress detailed output, show only results
./soxim config.toml -q

# Combine with overrides
./soxim config.toml -t TORUS -a MAD -r 0.05 -q
```

### Skip Traffic Generation

```bash
# Run simulation without traffic generation
./soxim config.toml --no-traffic

# Run simulation without analysis
./soxim config.toml --no-analysis
```

## Configuration File

The configuration file is a TOML file with the following structure:

```toml
[cycles]
total = 10000
warmup = 3000
measurement = 3000

[microarchitecture]
buffer_size = 8
virtual_channel_number = 8

[routing]
algorithm = "DOR"

[topology]
dimension = [ 8, 8, 1 ]
shape = "TORUS"

[traffic]
alpha = 0.5
beta = 0.5
flit_size = 1
injection_process = "periodic"
injection_rate = 0.01
packet_size = 20
packet_size_option = "random uniform"
traffic_pattern = "random uniform"
```

## CLI Overrides vs Config File

CLI options override configuration file settings:

1. **Config file** is loaded first
2. **CLI overrides** are applied
3. **Effective configuration** is used for simulation

Example:
```bash
# Config file has: injection_rate = 0.01
# CLI override: -r 0.05
# Effective rate: 0.05
./soxim config.toml -r 0.05
```

## Exit Codes

- `0` - Success
- `1` - Error (parsing failed, invalid arguments, etc.)

## Error Handling

### Missing Arguments
```bash
./soxim -o
# Error: Missing argument for -o
```

### Invalid Values
```bash
./soxim -r invalid
# Error: Invalid rate value: invalid
```

### Unknown Options
```bash
./soxim --unknown-option
# Error: Unknown option: --unknown-option
```

## Tips

### 1. Use Dry Run First
Always use `--dry-run` to verify your configuration before running a full simulation:
```bash
./soxim config.toml -t TORUS -a MAD -r 0.05 --dry-run
```

### 2. Save Configuration
Save your configuration for reproducibility:
```bash
./soxim config.toml -t TORUS -a MAD -r 0.05 --save-config experiment1.toml
```

### 3. Use Quiet Mode for Scripts
Use `-q` in scripts to reduce output:
```bash
./soxim config.toml -q > results.txt
```

### 4. Combine Options
Combine multiple options for complex scenarios:
```bash
./soxim config.toml -t TORUS -a MAD -r 0.05 -s 30 -c 20000 -q
```

## Comparison with Python Scripts

The CLI options provide similar functionality to the Python scripts:

| CLI Option | Python Script | Description |
|------------|---------------|-------------|
| `-t, --topology` | `topology_viz.py` | Topology type |
| `-a, --algorithm` | `saturation.py` | Routing algorithm |
| `-r, --rate` | `sweep.py` | Injection rate |
| `-o, --output` | All scripts | Output directory |

## Future Enhancements

Planned CLI features:
- `--benchmark` - Run performance benchmarks
- `--compare` - Compare multiple configurations
- `--visualize` - Generate visualizations after simulation
- `--export` - Export results to CSV/JSON
- `--plot` - Generate plots directly from CLI

## Summary

The enhanced CLI provides:
- **15+ command-line options** for flexible configuration
- **Configuration overrides** for quick experiments
- **Dry-run mode** for verification
- **Save/load configuration** for reproducibility
- **Quiet mode** for scripting
- **Comprehensive error handling**

This makes SOXIM more user-friendly and suitable for both interactive use and automated testing.
