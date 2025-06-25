#include "options.hpp"

Options::Options(){}

THEMES Options::parse_theme(const std::string& value){
  static const std::unordered_map<std::string, THEMES> theme_map = {
    {"real", REAL},
    {"lolcat", LOLCAT},
    {"atom", ATOM},
    {"style", STYLE}
  };
  auto it = theme_map.find(value);
  return it != theme_map.end() ? it->second : THEME_INVALID;
}

UNICODE Options::parse_mold(const std::string& value){
  static const std::unordered_map<std::string, UNICODE> mold_map = {
    {"default", DEFAULT},
    {"double", DOUBLE},
    {"rounded", ROUNDED},
    {"empty", EMPTY}
  };
  auto it = mold_map.find(value);
  return it != mold_map.end() ? it->second : MOLD_INVALID;
}

bool Options::validate_book_format(const std::string& book){
  auto colon = book.find(':');
  auto dash = book.find('-');
  if(colon == std::string::npos || dash == std::string::npos || dash <= colon + 1) return false;

  std::string nome = book.substr(0, colon);
  std::string num1 = book.substr(colon + 1, dash - colon - 1);
  std::string num2 = book.substr(dash + 1);

  if(nome.empty() || num1.empty() || num2.empty()) return false;

  for(char c : nome){
    if (!std::isalpha(c)) return false;
  }

  for(char c : num1 + num2){
    if (!std::isdigit(c)) return false;
  }

  return true;
}

void Options::run_options(int argc, char** argv){
  std::string locale = std::getenv("LANG");
  std::string lang = locale.substr(0, 2);
  THEMES theme = THEME_INVALID;
  UNICODE mold = MOLD_INVALID;
  std::string book;

  for(int i = 1; i < argc; ++i){
    std::string arg = argv[i];

    if(arg.rfind("--lang=", 0) == 0){
      lang = arg.substr(7);
      if(lang != "en" && lang != "pt"){
        std::cerr << "Invalid language: " << lang << "\n";
        std::exit(1);
      }
    }else if (arg.rfind("--theme=", 0) == 0){
      std::string value = arg.substr(8);
      theme = parse_theme(value);
      if(theme == THEME_INVALID){
        std::cerr << "Invalid theme: " << value << "\n";
        std::exit(1);
      }
    }else if (arg.rfind("--mold=", 0) == 0){
      std::string value = arg.substr(7);
      mold = parse_mold(value);
      if (mold == MOLD_INVALID) {
        std::cerr << "Invalid mold: " << value << "\n";
        std::exit(1);
      }
    }else if(arg.rfind("--book=", 0) == 0){
      std::string value = arg.substr(7);
      if (!validate_book_format(value)) {
        std::cerr << "Invalid format for --book. Use book:number-number, ex.: psalms:23-1\n";
        std::exit(1);
      }
      book = value;
    }else if(arg == "--help" || arg == "-h"){
      help();
      std::exit(0);
    }else if(arg == "--version" || arg == "-v"){
      std::cout << VERSION << '\n';
      std::exit(0);
    }else {
      std::cerr << "Unrecognized argument: " << arg << "\n";
      std::exit(1);
    }
  }

  start_def defs = {lang, theme, mold, book};

  auto bible = std::make_unique<BibleTUI>(defs);
  bible->run();
}

void Options::help() {
  std::cout << "Usage:\n"
    << "bible-tui [options]\n\n"
    << "Options:\n"
    << "  --lang=pt|en                        Set the language.\n"
    << "  --theme=real|lolcat|atom|style      Set the color theme.\n"
    << "  --mold=default|double|rounded|empty Set the border style.\n"
    << "  --book=name:num-num                 Set the book and verse.\n"
    << "                                         Examples:\n"
    << "                                           [EN]psalms:23-1 | [PT]salmos:23-1\n"
    << "  --help, -h                          Show this message.\n"
    << "  --version, -v                       Show version info.\n";
}
