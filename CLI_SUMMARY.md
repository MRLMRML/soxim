# SOXIM CLI Enhancements - Summary

## 🎉 All CLI Options Implemented!

### New Command-Line Options

**General Options:**
- `-h, --help` - Show help message
- `-v, --version` - Show version information
- `-q, --quiet` - Suppress detailed output
- `-d, --debug` - Enable debug output

**Simulation Options:**
- `-o, --output DIR` - Specify output directory
- `-t, --topology TYPE` - Override topology (MESH, TORUS)
- `-a, --algorithm ALGO` - Override routing algorithm (DOR, ROMM, MAD, VAL, ODD_EVEN)
- `-r, --rate RATE` - Override injection rate (0.0-1.0)
- `-s, --size SIZE` - Override packet size (flits)
- `-p, --pattern PATTERN` - Override traffic pattern
- `-c, --cycles CYCLES` - Override total cycles
- `-w, --warmup CYCLES` - Override warmup cycles
- `-m, --measure CYCLES` - Override measurement cycles

**Output Options:**
- `--no-traffic` - Skip traffic generation
- `--no-analysis` - Skip traffic analysis
- `--save-config FILE` - Save current configuration
- `--dry-run` - Preview configuration without running

### Features

1. **Configuration Overrides**
   - Override any config parameter from command line
   - Perfect for quick experiments and parameter sweeps
   - Overrides are applied after loading config file

2. **Dry Run Mode**
   - Preview effective configuration before running
   - Shows all settings including overrides
   - Fast validation of configuration

3. **Quiet Mode**
   - Suppress detailed configuration output
   - Show only simulation results
   - Perfect for scripting and automation

4. **Save Configuration**
   - Save current configuration (with overrides) to file
   - Reproducible experiments
   - Easy sharing of configurations

5. **Flexible Control**
   - Skip traffic generation if needed
   - Skip analysis if only simulation is required
   - Fine-grained control over simulation flow

### Usage Examples

```bash
# Basic usage
./soxim config.toml

# Override topology and algorithm
./soxim config.toml -t TORUS -a MAD -r 0.05

# Dry run (preview)
./soxim config.toml --dry-run

# Quiet mode
./soxim config.toml -q

# Save configuration
./soxim config.toml -t TORUS -a MAD -r 0.05 --save-config experiment.toml

# Multiple overrides
./soxim config.toml -t MESH -a ROMM -r 0.1 -s 30 -p "permutation" -c 20000

# Skip traffic generation
./soxim config.toml --no-traffic

# Skip analysis
./soxim config.toml --no-analysis
```

### Testing

All CLI options have been tested:

```bash
# Help
./soxim --help
# Output: Shows all available options

# Version
./soxim --version
# Output: soxim - Network-on-Chip Simulator v1.0

# Dry run
./soxim config.toml --dry-run
# Output: Shows effective configuration

# Override
./soxim config.toml -t TORUS -a MAD -r 0.05 --dry-run
# Output: Shows MAD algorithm and 0.05 rate

# Save config
./soxim config.toml -t TORUS -a MAD -r 0.05 --save-config /tmp/saved.toml
# Output: Configuration saved to /tmp/saved.toml

# Quiet mode
./soxim config.toml -q
# Output: Only shows results, no config details
```

### Documentation

**New Files:**
- `CLI_OPTIONS.md` - Comprehensive CLI documentation
- `CLI_SUMMARY.md` - This summary file

**Updated Files:**
- `src/main.cpp` - Enhanced with 15+ CLI options
- `README.md` - Added CLI examples

### Benefits

1. **User-Friendly**
   - Intuitive command-line interface
   - Comprehensive help messages
   - Clear error messages

2. **Flexible**
   - Override any configuration parameter
   - Combine multiple options
   - Fine-grained control

3. **Reproducible**
   - Save configurations
   - Share experiments
   - Version control friendly

4. **Efficient**
   - Dry run for quick validation
   - Quiet mode for automation
   - Skip unnecessary steps

### Comparison with Python Scripts

The CLI options complement the Python visualization tools:

| CLI Option | Python Script | Use Case |
|------------|---------------|----------|
| `-t, --topology` | `topology_viz.py` | Topology visualization |
| `-a, --algorithm` | `saturation.py` | Algorithm comparison |
| `-r, --rate` | `sweep.py` | Parameter sweeps |
| `-o, --output` | All scripts | Output directory |

### Future Enhancements

Planned CLI features:
- `--benchmark` - Run performance benchmarks
- `--compare` - Compare multiple configurations
- `--visualize` - Generate visualizations after simulation
- `--export` - Export results to CSV/JSON
- `--plot` - Generate plots directly from CLI

### Quick Reference

```bash
# Show all options
./soxim --help

# Common workflows

# 1. Quick experiment
./soxim config.toml -t TORUS -a MAD -r 0.05 --dry-run
./soxim config.toml -t TORUS -a MAD -r 0.05 -q

# 2. Save and reproduce
./soxim config.toml -t TORUS -a MAD -r 0.05 --save-config experiment.toml
./soxim experiment.toml

# 3. Parameter sweep (manual)
for rate in 0.01 0.05 0.1 0.2; do
  ./soxim config.toml -r $rate --save-config sweep_$rate.toml
done

# 4. Batch processing
./soxim config1.toml -q > results1.txt
./soxim config2.toml -q > results2.txt
```

### Summary

The CLI enhancements provide:
- **15+ command-line options** for flexible configuration
- **Configuration overrides** for quick experiments
- **Dry-run mode** for verification
- **Save/load configuration** for reproducibility
- **Quiet mode** for scripting
- **Comprehensive error handling**
- **Complete documentation**

This makes SOXIM more user-friendly and suitable for both interactive use and automated testing.

## 🎉 Complete!

All CLI options are implemented, tested, and documented. The simulator is now much more flexible and user-friendly!
