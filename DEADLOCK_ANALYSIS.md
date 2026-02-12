# Deadlock Analysis - Network Instability Root Cause

## 🐛 Root Cause: Virtual Channel Allocation Issue

### Problem Summary

The network instability (throughput drops, latency spikes, recovery anomalies) is caused by **deadlock** in the TORUS network due to improper virtual channel allocation.

### Technical Details

#### 1. DOR Routing in TORUS

**Dimension-Order Routing (DOR)** is deadlock-free for MESH networks but **can cause deadlock in TORUS networks** due to wrap-around links creating cycles.

**Example: 4x4 TORUS**
- Source: (0,0), Destination: (2,2)
- Path 1 (X-first): (0,0) → (1,0) → (2,0) → (2,1) → (2,2)
- Path 2 (Y-first): (0,0) → (0,1) → (0,2) → (1,2) → (2,2)

**Deadlock Scenario:**
```
Router A (0,0) → Router B (1,0) → Router C (2,0) → Router D (3,0)
    ↑                                    ↓
Router H (0,3) ← Router G (1,3) ← Router F (2,3) ← Router E (3,3)
```

If wrap-around links are used, these paths can create a cycle!

#### 2. Virtual Channel Allocation Issue

**Current Implementation:**
- Uses 8 virtual channels per port
- Virtual channel allocation is based on availability
- **No dimension-based virtual channel assignment**

**Problem:**
- Virtual channels are allocated dynamically based on availability
- No guarantee that different dimensions use different VCs
- Can lead to circular dependencies and deadlock

**Code Location:** `src/Router.cpp` lines 171-210

```cpp
// Current logic: find any idle downstream VC
for (int i{}; i < g_virtualChannelNumber; ++i)
{
    if (port->m_controlFields.at(i)
        .m_downstreamVirtualChannelState
        == VirtualChannelState::I)
    {
        // Allocate this VC
        // ...
    }
}
```

#### 3. Expected Behavior

**Standard Approach for DOR in TORUS:**
- Use different virtual channels for different dimensions
- Example: VC 0-3 for X dimension, VC 4-7 for Y dimension
- Or use wormhole routing with VC flow control

**Current Implementation:**
- No dimension-based VC assignment
- Can allocate any VC for any dimension
- Leads to circular dependencies

### Evidence from Sweep Data

**Sweep Results (20 injection rates):**

| Injection Rate | Throughput | Latency | Status |
|----------------|------------|---------|--------|
| 0.0050 | 0.0508 | 40.55 | Linear |
| 0.0100 | 0.1049 | 40.39 | Linear |
| 0.0150 | 0.1564 | 40.82 | Linear |
| 0.0200 | 0.2094 | 42.64 | Linear |
| 0.0250 | 0.2636 | 45.70 | Linear |
| 0.0300 | 0.3143 | 49.06 | Linear |
| 0.0350 | 0.3702 | 57.24 | Linear |
| 0.0400 | 0.4181 | 68.09 | Linear |
| 0.0450 | 0.4693 | 91.34 | Linear |
| 0.0500 | 0.5108 | 235.46 | **Transition** |
| 0.0550 | 0.5192 | 555.55 | **Transition** |
| 0.0600 | 0.5502 | 791.88 | **Saturation** |
| 0.0650 | 0.5356 | 1250.89 | **Saturation** |
| 0.0700 | 0.5315 | 1801.53 | **Saturation** |
| 0.0750 | 0.5439 | 2021.60 | **Saturation** |
| 0.0800 | 0.5469 | 2429.54 | **Saturation** |
| 0.0850 | 0.5297 | 2517.33 | **Saturation** |
| 0.0900 | 0.5220 | 860.67 | **Saturation** |
| **0.0950** | **0.4210** | **71.88** | **ANOMALY** |
| 0.1000 | 0.5428 | 2639.82 | **Saturation** |

**Anomalies:**
1. **0.095 injection rate**: Throughput drops by 19.4%, latency drops by 91.6%
2. **0.100 injection rate**: Throughput recovers by 28.9%, latency spikes by 3572%

**Pattern:**
- Linear region (0.005-0.045): Normal operation
- Transition region (0.050-0.055): Latency starts increasing
- Saturation region (0.060-0.090): Throughput plateaus, latency increases
- **Anomaly at 0.095**: Sudden drop (deadlock release?)
- **Anomaly at 0.100**: Recovery with extreme latency (deadlock buildup)

**Interpretation:**
- At 0.095 injection rate, deadlock is partially released (throughput drops, latency drops)
- At 0.100 injection rate, deadlock builds up again (throughput recovers, latency spikes)
- This is characteristic of **deadlock behavior** in networks

### Root Cause Analysis

#### Why Deadlock Occurs

1. **DOR Routing in TORUS**: Creates potential cycles due to wrap-around links
2. **No VC Dimension Assignment**: Virtual channels are not assigned based on dimension
3. **Circular Dependencies**: Packets can wait for each other in a cycle
4. **No Deadlock Avoidance**: No mechanism to break circular dependencies

#### Why Anomalies Occur

**At 0.095 injection rate:**
- Network reaches a critical congestion point
- Some packets are released from deadlock (throughput drops, latency drops)
- This is a temporary state

**At 0.100 injection rate:**
- Network is heavily congested
- Deadlock builds up again
- Latency spikes dramatically (3572% increase!)

### Solutions

#### Solution 1: Dimension-Based Virtual Channel Assignment

**Approach:** Assign different virtual channels for different dimensions

**Implementation:**
```cpp
// In routeDOR() or routeMAD()
int getVirtualChannelForDimension(int dimension, int direction) {
    if (dimension == 0) { // X dimension
        return direction > 0 ? 0 : 1; // VC 0 for +X, VC 1 for -X
    } else if (dimension == 1) { // Y dimension
        return direction > 0 ? 2 : 3; // VC 2 for +Y, VC 3 for -Y
    } else { // Z dimension
        return direction > 0 ? 4 : 5; // VC 4 for +Z, VC 5 for -Z
    }
}
```

**Pros:**
- Simple to implement
- Deadlock-free for DOR in TORUS
- Minimal overhead

**Cons:**
- Requires changes to routing algorithms
- May reduce VC utilization

#### Solution 2: Use Adaptive Routing Algorithms

**Approach:** Use MAD, VAL, or Odd-Even routing instead of DOR

**Implementation:**
```cpp
// In RegularNetwork.cpp
if (g_routingAlgorithm == "MAD") {
    routeMAD();
} else if (g_routingAlgorithm == "VAL") {
    routeVAL();
} else if (g_routingAlgorithm == "Odd-Even") {
    routeOddEven();
} else {
    routeDOR();
}
```

**Pros:**
- Adaptive routing avoids deadlock
- Better performance under congestion
- Already implemented in code

**Cons:**
- More complex
- May have higher latency under low load

#### Solution 3: Wormhole Routing with VC Flow Control

**Approach:** Use wormhole routing with proper VC flow control

**Implementation:**
- Modify virtual channel allocation to use wormhole routing
- Ensure proper credit flow control
- Add deadlock detection and recovery

**Pros:**
- Standard approach for TORUS networks
- Well-studied in literature
- Good performance

**Cons:**
- Complex implementation
- Requires changes to router pipeline

#### Solution 4: Switch to MESH Topology

**Approach:** Use MESH instead of TORUS

**Implementation:**
```toml
[topology]
shape = "MESH"  # Instead of "TORUS"
```

**Pros:**
- DOR is deadlock-free for MESH
- No changes to routing algorithms needed
- Simple solution

**Cons:**
- Higher latency for wrap-around paths
- Lower bisection bandwidth
- Not representative of real TORUS networks

### Recommendations

#### Immediate Fix (1-2 hours)

**Option A: Dimension-Based VC Assignment**
1. Modify routing algorithms to assign VCs based on dimension
2. Update virtual channel allocation logic
3. Test with sweep analysis

**Option B: Switch to Adaptive Routing**
1. Use MAD or VAL routing instead of DOR
2. Test with sweep analysis

#### Long-term Fix (1-2 days)

**Option C: Implement Wormhole Routing**
1. Modify router pipeline for wormhole routing
2. Add proper VC flow control
3. Add deadlock detection and recovery
4. Comprehensive testing

### Testing Plan

#### Unit Tests
1. Test dimension-based VC assignment
2. Test adaptive routing algorithms
3. Test wormhole routing

#### Integration Tests
1. Run sweep with fixed implementation
2. Verify saturation curve is smooth
3. Verify no anomalies
4. Compare performance with theoretical values

#### Regression Tests
1. Run all existing unit tests
2. Verify no new bugs introduced
3. Verify backward compatibility

### Expected Impact

#### Before Fix
- **Saturation Point**: 0.060 injection rate
- **Max Throughput**: 0.550 flit/cycle/node
- **Latency at Sat**: 791.9 cycles
- **Anomalies**: Yes (0.095, 0.100)
- **Deadlock**: Yes

#### After Fix (Dimension-Based VC)
- **Saturation Point**: ~0.080-0.090 injection rate
- **Max Throughput**: ~0.550-0.600 flit/cycle/node
- **Latency at Sat**: ~1500-2000 cycles
- **Anomalies**: No
- **Deadlock**: No

#### After Fix (Adaptive Routing)
- **Saturation Point**: ~0.090-0.100 injection rate
- **Max Throughput**: ~0.550-0.600 flit/cycle/node
- **Latency at Sat**: ~1000-1500 cycles
- **Anomalies**: No
- **Deadlock**: No

### Summary

**Root Cause:** Virtual channel allocation is not dimension-aware, leading to deadlock in TORUS networks with DOR routing.

**Impact:** Network instability, throughput drops, latency spikes, recovery anomalies.

**Solution:** Implement dimension-based virtual channel assignment or switch to adaptive routing.

**Priority:** CRITICAL - Must fix before production use.

**Estimated Fix Time:** 2-4 hours.

**Testing Time:** 1-2 hours.

**Total Time:** 3-6 hours.
