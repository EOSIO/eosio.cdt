#pragma once
#include <algorithm>
#include <string>
#include <regex>

namespace blanc {
   /**
    * @brief Remove double quotes from the string
    *
    * @return A string without quotes
    * @param str A string from which quotes are removed
    */
   std::string remove_quotes(std::string str) {
      str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
      //str.erase(std::remove(str.begin(), str.end(), '\''), str.end());
      return str;
   }

   /**
    * @brief Tokenize compiler command or custom attributes
    *
    * Tokenize compiler commands or custom attributes delivered by 
    * clang::annotation attribute. Comma and whitespace are considered
    * as delimeter. (Repeated whitespaces are counted by one.)
    *
    * @return A vector of token strings
    *
    * @param str A string to be tokenized
    * @param delim A delimiter regex
    */
   std::vector<std::string> tokenize(std::string str, std::string delim = R"([\s,]+)") {
      auto re = std::regex(delim);
      return std::vector<std::string>{
         std::sregex_token_iterator(str.begin(), str.end(), re, -1),
         std::sregex_token_iterator()
      };
   }
}
