#pragma once

#include <iostream>

#define ASSERT(Condition, Text)                                                \
  if (!(Condition)) {                                                          \
    std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ": " \
              << Text << "\n";                                                 \
    std::abort();                                                              \
  }
