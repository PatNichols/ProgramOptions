/*
 * ProgramOptions.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: pnichols
 */

#ifndef PROGRAMOPTIONS_HPP_
#define PROGRAMOPTIONS_HPP_
#include <string>
#include <vector>
#include <stdexcept>
#include <unistd.h>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <fstream>
#include "putils.hpp"
#include "FilePathUtils.h"
using namespace std;

namespace putils {
//!
//! \brief a program options class.
//!
//!  excepts most major input format schemes e.g.  --name=value, --name value , -name value and -name=value.
//!   one can also read in the name value pairs from a file or the environment.
//!
class ProgramOptions {
private:
    struct option_t {
    private:
        string key;
        string des;
        string val;
        int stat;
    public:
        option_t(const string& option_name, const string& description,
                 const string& default_value) :
            key(option_name), des(description), val(default_value), stat(-1)
        {
        }
        ;
        option_t(const string& option_name, const string& description) :
            key(option_name), des(description), val(""), stat(0)
        {
        }
        ;
        option_t(const char* option_name, const char * description,
                 const char * default_value) :
            key(option_name), des(description), val(default_value), stat(-1)
        {
        }
        ;
        option_t(const char* option_name, const char * description) :
            key(option_name), des(description), val(""), stat(0)
        {
        }
        ;

        void setValue(const string& new_value)
        {
            if (stat != 1) {
                stat = 1;
                val = new_value;
            }
        }
        ;

        bool hasValue() const throw ()
        {
            return (stat != 0);
        }
        ;
        bool wasSet() const throw ()
        {
            return (stat == 1);
        }
        ;
        bool matches(const string& option_name) const throw ()
        {
            return key.compare(option_name) == 0;
        }
        ;
        bool matches(const char * option_name) const throw ()
        {
            return key.compare(option_name) == 0;
        }
        ;
        string getOptionName() const throw ()
        {
            return string(key);
        }
        ;
        string getValue() const throw ()
        {
            return string(val);
        }
        ;
        ostream& write2Stream(ostream& os) const
        {
            os << "-" << key << " = " << des << endl;
            if (stat) {
                if (stat == 1) {
                    os << "    value = " << val << " set by user\n";
                }
                else {
                    os << "    value = " << val << " default value\n";
                }
            }
            return os;
        }
        ;
        ostream& operator<<(ostream& os) const
        {
            return this->write2Stream(os);
        };
    };

    vector<option_t> opts;
    bool allow_unused_options;
public:
    typedef vector<option_t>::iterator iterator;
    typedef vector<option_t>::const_iterator const_iterator;

    ///!
    ///! \brief default constructor
    ///!
    ProgramOptions():opts()
    {
    }
    ;

    virtual ~ProgramOptions()
    {
    }
    ;

    //!
    //! \brief return true if such an option name exists in the lists for this class.
    //!
    bool hasOption(const string& option_name) const throw ()
    {
        const_iterator iter = opts.begin();
        const_iterator iend = opts.end();
        for (; iter != iend; ++iter) {
            if (iter->matches(option_name))
                return true;
        }
        return false;
    }
    ;

    //!
    //! \brief return true if such an option name has a valid value for this class. (default value or one set by user).
    //!
    bool hasValue(const string& option_name) const throw ()
    {
        try {
            const_iterator iter = findConstIterator(option_name);
            return iter->hasValue();
        }
        catch (exception& e) {
            cerr << "ProgramOption::hasValue exception " << e.what() << endl;
            printHelp();
        }
        return false;
    }
    ;
    //!
    //! \brief return true if such an option name has a value for this class that was set by the user.
    //!

    bool wasSet(const string& option_name) const throw ()
    {
        try {
            const_iterator iter = findConstIterator(option_name);
            return iter->wasSet();
        }
        catch (exception& e) {
            cerr << "ProgramOption::hasValue exception " << e.what() << endl;
            printHelp();
        }
        return false;
    }
    ;
    //!
    //! \brief return the value associated with the option_name as a string
    //!
    string getValue(const string& option_name) const throw ()
    {
        try {
            const_iterator iter = findConstIterator(option_name);
            return iter->getValue();
        }
        catch (exception& e) {
            cerr << "ProgramOption::hasValue exception " << e.what() << endl;
            printHelp();
        }
        return string("");
    }
    ;
    //!
    //! \brief set the value associated with the option name with the given value.
    //!
    void setValue(const string& option_name, const string& value)
    {
        try {
            iterator iter = findIterator(option_name);
            return iter->setValue(value);
        }
        catch (exception& e) {
            if (allow_unused_options) {
                cerr << "option " << option_name << " not found\n";
                return;
            }
            cerr << "ProgramOption::setValue exception " << e.what() << endl;
            printHelp();
        }
    }
    ;

    //!
    //! \brief parse the command line for valid options and set their values to those given.
    //!
    void parseCommandLine(int argc,char **argv) throw()
    {
        try {
            for (int karg=1; karg<argc; ++karg) {
                string targ ( argv[karg] );
                if (targ.size()>2 && targ[0]=='-') {
                    int s=1;
                    if (targ[1]=='-') s=2;
                    string check_help = targ.substr(s,string::npos);
                    if (check_help.compare(0,4,"help")==0) {  
                        printHelp();
                    }
                    size_t eq_pos = targ.find("=");
                    if (eq_pos==string::npos) {
                        // no equal in options value
                        string key=targ.substr(s,string::npos);
                        if (key.compare("help")==0) printHelp();
                        int knext=karg+1;
                        if (knext<argc && argv[knext][0]!='-') {
                            string val(argv[knext]);
                            setValue(key,val);
                            ++karg;
                        }
                        else {
                            string val("1");
                            setValue(key,val);
                        }
                    }
                    else {
                        string key=targ.substr(s,(eq_pos-s));
                        string val=targ.substr(eq_pos+1,string::npos);
                        if (val.size())  {
                            setValue(key,val);
                        }
                        else {
                            setValue(key,string("1"));
                        }
                    }
                }
                else {
                    cerr << "ProgramOptions error found " << targ << endl;
                    string err("expected an -option_name but found value");
                    throw ParseError(err);
                }
            }
        }
        catch (exception& e) {
            cerr << "ProgramOption::parseCommandLine exception " << e.what() << endl;
            printHelp();
        }
    };

    //!
    //! \brief parse a file for valid options and set their values to those given.
    //!
    void parseOptionFile(const string& options_filename) throw()
    {
        if (!isRegularFile(options_filename)) {
            cerr << "File with options :" << options_filename << " do not exist or is not a regular file!\n";
            exit(EXIT_FAILURE);
        }
        if (!canRead(options_filename)) {
            cerr << "File with options :" << options_filename << " cannot be read!\n";
            exit(EXIT_FAILURE);        
        }
        try {
            ifstream in;
            in.exceptions(ifstream::badbit);
            const string delims(" =\t\n\r\f");
            in.open(options_filename.c_str());
            string sline;
            while (getline(in,sline)) {
                if (sline.size()==0) break;
                if (sline[0]=='!' || sline[0]=='#' || sline[0]=='[') continue;
                cerr << "line :"<< sline << endl;
                StringTokenizer tokenizer(sline,delims);
                vector<string> tokens;
                tokenizer.splitString(tokens);
                tokenizer.rewind();
                if (tokenizer.hasTokens()) {
                    string key=tokenizer.nextToken();
                    if (key.size()==0) {
                        throw ParseError(string("zero size key"));
                    }
                    if (tokenizer.hasTokens()) {
                        string val=tokenizer.nextToken();
                        setValue(key,val);
                    }
                    else {
                        setValue(key,string("true"));
                    }
                }
                else {
                    string err("no tokens found in option input file\n");
                    throw ParseError(err);
                }
                if (in.eof() || !in) break;
            }
            in.close();
        }
        catch (exception& e) {
            cerr << "ProgramOption::parseOptionFile exception " << e.what() << endl;
            printHelp();
        }
        cerr << "parsed option file " << options_filename << endl;
    };
    //!
    //! \brief parse the environment for valid options and set their values to those given.
    //! note the options name must appear as all caps in the environment variable and may
    //! be prefixed by the given string in all caps.
    //! For example an option such as data would be DATA and if the prefix is MIN then
    //! MIN_DATA.
    //!
    //!

    void parseEnvironment(const string& prefix=string("")) throw()
    {
        try {
            if (prefix.size()) {
                size_t sz=opts.size();
                for (size_t k=0; k<sz; ++k) {
                    string key = opts[k].getOptionName();
                    for (size_t j=0; j<key.size(); ++j) {
                        char ch = key[j];
                        if (isalpha(ch)) {
                            key[j] = toupper(ch);
                        }
                    }
                    string env_name = prefix + "_" + key;
                    char * ret=std::getenv(env_name.c_str());
                    if (ret) {
                        opts[k].setValue(string(ret));
                    }
                }
            }
            else {
                size_t sz=opts.size();
                for (size_t k=0; k<sz; ++k) {
                    string key = opts[k].getOptionName();
                    for (size_t j=0; j<key.size(); ++j) {
                        char ch = key[j];
                        if (isalpha(ch)) {
                            key[j] = toupper(ch);
                        }
                    }
                    char * ret=std::getenv(key.c_str());
                    if (ret) {
                        opts[k].setValue(string(ret));
                    }
                }
            }
        }
        catch (exception& e) {
            cerr << "ProgramOption::parseEnvironment exception " << e.what() << endl;
            printHelp();
        }
    };

    //!
    //! \brief add the option to the list with the given default value
    //!
    void addOption(const string& option_name, const string& description,
                   const string& default_value)
    {
        opts.push_back(option_t(option_name,description,default_value));
    }
    ;
    //!
    //! \brief add the option to the list
    //!
    void addOption(const string& option_name, const string& description)
    {
        opts.push_back(option_t(option_name,description));
    }
    ;
    //!
    //! \brief add the option to the list  with the given default value
    //!
    void addOption(const char *  option_name, const char *  description,
                   const char *  default_value)
    {
        opts.push_back(option_t(option_name,description,default_value));
    }
    ;
    //!
    //! \brief add the option to the list
    //!
    void addOption(const char *  option_name, const char *  description)
    {
        opts.push_back(option_t(option_name,description));
    }
    ;

    //!
    //! \brief helper method to write out options to a stream
    //!
    ostream& write2stream(ostream& os) const
    {
        const_iterator iter=opts.begin();
        const_iterator iend=opts.end();
        for (; iter!=iend; ++iter) {
            iter->write2Stream(os);
        }
        return os;
    }

    //!
    //! \brief print out the options (name,descriptions and values) then exit.
    //!
    void printHelp() const
    {
        std::cerr << "Usage is:\n";
        const_iterator iter=opts.begin();
        const_iterator iend=opts.end();
        for (; iter!=iend; ++iter) {
            iter->write2Stream(cerr);
        }
        exit(EXIT_FAILURE);
    };

protected:
    iterator findIterator(const string& option_name)
    {
        iterator iter = opts.begin();
        const_iterator iend = opts.end();
        for (; iter != iend; ++iter) {
            if (iter->matches(option_name))
                return iter;
        }
        string err("ProgramOptions could not find the option ");
        err += option_name;
        err += "\n";
        throw runtime_error(err);
    }
    ;
    const_iterator findConstIterator(const string& option_name) const
    {
        const_iterator iter = opts.begin();
        const_iterator iend = opts.end();
        for (; iter != iend; ++iter) {
            if (iter->matches(option_name))
                return iter;
        }
        string err("ProgramOptions could not find the option ");
        err += option_name;
        err += "\n";
        throw runtime_error(err);
    }
    ;
}; // end class defn.

} // end namespace putils

//!
//! \brief writes out options to stream (file,command line,etc)
//!
static inline ostream& operator<<(ostream& os,const putils::ProgramOptions& popts)
{
    return popts.write2stream(os);
};

#endif /* PROGRAMOPTIONS_HPP_ */
