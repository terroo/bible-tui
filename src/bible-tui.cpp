#include "bible-tui.hpp"

BibleTUI::BibleTUI(const start_def& defs) : GetVerse(defs.lang, defs.book){
  m_defs          = std::make_unique<start_def>();

  m_defs->lang    = defs.lang;
  m_defs->unicode = defs.unicode;
  m_defs->theme   = defs.theme;
  m_defs->book    = defs.book;

  set_unicodes();
  set_theme();
  this->run_verse();
}

size_t BibleTUI::utf8_length(const std::string& str){
  size_t len = 0;
  for(char c : str){
    if((static_cast<unsigned char>(c) & 0b11000000) != 0b10000000){
      ++len;
    }
  }
  return len;
}

std::vector<std::string> BibleTUI::wrap_text(const std::string& text, int width){
  std::vector<std::string> lines;
  size_t start = 0;

  while(start < text.size()){
    size_t end = start + static_cast<size_t>(width);
    if(end >= text.size()){
      lines.push_back(text.substr(start));
      break;
    }

    size_t space_pos = text.rfind(' ', end);
    if(space_pos == std::string::npos || space_pos < start){
      space_pos = end;
    }

    lines.push_back(text.substr(start, space_pos - start));
    start = (text[space_pos] == ' ') ? space_pos + 1 : space_pos;
  }

  return lines;
}

std::string BibleTUI::pad_line(const std::string& line, int width, 
    const std::string& content_color){
  size_t visual_len = utf8_length(line);
  std::string padding(static_cast<size_t>(width) - visual_len + TEXT_OFFSET, ' ');
  return "  " + theme[0] + unicode[3] + "  " + content_color + line + padding + theme[0] + "  " + unicode[3];
}

std::string BibleTUI::make_border(const std::string& left, const std::string& fill, 
    const std::string& right, int count){
  std::string result = "  " + left;
  for (int i = 0; i < count; ++i){
    result += fill;
  }
  result += right;
  return result;
}

void BibleTUI::run(){

  auto wrapped = wrap_text(verse, MAX_WIDTH);
  int border_fill_count = MAX_WIDTH + 6;

  std::string top_border    = make_border(unicode[0], unicode[1], unicode[2], border_fill_count);
  std::string bottom_border = make_border(unicode[4], unicode[1], unicode[5], border_fill_count);
  std::string empty_line    = pad_line("", MAX_WIDTH, theme[0]);

  std::cout.put('\n'); // START
  std::cout << theme[0];

  std::cout << top_border << '\n';
  std::cout << empty_line << '\n';

  for (const auto& line : wrapped)
    std::cout << pad_line(line, MAX_WIDTH, theme[1]) << '\n';


  std::cout << empty_line << '\n';

  std::cout << pad_line("— " + capt, MAX_WIDTH, theme[2]) << '\n';
  std::cout << empty_line << '\n';
  std::cout << bottom_border << '\n';

  std::cout << "\033[m";
  std::cout.put('\n'); // OK
}

void BibleTUI::set_unicodes(){
  unicodes = {{
    {"┌", "─", "┐", "│", "└", "┘"}, // DEFAULT
    {"╔", "═", "╗", "║", "╚", "╝"}, // DOUBLE
    {"╭", "─", "╮", "│", "╰", "╯"}, // ROUNDED
    {" ", " ", " ", " ", " ", " "}  // EMPTY
  }};

  if(m_defs->unicode == MOLD_INVALID) m_defs->unicode = ROUNDED;
  unicode = unicodes[m_defs->unicode];
}

void BibleTUI::set_theme(){
  themes = {{
    {"\033[38;2;200;200;68m", "\033[1;38;2;166;226;46m", "\033[3;38;2;117;113;94m"}, // real
    {"\033[38;2;90;90;90m", "\033[1;38;2;240;83;137m", "\033[3;38;2;158;95;212m"}, // lolcat
    {"\033[38;2;255;146;41m", "\033[1;38;2;169;177;189m", "\033[3;38;2;208;153;102m"}, // atom
  }};
  if(m_defs->theme == THEME_INVALID) m_defs->theme = LOLCAT;
  theme = themes[m_defs->theme];
}
