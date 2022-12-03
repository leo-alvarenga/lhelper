/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#ifndef LHERLPER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char prefix;
} ModuleMeta;

typedef struct {
    int des_charge;
    int max_charge;
    int cur_charge;
    int percentage;
    int health;
    char *status;
} BatteryInfo;

typedef struct {
    int days, hrs, min, s;
} Uptime;

typedef struct {
    float one_min;
    float five_min;
    float fifteen_min;
    char *entities;
    int newest_proc;
} LoadAvg;

typedef struct {
    const char *name;
    const char *spacer;
    const char *prefix;
    const char *suffix[4];
} Module;

char* join_path(const char *base, const char *dir);
BatteryInfo* get_battery_info();
Uptime* get_uptime();
LoadAvg* get_loadavg();

#endif // LHELPER