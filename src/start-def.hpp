#pragma once

#include <string>
#include "unicode-theme.hpp"

typedef struct START {
  std::string lang;
  THEMES theme;
  UNICODE unicode;
  std::string book;
} start_def;
