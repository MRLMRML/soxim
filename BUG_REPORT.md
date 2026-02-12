# SOXIM Bug Report - Network Instability Analysis

## 🐛 Critical Bugs Found

### Bug 1: Packet Generation Logic Error

**Location:** `src/TrafficOperator.cpp` lines 63-100

**Issue:** The packet generation loop has incorrect logic that causes:
1. **Exponential packet generation** - Each packetID generates packets for ALL sources
2. **Massive over-injection** - Network is flooded with packets

**Current Code:**
```cpp
for (int packetID{}; packetID < g_packetNumber; ++packetID)
{
    for (int source{ -1 }; source >= -m_network->getRouterNumber(); --source)
    {
        // Generate packet for this source
    }
}
```

**Problem:** 
- If `g_packetNumber = 100` and `g_routerNumber = 64`
- This generates `100 * 64 = 6,400` packets
- But the calculation `g_packetNumber = g_totalCycles * g_injectionRate` expects only 100 packets

**Example:**
- `g_totalCycles = 10000`
- `g_injectionRate = 0.01`
- `g_packetNumber = 10000 * 0.01 = 100`
- **Expected:** 100 packets total
- **Actual:** 100 * 64 = 6,400 packets (64x over-injection!)

### Bug 2: Type Mismatch

**Location:** `src/Parameters.h` line 28

**Issue:** `g_packetNumber` is declared as `float` but used as `int`

**Current Code:**
```cpp
inline float g_packetNumber{};  // In Parameters.h
for (int packetID{}; packetID < g_packetNumber; ++packetID)  // In TrafficOperator.cpp
```

**Problem:** 
- Float comparison in loop can cause unexpected behavior
- Should be `int` for packet counting

### Bug 3: Injection Rate Calculation Error

**Location:** `src/main.cpp` lines 309, 331

**Issue:** `g_packetNumber = g_totalCycles * g_injectionRate` is incorrect

**Problem:**
- This calculates total packets over ALL cycles
- But injection rate is per-cycle per-node
- Should be: `g_packetNumber = g_totalCycles * g_injectionRate * g_routerNumber`

**Example:**
- `g_totalCycles = 10000`
- `g_injectionRate = 0.01` (1% per cycle per node)
- `g_routerNumber = 64`
- **Expected:** `10000 * 0.01 * 64 = 6,400` packets
- **Actual:** `10000 * 0.01 = 100` packets

### Bug 4: Packet Distribution Error

**Location:** `src/TrafficOperator.cpp` lines 63-100

**Issue:** Packets are distributed across ALL sources for each packetID

**Current Logic:**
```
packetID 0: Generate packets for source -1, -2, -3, ..., -64
packetID 1: Generate packets for source -1, -2, -3, ..., -64
...
packetID 99: Generate packets for source -1, -2, -3, ..., -64
```

**Expected Logic:**
```
packetID 0: Generate packet for source -1
packetID 1: Generate packet for source -2
...
packetID 63: Generate packet for source -64
packetID 64: Generate packet for source -1
...
```

### Bug 5: Clock Randomization in TerminalInterface

**Location:** `src/TerminalInterface.cpp` lines 12-18

**Issue:** Each terminal interface has a random clock offset

**Current Code:**
```cpp
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, 10);
m_clock.set(static_cast<float>(dist(gen)));
```

**Problem:**
- Each router starts at a different cycle
- Causes desynchronization
- Leads to unpredictable behavior

### Bug 6: Random Number Generation in Injection Process

**Location:** `src/TerminalInterface.cpp` lines 58-75

**Issue:** Random number generator is recreated every cycle

**Current Code:**
```cpp
void TerminalInterface::injectTraffic()
{
    std::random_device rd;
    std::mt19937 gen(rd());  // New generator every cycle!
    std::bernoulli_distribution distBernoulli(g_injectionRate);
    ...
}
```

**Problem:**
- Creates new random generator every cycle
- Leads to poor randomness
- Causes unpredictable injection patterns

## 📊 Impact Analysis

### Network Instability Causes

1. **Massive Over-injection** (Bug 1)
   - 64x more packets than expected
   - Network immediately saturated
   - Causes throughput collapse

2. **Desynchronization** (Bug 5)
   - Routers start at different times
   - Causes timing issues
   - Leads to unpredictable behavior

3. **Poor Randomness** (Bug 6)
   - Unpredictable injection patterns
   - Causes bursty traffic
   - Leads to latency spikes

### Observed Symptoms

**From sweep analysis:**
- Throughput peaks at 0.085 injection rate (0.544 flit/cycle/node)
- Then drops to 0.31-0.45 flit/cycle/node
- Latency spikes to 2580+ cycles
- Recovery region shows unpredictable behavior

**Root Cause:** The network is receiving 64x more packets than expected!

## 🔧 Recommended Fixes

### Fix 1: Correct Packet Generation Logic

**Change in `src/TrafficOperator.cpp`:**

```cpp
// OLD CODE (BUGGY)
for (int packetID{}; packetID < g_packetNumber; ++packetID)
{
    for (int source{ -1 }; source >= -m_network->getRouterNumber(); --source)
    {
        // Generate packet for this source
    }
}

// NEW CODE (CORRECT)
for (int source{ -1 }; source >= -m_network->getRouterNumber(); --source)
{
    for (int packetID{}; packetID < g_packetNumber; ++packetID)
    {
        // Generate packet for this source
    }
}
```

### Fix 2: Change g_packetNumber to int

**Change in `src/Parameters.h`:**

```cpp
// OLD CODE
inline float g_packetNumber{};

// NEW CODE
inline int g_packetNumber{};
```

### Fix 3: Correct Packet Number Calculation

**Change in `src/main.cpp`:**

```cpp
// OLD CODE
g_packetNumber = g_totalCycles * g_injectionRate;

// NEW CODE
g_packetNumber = g_totalCycles * g_injectionRate * g_routerNumber;
```

### Fix 4: Remove Clock Randomization

**Change in `src/TerminalInterface.cpp`:**

```cpp
// OLD CODE
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(0, 10);
m_clock.set(static_cast<float>(dist(gen)));

// NEW CODE
m_clock.set(0);
```

### Fix 5: Use Static Random Generator

**Change in `src/TerminalInterface.cpp`:**

```cpp
// OLD CODE
void TerminalInterface::injectTraffic()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution distBernoulli(g_injectionRate);
    ...
}

// NEW CODE
class TerminalInterface {
private:
    static std::mt19937 gen;
    static std::random_device rd;
};

// In TerminalInterface.cpp
std::mt19937 TerminalInterface::gen(rd());
std::random_device TerminalInterface::rd;

void TerminalInterface::injectTraffic()
{
    std::bernoulli_distribution distBernoulli(g_injectionRate);
    ...
}
```

## 📈 Expected Impact

### Before Fixes
- **Packet Generation:** 64x over-injection
- **Network State:** Immediately saturated
- **Throughput:** Unstable, drops after saturation
- **Latency:** Spikes to 2500+ cycles
- **Behavior:** Unpredictable

### After Fixes
- **Packet Generation:** Correct injection rate
- **Network State:** Stable operation
- **Throughput:** Smooth saturation curve
- **Latency:** Predictable increase
- **Behavior:** Stable and predictable

## 🧪 Testing Plan

### Unit Tests
1. Verify packet generation count
2. Verify injection rate accuracy
3. Verify clock synchronization
4. Verify random number generation

### Integration Tests
1. Run sweep with corrected parameters
2. Verify saturation curve is smooth
3. Verify latency behavior is predictable
4. Verify throughput matches theoretical values

### Regression Tests
1. Run all existing unit tests
2. Verify no new bugs introduced
3. Verify backward compatibility

## 📝 Summary

**Critical Bugs Found:**
1. ✗ Packet generation logic (64x over-injection)
2. ✗ Type mismatch (float vs int)
3. ✗ Injection rate calculation (missing router count)
4. ✗ Packet distribution (wrong loop structure)
5. ✗ Clock randomization (desynchronization)
6. ✗ Random generator recreation (poor randomness)

**Impact:** Network instability, unpredictable behavior, incorrect results

**Fix Priority:** CRITICAL - Must fix before production use

**Estimated Fix Time:** 2-3 hours

**Testing Time:** 1-2 hours

**Total Time:** 3-5 hours
