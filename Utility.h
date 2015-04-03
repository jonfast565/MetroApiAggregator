#ifndef UTILITY_H
#define	UTILITY_H

// MongoDB includes
#include <bson.h>

// string exception type
struct ApiException : public std::exception
{
   std::string s;
   ApiException(std::string ss) : s(ss) {}
   ~ApiException() throw () {}
   const char* what() const throw() { return s.c_str(); }
};

namespace Utility {
    std::string strip_to_json(std::string response);
}

#endif	/* UTILITY_H */

