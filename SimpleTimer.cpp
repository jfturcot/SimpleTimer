/*
 * SimpleTimer.cpp
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

#include "SimpleTimer.h"


SimpleTimer::SimpleTimer() {
    long current_millis = millis();

    for (int i = 0; i < MAX_TIMERS; i++) {
        enabled[i] = false;
        callbacks[i] = 0;
        prev_millis[i] = current_millis;
    }

    numTimers = 0;
}


void SimpleTimer::run() {
    int i;
    long current_millis;

    // get current time
    current_millis = millis();

    for (i = 0; i < MAX_TIMERS; i++) {

        // only process active timers
        if (callbacks[i] && enabled[i]) {

            // is it time to process this timer ?
            if (current_millis - prev_millis[i] >= delays[i]) {

                // update time
                prev_millis[i] = current_millis;

                // "run forever" timers must always be executed
                if (maxNumRuns[i] == RUN_FOREVER) {
                    (*callbacks[i])();
                }
                // other timers get executed the specified number of times
                else if (numRuns[i] < maxNumRuns[i]) {
                    (*callbacks[i])();
                    numRuns[i]++;

                    // after the last run, delete the timer
                    // to save some cycles
                    if (numRuns[i] >= maxNumRuns[i]) {
                        deleteTimer(i);
                    }
                }
            }
        }
    }
}


int SimpleTimer::setTimer(long d, timer_callback f, int n) {
    if (numTimers >= MAX_TIMERS) {
        return -1;
    }

    delays[numTimers] = d;
    callbacks[numTimers] = f;
    maxNumRuns[numTimers] = n;
    enabled[numTimers] = true;
    numRuns[numTimers] = 0;

    numTimers++;

    return (numTimers - 1);
}


int SimpleTimer::setInterval(long d, timer_callback f) {
    return setTimer(d, f, RUN_FOREVER);
}


int SimpleTimer::setTimeout(long d, timer_callback f) {
    return setTimer(d, f, RUN_ONCE);
}


void SimpleTimer::deleteTimer(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    // nothing to disable if no timers are in use
    if (numTimers == 0) {
        return;
    }

    callbacks[numTimer] = 0;
    enabled[numTimer] = false;
    delays[numTimer] = 0;

    // update number of timers
    numTimers--;
}


boolean SimpleTimer::isEnabled(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    return enabled[numTimer];
}


void SimpleTimer::enable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    enabled[numTimer] = true;
}


void SimpleTimer::disable(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    enabled[numTimer] = false;
}


void SimpleTimer::toggle(int numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    enabled[numTimer] = !enabled[numTimer];
}


int SimpleTimer::getNumTimers() {
    return numTimers;
}