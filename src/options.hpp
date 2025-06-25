#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>
#include "unicode-theme.hpp"
#include "bible-tui.hpp"

class Options {
  //std::unique_ptr<start_def> defs_options = nullptr;
  const std::string VERSION = "bible-tui v1.0";
  public:
    Options();
    THEMES parse_theme(const std::string& value);
    UNICODE parse_mold(const std::string& value);
    bool validate_book_format(const std::string& book);
    void run_options(int, char**);
    void help();
};

