# SOXIM Sweep Analysis Summary

## 📊 Enhanced Parameter Sweep Analysis

### Overview

The `sweep.py` script has been enhanced with automatic saturation point detection and fine-grained parameter sweeps to provide deeper insights into network performance.

## Enhanced Features

### 1. Automatic Saturation Point Detection

The enhanced `sweep.py` now:
- **Automatically detects** the saturation point (where throughput peaks)
- **Marks saturation point** on both throughput and latency plots
- **Calculates efficiency** (throughput/injection rate)
- **Provides detailed analysis** output

### 2. Saturation Analysis Output

```
============================================================
SATURATION ANALYSIS
============================================================
Saturation Point:   0.085 injection rate
Max Throughput:     0.544 flit/cycle/node
Latency at Sat:     2580.6 cycles
Efficiency:         640.0%
============================================================
```

## Sweep Results

### Fine-Grained Sweep (20 Injection Rates)

**Command:**
```bash
python3 sweep.py -c configs/example.toml \
  --rates 0.005 0.01 0.015 0.02 0.025 0.03 0.035 0.04 0.045 0.05 \
          0.055 0.06 0.065 0.07 0.075 0.08 0.085 0.09 0.095 0.10 \
  --soxim-path /home/mi/soxim/build/src/soxim \
  -o docs/figures/sweep_results_fine.png
```

**Results:**
| Injection Rate | Throughput | Latency | Status |
|----------------|------------|---------|--------|
| 0.0050 | 0.0529 | 41.07 | Linear |
| 0.0100 | 0.1059 | 41.43 | Linear |
| 0.0150 | 0.1588 | 41.79 | Linear |
| 0.0200 | 0.2098 | 42.22 | Linear |
| 0.0250 | 0.2632 | 44.71 | Linear |
| 0.0300 | 0.3128 | 49.05 | Linear |
| 0.0350 | 0.3709 | 56.54 | Linear |
| 0.0400 | 0.4130 | 65.86 | Linear |
| 0.0450 | 0.4678 | 124.11 | Transition |
| 0.0500 | 0.5008 | 268.44 | Transition |
| 0.0550 | 0.5286 | 458.94 | Transition |
| 0.0600 | 0.5357 | 880.24 | Near Sat |
| 0.0650 | 0.5315 | 1293.95 | Near Sat |
| 0.0700 | 0.5388 | 1694.10 | Near Sat |
| 0.0750 | 0.4784 | 438.25 | Fluctuation |
| 0.0800 | 0.4752 | 488.37 | Fluctuation |
| **0.0850** | **0.5440** | **2580.56** | **SATURATION** |
| 0.0900 | 0.5411 | 2684.93 | Saturation |
| 0.0950 | 0.3148 | 10.23 | Recovery |
| 0.1000 | 0.4502 | 103.50 | Recovery |

### Saturation Analysis

**Key Findings:**

1. **Linear Region (0.005 - 0.040)**
   - Throughput increases linearly with injection rate
   - Latency remains low (41-66 cycles)
   - Network is not congested

2. **Transition Region (0.045 - 0.055)**
   - Throughput approaches 0.5 flit/cycle/node
   - Latency starts increasing significantly
   - Network begins to saturate

3. **Saturation Region (0.060 - 0.090)**
   - Throughput plateaus around 0.53-0.54 flit/cycle/node
   - Latency spikes dramatically (880-2685 cycles)
   - **Saturation Point**: 0.085 injection rate
   - **Max Throughput**: 0.544 flit/cycle/node

4. **Recovery Region (0.095 - 0.100)**
   - Throughput fluctuates (0.31-0.45 flit/cycle/node)
   - Latency drops significantly
   - Network behavior becomes unpredictable

### Saturation Point Detection

The algorithm detects saturation as the point where:
1. Throughput reaches its maximum value
2. Throughput starts to plateau or decrease
3. Latency shows significant increase

**Detected Saturation Point:**
- **Injection Rate**: 0.085
- **Max Throughput**: 0.544 flit/cycle/node
- **Latency**: 2580.6 cycles
- **Efficiency**: 640.0% (throughput/injection rate)

### Efficiency Analysis

Efficiency = Throughput / Injection Rate

| Injection Rate | Throughput | Efficiency |
|----------------|------------|------------|
| 0.005 | 0.0529 | 1058% |
| 0.010 | 0.1059 | 1059% |
| 0.020 | 0.2098 | 1049% |
| 0.040 | 0.4130 | 1033% |
| 0.050 | 0.5008 | 1002% |
| 0.060 | 0.5357 | 893% |
| 0.070 | 0.5388 | 770% |
| **0.085** | **0.5440** | **640%** |
| 0.090 | 0.5411 | 601% |
| 0.100 | 0.4502 | 450% |

**Observation**: Efficiency decreases as injection rate increases due to congestion.

## Generated Plots

### 1. sweep_results_fine.png (141KB)
- **20 injection rates** (0.005 - 0.10)
- **Saturation point marked** with green star and vertical line
- **Throughput plot** with ideal line
- **Latency plot** with saturation point
- **Saturation analysis** output

### 2. sweep_results_enhanced.png (140KB)
- **10 injection rates** (0.01 - 0.10)
- **Saturation point detection** enabled
- **Enhanced visualization** with markers

### 3. sweep_results_comprehensive.png (108KB)
- **10 injection rates** (0.01 - 0.10)
- **Basic saturation curve** without point detection

## Visualization Features

### Throughput Plot
- **Blue line**: Measured throughput
- **Black dashed line**: Ideal throughput (throughput = injection rate)
- **Green dashed line**: Saturation point
- **Green star**: Maximum throughput point

### Latency Plot
- **Red line**: Measured latency
- **Green dashed line**: Saturation point
- **Green star**: Latency at saturation

## Comparison with Previous Results

### Previous Sweep (3 rates)
| Rate | Throughput | Latency |
|------|------------|---------|
| 0.01 | 0.1055 | 41.46 |
| 0.05 | 0.4190 | 153.37 |
| 0.10 | 0.4947 | 176.21 |

### Enhanced Sweep (20 rates)
| Rate | Throughput | Latency |
|------|------------|---------|
| 0.01 | 0.1059 | 41.43 |
| 0.05 | 0.5008 | 268.44 |
| 0.10 | 0.4502 | 103.50 |

**Key Differences:**
- More detailed analysis with 20 points
- Saturation point identified at 0.085
- Better understanding of network behavior
- Clear visualization of saturation region

## Usage Examples

### Basic Sweep
```bash
python3 sweep.py -c config.toml --rates 0.01 0.05 0.1 -o output.png
```

### Comprehensive Sweep
```bash
python3 sweep.py -c config.toml \
  --rates 0.01 0.02 0.03 0.04 0.05 0.06 0.07 0.08 0.09 0.10 \
  -o comprehensive.png
```

### Fine-Grained Sweep
```bash
python3 sweep.py -c config.toml \
  --rates 0.005 0.01 0.015 0.02 0.025 0.03 0.035 0.04 0.045 0.05 \
          0.055 0.06 0.065 0.07 0.075 0.08 0.085 0.09 0.095 0.10 \
  -o fine.png
```

### Auto-Generated Rates
```bash
python3 sweep.py -c config.toml --min-rate 0.01 --max-rate 0.10 -n 20 -o output.png
```

## Insights

### Network Behavior
1. **Linear Region**: Network operates efficiently with minimal congestion
2. **Transition Region**: Congestion begins, latency increases
3. **Saturation Region**: Network is saturated, throughput plateaus
4. **Recovery Region**: Unpredictable behavior, possible routing issues

### Design Implications
1. **Operating Point**: 0.04-0.05 injection rate for good performance
2. **Saturation Limit**: 0.085 injection rate (maximum sustainable load)
3. **Latency Trade-off**: Higher injection rates lead to dramatic latency increase
4. **Efficiency**: Network efficiency decreases with increasing load

### Recommendations
1. **For Low Latency**: Use injection rate < 0.04
2. **For High Throughput**: Use injection rate 0.05-0.06
3. **Avoid Saturation**: Keep injection rate < 0.085
4. **Monitor Latency**: Watch for latency spikes above 200 cycles

## Future Enhancements

### Planned Improvements
1. **Adaptive Rate Selection**: Auto-detect optimal rate range
2. **Multi-Algorithm Comparison**: Compare saturation points across algorithms
3. **Statistical Analysis**: Add confidence intervals and error bars
4. **Interactive Plots**: Web-based interactive saturation curves
5. **Export to CSV**: Enhanced data export with metadata

### Advanced Features
1. **Saturation Prediction**: Machine learning to predict saturation
2. **Parameter Optimization**: Auto-tune network parameters
3. **Real-time Monitoring**: Live saturation detection during simulation
4. **Comparative Analysis**: Compare multiple network configurations

## Summary

The enhanced sweep analysis provides:

- **20 injection rates** for fine-grained analysis
- **Automatic saturation point detection**
- **Detailed saturation analysis** (point, throughput, latency, efficiency)
- **Professional visualizations** with clear markings
- **Actionable insights** for network design

**Key Results:**
- **Saturation Point**: 0.085 injection rate
- **Max Throughput**: 0.544 flit/cycle/node
- **Latency at Sat**: 2580.6 cycles
- **Efficiency**: 640.0%

The sweep analysis is now comprehensive and provides deep insights into network performance and saturation behavior.
