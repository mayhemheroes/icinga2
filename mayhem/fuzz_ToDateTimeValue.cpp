#include <stdint.h>
#include <stdio.h>
#include <climits>

#include <fuzzer/FuzzedDataProvider.h>
#include "convert.hpp"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    FuzzedDataProvider provider(data, size);
    double x = provider.ConsumeFloatingPoint<double>();
    icinga::Convert::ToDateTimeValue(x);
    return 0;
}