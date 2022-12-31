#ifndef MEASURING_HPP_
#define MEASURING_HPP_

#include "runtime_measurement/instrumentation_profiler.hpp"
#include "runtime_measurement/timer.hpp"

#define MEASURE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define MEASURE_FUNCTION() MEASURE_SCOPE(__PRETTY_FUNCTION__)

#endif // MEASURING_HPP_
