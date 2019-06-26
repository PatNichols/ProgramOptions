/*
 * Stopwatch.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: pnichols
 */

#ifndef STOPWATCH_HPP_
#define STOPWATCH_HPP_
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <iomanip>
//#include "config.h"
#define HAVE_GETTIMEOFDAY 1
using namespace std;
namespace putils {



#ifdef HAVE_CLOCKGETTIME
///!
///!  \brief A stopwatch timer class. Records increments of time between start and stop and returns the sum of these increments.
///!
struct Stopwatch {
    Stopwatch():acc(0) {};

    ///!
    ///! \brief return accumulated total time
    ///1
    double elapsedTime() const throw()
    {
        return acc;
    };

    ///!
    ///! \brief set accumulated time to zero
    ///!
    void clear() throw()
    {
        acc=0.;
    };

    ///!
    ///! \brief start timing
    ///!
    void start()
    {
        clock_gettime(CLOCK_MONOTONIC,&ts);
    };

    ///!
    ///! \brief stop timing and add the time between stop and start to accumulated total
    ///!
    void stop()
    {
        clock_gettime(CLOCK_MONOTONIC,&tf);
        acc += (tf.tv_sec-ts.tv_sec) + 1.e-9*(tf.tv_nsec - ts.tv_nsec);
    };

    ///!
    ///! \brief return estimated time error
    ///!
    double tick() const
    {
        return 1.e-6;
    }
private:
    double acc;
    timespec ts;
    timespec tf;
};

#else
#ifdef HAVE_GETTIMEOFDAY
///!
///!  \brief A stopwatch timer class. Records increments of time between start and stop and returns the sum of these increments.
///!
struct Stopwatch {
    Stopwatch():acc(0) {};

    double elapsedTime() const throw()
    {
        return acc;
    };

    void clear() throw()
    {
        acc=0.;
    };

    void start()
    {
        gettimeofday(&ts,0x0);
    };

    void stop()
    {
        gettimeofday(&tf,0x0);
        acc += (tf.tv_sec-ts.tv_sec) + 1.e-6*(tf.tv_usec - ts.tv_usec);
    };

    double tick() const
    {
        return 1.e-6;
    }
private:
    double acc;
    timeval ts;
    timeval tf;
};

#else
///!
///!  \brief A stopwatch timer class. Records increments of time between start and stop and returns the sum of these increments.
///!
struct Stopwatch {
public:
    Stopwatch():acc(0)
    {
        cnv_const = 1./CLOCKS_PER_SEC;
    };

    double elapsedTime() const throw()
    {
        return acc*cnv_const;
    };

    void clear() throw()
    {
        acc=0.;
    };

    void start()
    {
        ts = clock();
    };

    void stop()
    {
        tf = clock();
        acc += (tf-ts);
    };

    double tick() const
    {
        return 1.e-2;
    }
private:
    double acc,cnv_const;
    clock_t ts;
    clock_t tf;
};

#endif
#endif
} /* namespace putils */
#endif /* STOPWATCH_HPP_ */
