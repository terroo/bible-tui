#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>
#include <nlohmann/json.hpp>
#include "start-def.hpp"

class GetVerse {

  const std::string br_random = "https://dailyverses.net/pt/versiculo-aleatorio-da-biblia";
  const std::string en_random = "https://dailyverses.net/random-bible-verse";

  protected:
    GetVerse(const std::string& lang, const std::string& book);
    void run_verse();
    std::string format(const std::string& str);
    std::string br, en, lang, book;

  public:
    std::string verse, capt;
};
