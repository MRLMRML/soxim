#pragma once
#include <string_view>

#define DEBUG 0
// 0 off
// 1 Router and Port
#define BENCHMARK 1
#define REPRODUCE_RANDOM 1
#define MAGIC_NUMBER 42

inline int g_x{}, g_y{}, g_z{};
inline std::string_view g_shape{};
inline std::string_view g_routingAlgorithm{};
inline int g_virtualChannelNumber{};
inline int g_bufferSize{};
inline int g_flitSize{};
inline int g_packetSize{};
inline std::string_view g_packetSizeOption{};
inline float g_injectionRate{};
inline std::string_view g_injectionProcess{};
inline float g_alpha{};
inline float g_beta{};
inline std::string_view g_trafficPattern{};
inline int g_totalCycles{};
inline int g_warmupCycles{};
inline int g_measurementCycles{};
inline int g_drainCycles{};
inline float g_packetNumber{};