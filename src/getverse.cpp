#include "getverse.hpp"

GetVerse::GetVerse(const std::string& lang, const std::string& book) : lang(lang), book(book){
  en = "https://bible-api.com/";
  br = "?translation=almeida";
}

std::string GetVerse::format(const std::string& str){
  size_t start = 0;
  if (str.compare(0, 3, "pt+") == 0) start = 3;

  std::string ret;
  ret.reserve(str.size() - start);

  for(size_t i = start; i < str.size(); ++i){
    char c = str[i];
    if(c == '/'){
      ret += ':';
    }else{
      ret += c;
    }
  }

  return ret;
}

std::string GetVerse::format_book(const std::string& str){
  size_t start = 0;
  if (str.compare(0, 3, "pt+") == 0) start = 3;

  std::string ret;
  ret.reserve(str.size() - start);

  for(size_t i = start; i < str.size(); ++i){
    char c = str[i];
    if(c == ':'){
      ret += '+';
    }else{
      ret += c;
    }
  }

  std::string ret2 = {};
  for(size_t i = start; i < str.size(); ++i){
    char c = ret[i];
    if(c == '-'){
      ret2 += ':';
    }else{
      ret2 += c;
    }
  }

  return ret2;
}

void GetVerse::run_verse(){
  try {
    curlpp::Cleanup cleanup;
    std::ostringstream os;

    curlpp::Easy request1;

    if(lang == "en"){
      request1.setOpt(curlpp::options::Url(en_random));
    }else{
      request1.setOpt(curlpp::options::Url(br_random));
    }
    request1.setOpt(curlpp::options::WriteStream(&os));
    request1.perform();

    std::string html = os.str();

    std::string search = "<div class=\"vr\"><a href=\"";
    size_t pos = html.find(search);
    if (pos == std::string::npos) {
      std::cerr << "Failure to find verse.\n";
      std::exit(1);
    }
    pos += search.size();

    size_t end = html.find("\"", pos);
    if (end == std::string::npos) {
      std::cerr << "Problem when generating the verse.\n";
      std::exit(1);
    }
    std::string href = html.substr(pos, end - pos);
    while (!href.empty() && !std::isalpha(href[0])) {
      href.erase(0,1);
    }

    size_t first_slash = href.find('/');
    if (first_slash != std::string::npos) {
      href[first_slash] = '+';
      size_t second_slash = href.find('/', first_slash + 1);
      if(second_slash != std::string::npos){
        if(lang == "en"){
          href[second_slash] = ':';
        }else{
          href[second_slash] = '+';
        }
      }
    }

    std::string api = en + format(href);

    if(!book.empty()){
      book = format_book(book);
      api = en + book;
    }

    if(lang != "en"){
      api += br;
    }

    os.str("");
    os.clear();
    curlpp::Easy request2;
    request2.setOpt(curlpp::options::Url(api));
    request2.setOpt(curlpp::options::WriteStream(&os));
    request2.perform();

    html = os.str();
    nlohmann::json json;
    std::istringstream iss(html);
    iss >> json;

    if(json["reference"].empty()){
      std::cout << "Try again.\n";
      std::exit(1);
    }

    this->verse = json["text"].get<std::string>();
    this->capt  = json["reference"].get<std::string>();

    std::replace(verse.begin(), verse.end(), '\n', ' ');
    std::replace(verse.begin(), verse.end(), '\r', ' ');

  } catch (curlpp::RuntimeError &e) {
    std::cerr << "Erro runtime: " << e.what() << "\n";
  } catch (curlpp::LogicError &e) {
    std::cerr << "Erro lógica: " << e.what() << "\n";
  }
}
