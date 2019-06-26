/*
 * putils.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: pnichols
 */

#ifndef PUTILS_HPP_
#define PUTILS_HPP_
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <climits>
#include <cfloat>
#include <exception>
#include <stdexcept>
#include <cerrno>
#include <cfloat>
#include <climits>
using namespace std;


inline void *Malloc(size_t nbytes)
{
    void * ptr = malloc(nbytes);
    if (ptr) return ptr;
    fprintf(stderr,"Malloc failed to allocate %lu bytes\n",nbytes);
    exit(EXIT_FAILURE);
}

namespace putils {

//!
//! @brief parse error exception class
//!
struct ParseError: public exception {
    ParseError(const string& msg):w(msg) {};
    ParseError():w("ParseError exception") {};
    virtual ~ParseError() throw() {};
    virtual const char * what() const throw()
    {
        return w.c_str();
    };
    string w;
};

//!
//! @brief putils specific error exception class
//!
struct PutilsError: public exception {
    PutilsError(const string& msg):w(msg) {};
    PutilsError():w("PutilsError exception") {};
    virtual ~PutilsError() throw() {};
    virtual const char * what() const throw()
    {
        return w.c_str();
    };
    string w;
};

//!
//! @brief system errors that set errno exception class
//!
struct SystemError: public exception {
    SystemError(const string& msg):w(msg) {};
    SystemError():w("SystemError exception") {};
    SystemError(int error_code):w("SystemError Exception ")
    {
        w += strerror(error_code);
    };
    SystemError(const string& msg,int error_code):w(msg)
    {
        w += " error ";
        w += string(strerror(error_code));
    };
    virtual ~SystemError() throw() {};
    virtual const char * what() const throw()
    {
        return w.c_str();
    };
    string w;
};

/////////////////////////////////////////////////////////////////////////////////////////
// split a string into separate substring by separating at characters given by
//  delimiters.
//////////////////////////////////////////////////////////////////////////////////////////
inline size_t splitString( const string& str_in, const string& delimiters, vector<string>& tokens)
{
    size_t ntokens(0);
    tokens.clear();
    if (str_in.size()==0) return 0;
    size_t first=str_in.find_first_not_of(delimiters,0);
    size_t last=str_in.find_first_of(delimiters,first);
    while (first!=string::npos) {
        ++ntokens;
        tokens.push_back( str_in.substr( first, last-first) );
        first=str_in.find_first_not_of(delimiters,last);
        last=str_in.find_first_of(delimiters,first);
    }
    return ntokens;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////

//!
//!  \brief convert string into a definite type
//!
template<typename T> inline T string2type(const string& str)
{
    T x;
    try {
        istringstream is(str);
        is >> x;
        return x;
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str << endl;
        throw ParseError();
    }
}
//!
//!  \brief convert string into an int
//!
template<> inline  int string2type<int>(const string& str)
{
    try {
        const char * arg = str.c_str();
        char *end;
        errno = 0;
        long k = strtol(arg, &end, 10);
        int e = errno;
        if (arg == end) {
            string err("could not find any digits");
            throw ParseError(err);
        }
        if (e) {
            if (k == LONG_MAX || k == LONG_MIN) {
                string err("overflow in parsing value");
                throw ParseError(err);
            }
            else {
                string err(strerror(e));
                throw ParseError(err);
            }
        }
        if (k > INT_MAX || k < INT_MIN) {
            string err("overflow in parsing value");
            throw ParseError(err);
        }
        return static_cast<int>(k);
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}

//!
//!  \brief convert string into a long
//!
template<> inline  long string2type<long>(const string& str)
{
    try {
        const char * arg = str.c_str();
        char *end;
        errno = 0;
        long k = strtol(arg, &end, 10);
        int e = errno;
        if (arg == end) {
            string err("could not find any digits");
            throw ParseError(err);
        }
        if (e) {
            if (k == LONG_MAX || k == LONG_MIN) {
                string err("overflow in parsing value");
                throw ParseError(err);
            }
            else {
                string err(strerror(e));
                throw ParseError(err);
            }
        }
        return k;
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}

template<> inline  unsigned int string2type<unsigned int>(const string& str)
{
    try {
        const char * arg = str.c_str();
        char *end;
        errno = 0;
        unsigned long k = strtoul(arg, &end, 10);
        int e = errno;
        if (arg == end) {
            string err("could not find any digits");
            throw ParseError(err);
        }
        if (e) {
            if (k == ULONG_MAX) {
                string err("overflow in parsing value");
                throw ParseError(err);
            }
            else {
                string err(strerror(e));
                throw ParseError(err);
            }
        }
        if (k > UINT_MAX) {
            string err("overflow in parsing value");
            throw ParseError(err);
        }
        return static_cast<unsigned int>(k);
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}

template<> inline  unsigned long string2type<unsigned long>(const string& str)
{
    try {
        const char * arg = str.c_str();
        char *end;
        errno = 0;
        unsigned long k = strtoul(arg, &end, 10);
        int e = errno;
        if (arg == end) {
            string err("could not find any digits");
            throw ParseError(err);
        }
        if (e) {
            if (k == ULONG_MAX) {
                string err("overflow in parsing value");
                throw ParseError(err);
            }
            else {
                string err(strerror(e));
                throw ParseError(err);
            }
        }
        return k;
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}

template<> inline  float string2type<float>(const string& str)
{
    try {
        const char * arg = str.c_str();
        char *end;
        errno = 0;
        double k = strtod(arg, &end);
        int e = errno;
        if (arg == end) {
            string err("could not find any digits");
            throw ParseError(err);
        }
        if (e) {
            if (k == DBL_MAX || k == DBL_MIN) {
                string err("overflow in parsing value");
                throw ParseError(err);
            }
            else {
                string err(strerror(e));
                throw ParseError(err);
            }
        }
        if (k > FLT_MAX) {
            string err("overflow/underflow in parsing float value");
            throw ParseError(err);
        }
        return static_cast<float>(k);
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}

template<> inline  double string2type<double>(const string& str)
{
    try {
        const char * arg = str.c_str();
        char *end;
        errno = 0;
        double k = strtod(arg, &end);
        int e = errno;
        if (arg == end) {
            string err("could not find any digits");
            throw ParseError(err);
        }
        if (e) {
            if (k == DBL_MAX || k == DBL_MIN) {
                string err("overflow in parsing value");
                throw ParseError(err);
            }
            else {
                string err(strerror(e));
                throw ParseError(err);
            }
        }
        return k;
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}

template<> inline  string string2type<string>(const string& str)
{
    return str;
}
;

template<> inline  bool string2type<bool>(const string& str)
{
    try {
        if (str.size() == 0) {
            string err("null string passed as value");
            throw ParseError(err);
        }
        char ch = str[0];
        if (ch == 'F' || ch == 'f' || ch == '0' || ch == 'N' || ch == 'n')
            return false;
        return true;
    }
    catch (exception& e) {
        cerr << "putils::string2type error parsing " << str;
        cerr << " exception " << e.what() << endl;
        throw ParseError();
    }
}
;

//!
//! @brief convert the given data element to a string
//!
template< class T > inline string type2string(const T& x)
{
    try {
        ostringstream os;
        os << x;
        return os.str();
    }
    catch (exception& e) {
        cerr << "putils::type2string error parsing " << x;
        cerr << "exception " << e.what() << endl;
        throw ParseError();
    }
}
;

template<> inline  string type2string<string>(const string& x)
{
    try {
        return x;
    }
    catch (exception& e) {
        cerr << "putils::type2string error parsing " << x;
        cerr << "exception " << e.what() << endl;
        throw ParseError();
    }
}
;

template<> inline string type2string<bool>(const bool& x)
{
    try {
        if (x)
            return string("true");
        else
            return string("false");
    }
    catch (exception& e) {
        cerr << "putils::type2string error parsing " << x;
        cerr << "exception " << e.what() << endl;
        throw  ParseError();
    }
}
;

template<> inline string type2string<int>(const int& x)
{
    try {
        char str[32];
        sprintf(str,"%d",x);
        return string(str);
    }
    catch (exception& e) {
        string err("putils::type2string error parsing int to string");
        throw ParseError(err);
    }
}

template<> inline string type2string<long>(const long& x)
{
    try {
        char str[32];
        sprintf(str,"%ld",x);
        return string(str);
    }
    catch (exception& e) {
        string err("putils::type2string error parsing long to string");
        throw ParseError(err);
    }
}
template<> inline string type2string<unsigned int>(const unsigned int& x)
{
    try {
        char str[32];
        sprintf(str,"%u",x);
        return string(str);
    }
    catch (exception& e) {
        string err("putils::type2string error parsing unsigned int to string");
        throw ParseError(err);
    }
}

template<> inline string type2string<unsigned long>(const unsigned long& x)
{
    try {
        char str[32];
        sprintf(str,"%lu",x);
        return string(str);
    }
    catch (exception& e) {
        string err("putils::type2string error parsing unsigned long to string");
        throw ParseError(err);
    }
}

template<> inline string type2string<float>(const float& x)
{
    try {
        char str[32];
        sprintf(str,"%g",x);
        return string(str);
    }
    catch (exception& e) {
        string err("putils::type2string error parsing float to string");
        throw ParseError(err);
    }
}

template<> inline string type2string<double>(const double& x)
{
    try {
        char str[32];
        sprintf(str,"%lg",x);
        return string(str);
    }
    catch (exception& e) {
        string err("putils::type2string error parsing double to string");
        throw ParseError(err);
    }
}

//!
//! \brief splits a string into tokens by cutting at points where certain characters called delimiters appear.
//!
//!  These tokens can be extracted in a java iterator fashion using hasTokens and nextToken or extracted at once
//!  using the splitString method. The advantage to the java iterator methods, is that one can parse half the string
//!  with one set of delimiters, reset the delimiters and parse the rest of the string.
//!
//!
class StringTokenizer {
#define DEFAULT_DELIMITERS string(" \n\t\r\f")
public:
    StringTokenizer(const string& string_in,const string& delimiters_in):
        str(string_in),del(delimiters_in),first(0),last(0)
    {
        if (del.size()==0) del=DEFAULT_DELIMITERS;
        tokenize();
    };
    StringTokenizer(const char* string_in,const char* delimiters_in):
        str(string_in),del(delimiters_in),first(0),last(0)
    {
        if (del.size()==0) del=DEFAULT_DELIMITERS;
        tokenize();
    };

    virtual ~StringTokenizer()
    {
    };
    //!
    //!  \brief replace the current string with a new one to split using the current delimiters
    //!
    void setString(const string& new_string)
    {
        str=new_string;
        rewind();
    };
    void setString(const char *new_string)
    {
        str=new_string;
        rewind();
    };
    //!
    //! \brief change the delimiters to use for splitting the string
    //!
    void setDelimiters(const string& new_delimiters)
    {
        del=new_delimiters;
    };
    //!
    //! \brief resets the delimiters to the default one \n \r \t\ \f and space characters.
    //!
    void setDelimiters()
    {
        del=DEFAULT_DELIMITERS;
    };
    //!
    //! \brief change the delimiters to use for splitting the string
    //!
    void setDelimiters(const char *new_delimiters)
    {
        del=new_delimiters;
        if (del.size()==0) del=DEFAULT_DELIMITERS;
    };
    //!
    //! \brief reset the tokenization to the beginning of string
    //!
    void rewind()
    {
        first=last=0;
        tokenize();
    };
    //!
    //! \brief return true if there are more tokens to extract
    //!
    bool hasTokens() const throw()
    {
        return (first!=string::npos);
    };

    //!
    //! \brief returns the number of  tokens after splitting the string
    //!
    size_t countTokens() const
    {
        size_t f=first;
        size_t l=last;
        size_t num_tokens=0;
        while (f!=string::npos) {
            ++num_tokens;
            f=str.find_first_not_of(del,l);
            l=str.find_first_of(del,f);
        }
        return num_tokens;
    };
    //!
    //! \brief extract all tokens at once and place them into the std:vector<string> tokens
    //!
    size_t splitString(std::vector<string>& tokens) const
    {
        size_t f(0);
        size_t l(0);
        tokens.clear();
        f=str.find_first_not_of(del,l);
        l=str.find_first_of(del,f);
        while (f!=string::npos) {
            tokens.push_back(str.substr(f,(l-f)));
            f=str.find_first_not_of(del,l);
            l=str.find_first_of(del,f);
        }
        return tokens.size();
    };
    //!
    //! \brief return the next Token in the seqquence and advance the counter.
    //1
    string nextToken()
    {
        size_t f=first;
        size_t l=last;
        if (f!=string::npos) {
            tokenize();
            return str.substr(f,(l-f));
        }
        string err("StringTokenizer::nextToken tried to get token past end of string");
        throw runtime_error(err);
    };

    //!
    //! \brief  return the next token as a specified type and advance the counter
    //!
    template < class T > T nextElement()
    {
        string token=nextToken();
        return string2type<T>(token);
    };

private:
    string str;
    string del;
    size_t first;
    size_t last;

    void tokenize()
    {
        first=str.find_first_not_of(del,last);
        last=str.find_first_of(del,first);
    };
};

}


#endif /* PUTILS_HPP_ */
