#include "Utility.h"    

// strip an HTTP packet of its headers and extraneous data (quick & dirty)
std::string Utility::strip_to_json(std::string response) {
    std::size_t first_bracket = response.find_first_of("{");
    std::size_t last_bracket = response.find_last_of("}");
    return response.substr(first_bracket, last_bracket);
}
