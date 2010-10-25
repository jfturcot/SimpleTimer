/*
 * SimpleTimer.h
 *
 * SimpleTimer - A timer library for Arduino.
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef SIMPLETIMER_H
#define SIMPLETIMER_H

#include <WProgram.h>

typedef void (*timer_callback)(int);

class SimpleTimer {

public:
    // maximum number of timers
    const static int MAX_TIMERS = 10;

    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;

    // constructor
    SimpleTimer(const int timer_id);

    // this function must be called inside loop()
    void run();

    // call function f every d milliseconds
    int setInterval(long d, timer_callback f);

    // call function f once after d milliseconds
    int setTimeout(long d, timer_callback f);

    // call function f every d milliseconds for n times
    int setTimer(long d, timer_callback f, int n);

    // destroy the specified timer
    void deleteTimer(int numTimer);

    // returns true if the specified timer is enabled
    boolean isEnabled(int numTimer);

    // enables the specified timer
    void enable(int numTimer);

    // disables the specified timer
    void disable(int numTimer);

    // enables or disables the specified timer
    // based on its current state
    void toggle(int numTimer);

    // returns the number of used timers
    int getNumTimers();

		// returns the miliseconds left before the next event
		// for the specified timer
    int getTimeLeft(int numTimer);

private:
    // value returned by the millis() function
    // in the previous run() call
    long prev_millis[MAX_TIMERS];

    // pointers to the callback functions
    timer_callback callbacks[MAX_TIMERS];

    // delay values
    long delays[MAX_TIMERS];

    // number of runs to be executed for each timer
    int maxNumRuns[MAX_TIMERS];

    // number of executed runs for each timer
    int numRuns[MAX_TIMERS];

    // which timers are enabled
    boolean enabled[MAX_TIMERS];

    // actual number of timers in use
    int numTimers;

		// id of the timer
		int id;
};

#endif