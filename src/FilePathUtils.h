#ifndef _FILEPATH_H_
#define _FILEPATH_H_
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <iostream>
extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
}
#include <unistd.h>
#include <stdexcept>
using namespace std;

namespace putils {

inline bool isRegularFile(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::isRegularFile error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return S_ISREG(fst.st_mode);
}

inline bool isDirectory(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::isDirectory error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return S_ISDIR(fst.st_mode);
}

inline bool isLink(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::isLink error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return S_ISLNK(fst.st_mode);
}

inline bool canRead(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::isRead error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return bool( fst.st_mode & S_IRUSR);
}

inline bool canExecute(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::canExecute error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return bool( fst.st_mode & S_IXUSR);
}

inline bool canWrite(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::canWrite error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return bool( fst.st_mode & S_IWUSR);
}


inline size_t sizeOfFile(const string& filename)
{
    struct stat64 fst;
    errno = 0;
    int e = stat64(filename.c_str(),&fst);
    int ecode = errno;
    if (e==-1) {
        if (ecode == ENOENT) {
            return false;
        }
        string msg("FileInfo::sizeofOfFile error ");
        msg += strerror(ecode);
        cerr << msg << endl;
        return false;
    }
    return fst.st_size;
}

inline void copyFileToFile(const string& inputFile,const string& outputFile)
{
    try {
        ofstream out(inputFile.c_str());
        ifstream in(outputFile.c_str());
        out << in.rdbuf();
        out.close();
        in.close();
    } catch (exception& e) {
        string err_msg("FileInfo::copyFileToFile error copying ");
        err_msg += inputFile;
        err_msg += " to ";
        err_msg += outputFile;
        err_msg +=" ";
        err_msg += e.what();
        throw runtime_error(err_msg);
    }
}

}
#endif
