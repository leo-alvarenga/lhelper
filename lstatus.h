/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#ifndef LSTATUS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

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
} UptimeInfo;

typedef struct {
    float one_min;
    float five_min;
    float fifteen_min;
    char *entities;
    int newest_proc;
} LoadAvg;

typedef struct {
    int brightness;
    int max_brightness;
    int brightness_rate;
} BacklightInfo;

typedef struct {
    int used_rate;
    int free_rate;
    int swp_used_rate;
    int swp_free_rate;
    int used;
    int cach;
    int shrd;
    int free;
    int swp_used;
    int swp_free;
} MemInfo;

typedef struct {
    const char *name;
    const char *spacer;
    const char *prefix;
    const char *suffix[4];
} Module;

char* join_path(const char *base, const char *dir);
char* get_local_time(const char *fmt, const char *tz);
BatteryInfo* get_battery_info();
UptimeInfo* get_uptime();
LoadAvg* get_loadavg();
BacklightInfo* get_backlight_info();
MemInfo* get_meminfo();

#endif // LSTATUS