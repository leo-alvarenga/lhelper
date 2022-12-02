/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#ifndef CONFIG_H

#include "lhelper.h"

/* Customizable variables: change these accordding to your needs */
static const bool has_battery     = true;
static const char *battery_device = "/sys/class/power_supply/BAT1";
static const char *loadavg_path   = "/proc/loadavg";

static const char *divider = "|";
static const char *spacer  = " ";

static const short int m_count = 11; // module count; change according to constants below

/*
    Syntax: module_name:option
    Available modules:
        - battery
            - :percentage
            - :charge
            - :full_at
            - :full_des
            - :health
            - :status
        - loadavg*
            - :one (or :1)
            - :five (or :5)
            - :fifteen (or :15)
            - :entities
            - :pid
    (*) The default behavior for the loadavg floating points is to use 2 decimal fields;
        To alter it, change the sprintf calls in the `output()` function.
*/
static const char *modules[]   = {
    "battery:percentage",
    "battery:charge",
    "battery:full_at",
    "battery:full_des",
    "battery:health",
    "battery:status",
    "loadavg:1",
    "loadavg:5",
    "loadavg:15",
    "loadavg:entities",
    "loadavg:pid",
};

static const char *prefixes[] = {
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
    ". ",
};

#endif // CONFIG_H