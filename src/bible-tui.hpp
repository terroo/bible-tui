#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <array>
#include <memory>
#include "getverse.hpp"
#include "unicode-theme.hpp"
#include "start-def.hpp"

class BibleTUI : public GetVerse {

  static constexpr int MAX_WIDTH = 55;
  static constexpr int TEXT_OFFSET = 2;

  std::unique_ptr<start_def> m_defs;
   
  std::array<std::array<std::string, 6>, COUNT> unicodes;
  std::array<std::string, 6> unicode;
  void set_unicodes();

  std::array<std::array<std::string, 3>, STYLE> themes;
  std::array<std::string, 3> theme;
  void set_theme();

  size_t utf8_length(const std::string& str);
  std::vector<std::string> wrap_text(const std::string& text, int width);
  std::string pad_line(const std::string& line, int width, const std::string& content_color);
  std::string make_border(const std::string& left, const std::string& fill, 
      const std::string& right, int count);

  public:
    BibleTUI(const start_def& defs);
    void run();
};
