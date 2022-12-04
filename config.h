/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#ifndef CONFIG_H

#include "lstatus.h"

/* Customizable variables: change these accordding to your needs */
static const bool has_battery     = true;
static const char *battery_device = "/sys/class/power_supply/BAT1";
static const char *backlight_path = "/sys/class/backlight/intel_backlight";
static const char *uptime_path    = "/proc/uptime";
static const char *loadavg_path   = "/proc/loadavg";

/* Time zone and output formatting */
static const char *tz_name = "America/Sao_Paulo";

/* Use the formatting expected by 'strftime' */
static const char *localtime_fmt = "%a, %b %d %H:%M";

/* Suffixes shared among all time related values */
static const char *day_suffix = "d";
static const char *hrs_suffix = "hrs";
static const char *min_suffix = "min";
static const char *sec_suffix = "s";

/* Str to be used between each module's output */
static const char *separator = "|";

/* Module count; Change according to the modules array length */
static const short int m_count = 6;

/*
    Syntax: module_name:value
    Available modules:
        - time
        - battery
            - :percentage
            - :percentage+ *
            - :charge
            - :full_at
            - :full_des
            - :health
            - :status *
        - uptime
            - :all (or :dhms)
            - :hms
            - :ms
            - :s
        - loadavg *
            - :one (or :1)
            - :five (or :5)
            - :fifteen (or :15)
            - :entities
            - :pid
        - backlight
            - :percentage
            - :ratio *
            - :brightness
            - :max
    Important:
    * 'battery:percentage+' is the same as 'battery:percentage', except its suffix
        works a little different than others, requiring at least four (4) suffixes
        to work as they represent, respectively, Discharing, Charging, Full and Other
    * The 'battery:status' suffix works a little different than others, 
        requiring at least four (4) suffixes to work as they represent,
        respectively, Discharing, Charging, Full and Other
    * The default behavior for the loadavg floating points is to use two (2)
        decimal fields; To alter it, change the 'sprintf' calls in the `output()` function.
    * 'backlight:ratio' is the ratio between the current and max brightness (E.g.: 67/153)
*/

static const Module modules[] = {
    { 1, "battery:percentage+", " ",  "", "%", "%+", "%!", "%?" },
    { 1, "battery:health", " ",  "", "%" },
    { 1, "battery:status", " ", "", "-", "+", "", "?" },
    { 1, "backlight:percentage", " ", "", "%" },
    { 1, "uptime:hms", " ",  "", "" },
    { 1, "time", " ",  "TIME: ", "" },
    /* Example:
        { "loadavg:pid", "  ",  "ID: (", ")" }
    */
};

#endif // CONFIG_H