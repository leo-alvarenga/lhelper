/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#include "config.h"
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>

/* status global */
char *status = NULL;

void help();

void xsetroot_name(char *str, Display *dpy);

/* helpers */
bool contains(const char *a, const char *b);

/* module handlers */
char *battery_module(const Module mod, const BatteryInfo *bat);
char *uptime_module(const Module mod, UptimeInfo *uptime);
char *loadavg_module(const Module mod, LoadAvg *load);
char *backlight_module(const Module mod, BacklightInfo *back);
char* retrieve_all();

int main(int argc, char *argv[]) {
    bool use_stdout = !use_setxroot;
    
    if (argc > 1) {
        if (strcmp(argv[1], "--output") != 0) {
            help();
            return 0;
        }

        use_stdout = true;
    }

    status = retrieve_all();

    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
		fprintf(stderr, "lstatus could not proceed; cannot open display \n");
		return 1;
	}

    if (use_stdout) {
        printf("%s\n", status);
        return 0;
    }

    for (;; sleep(base_interval)) {
        xsetroot_name(status, dpy);
        retrieve_all();
    }

    return 0;
}

void help() {
    printf("Help screen....\n");
}

void xsetroot_name(char *str, Display *dpy) {
    XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

bool contains(const char *a, const char *b) {
    return strstr(a, b) != NULL;
}

char* retrieve_all() {    
    BatteryInfo *bat = NULL;
    UptimeInfo *uptime = NULL;
    LoadAvg *load = NULL;
    BacklightInfo *back = NULL;
    
    char *value;
    Module cur;

    if (status != NULL) free(status);

    status = (char *)(malloc(sizeof(char) * 256));
    if (status == NULL) return NULL;

    strcpy(status, "");

    for (int i = 0; i < m_count; ++i) {
        cur = modules[i];

        /* value is a variable used as shorthand to the formatted data */
        value = (char *)(malloc(sizeof(char) * 26));
        
        if (contains(cur.name, "battery")) {
            if (bat != NULL) free(bat);
            
            bat = get_battery_info();
            if (bat == NULL) continue;

            value = battery_module(cur, bat);
        } else if (contains(cur.name, "uptime")) {
            if (uptime != NULL) free(uptime);
            
            uptime = get_uptime();
            if (uptime == NULL) continue;

            value = uptime_module(cur, uptime);
        } else if (contains(cur.name, "loadavg")) {
            if (load != NULL) free(load);
            
            load = get_loadavg();
            if (load == NULL) continue;

            value = loadavg_module(cur, load);
        } else if (contains(cur.name, "backlight")) {
            if (back == NULL) back = get_backlight_info();
            if (back == NULL) continue;

            value = backlight_module(cur, back);
        } else if (contains(cur.name, "time")) {
            sprintf(value, "%s%s%s%s%s", cur.spacer, cur.prefix, get_local_time(localtime_fmt, tz_name), cur.suffix[0], cur.spacer);
        }

        if (value != NULL) {
            strcat(status, value);
            free(value);

            if (i < m_count - 1) strcat(status, separator);
        }
    }

    if (bat != NULL) free(bat);
    if (uptime != NULL) free(uptime);
    if (load != NULL) free(load);
    if (back != NULL) free(back);

    return status;
}

char *battery_module(const Module mod, const BatteryInfo *bat) {
    short which_suffix = 0;

    char *value = (char *)(malloc(sizeof(char) * 16));
    char *out = (char *)(malloc(sizeof(char) * 30));
    if (value == NULL || out == NULL) return NULL;


    if (contains(mod.name, "percentage+")) {
        which_suffix = 3;

        if (strcmp(bat->status, "Discharging") == 0)
            which_suffix = 0;
        else if (strcmp(bat->status, "Charging") == 0)
            which_suffix = 1;
        else if (strcmp(bat->status, "Full") == 0)
            which_suffix = 2;

        sprintf(value, "%d", bat->percentage);
    } else if (contains(mod.name, "percentage")) {
        sprintf(value, "%d", bat->percentage);
    } else if (contains(mod.name, "rate")) {
        sprintf(value, "%d", bat->percentage);
    } else if (contains(mod.name, "charge")) {
        sprintf(value, "%d", bat->cur_charge);
    } else if (contains(mod.name, "full_at")) {
        sprintf(value, "%d", bat->max_charge);
    } else if (contains(mod.name, "full_des")) {
        sprintf(value, "%d", bat->des_charge);
    } else if (contains(mod.name, "health")) {
        sprintf(value, "%d", bat->health);
    } else if (contains(mod.name, "status")) {
        which_suffix = 3;

        if (strcmp(bat->status, "Discharging") == 0)
            which_suffix = 0;
        else if (strcmp(bat->status, "Charging") == 0)
            which_suffix = 1;
        else if (strcmp(bat->status, "Full") == 0)
            which_suffix = 2;
        
        sprintf(value, "%s", bat->status);
    } else {
        free(value);
        value = NULL;
    }

    if (value != NULL) {
        sprintf(out, "%s%s%s%s%s", mod.spacer, mod.prefix, value, mod.suffix[which_suffix], mod.spacer);
        free(value);
    }

    return out;
}

char *uptime_module(const Module mod, UptimeInfo *uptime) {
    char *value = (char *)(malloc(sizeof(char) * 16));
    char *out = (char *)(malloc(sizeof(char) * 50));
    if (value == NULL || out == NULL) return NULL;  
    
    if (contains(mod.name, "all") || contains(mod.name, "dhms")) {
        sprintf(
            value,
            "%d%s%s%d%s%s%d%s%s%d%s",
            uptime->days, day_suffix, mod.spacer,
            uptime->hrs, hrs_suffix, mod.spacer,
            uptime->min, min_suffix, mod.spacer,
            uptime->s, sec_suffix
        );
    } else if (contains(mod.name, "hms")) {
        uptime->hrs += (uptime->days * 24);

        sprintf(
            value,
            "%d%s%s%d%s%s%d%s",
            uptime->hrs, hrs_suffix, mod.spacer,
            uptime->min, min_suffix, mod.spacer,
            uptime->s, sec_suffix
        );
    } else if (contains(mod.name, "ms")) {
        uptime->hrs += (uptime->days * 24);
        uptime->min += (uptime->hrs * 60);

        sprintf(
            value,
            "%d%s%s%d%s",
            uptime->min, min_suffix, mod.spacer,
            uptime->s, sec_suffix
        );
    } else if (contains(mod.name, "s")) {
        uptime->hrs += (uptime->days * 24);
        uptime->min += (uptime->hrs * 60);
        uptime->s += (uptime->min * 60);

        sprintf(
            value, "%d%s",
            uptime->s, sec_suffix
        );
    } else {
        free(value);
        value = NULL;
    }

    if (value != NULL) {
        sprintf(out, "%s%s%s%s%s", mod.spacer, mod.prefix, value, mod.suffix[0], mod.spacer);
        free(value);
    }

    return out;
}

char *loadavg_module(const Module mod, LoadAvg *load) {
    char *value = (char *)(malloc(sizeof(char) * 16));
    char *out = (char *)(malloc(sizeof(char) * 30));
    if (value == NULL || out == NULL) return NULL;  

    if (contains(mod.name, "fifteen") || contains(mod.name, "15")) {
        sprintf(value, "%.2f", load->fifteen_min);
    } else if (contains(mod.name, "five") || contains(mod.name, "5")) {
        sprintf(value, "%.2f", load->five_min);
    } else if (contains(mod.name, "one") || contains(mod.name, "1")) {
        sprintf(value, "%.2f", load->one_min);
    } else if (contains(mod.name, "entities")) {
        sprintf(value, "%s", load->entities);
    } else if (contains(mod.name, "pid")) {
        sprintf(value, "%d", load->newest_proc);
    } else {
        free(value);
        value = NULL;
    }

    if (value != NULL) {
        sprintf(out, "%s%s%s%s%s", mod.spacer, mod.prefix, value, mod.suffix[0], mod.spacer);
        free(value);
    }

    return out;
}

char *backlight_module(const Module mod, BacklightInfo *back) {
    char *value = (char *)(malloc(sizeof(char) * 8));
    char *out = (char *)(malloc(sizeof(char) * 10));
    if (value == NULL || out == NULL) return NULL;  

    if (contains(mod.name, "percentage")) {
        sprintf(value, "%d", back->brightness_rate);
    } else if (contains(mod.name, "ratio")) {
        sprintf(value, "%d/%d", back->brightness, back->max_brightness);
    } else if (contains(mod.name, "brightness")) {
        sprintf(value, "%d", back->brightness);
    } else if (contains(mod.name, "max")) {
        sprintf(value, "%d", back->max_brightness);
    } else {
        free(value);
        value = NULL;
    }

    if (value != NULL) {
        sprintf(out, "%s%s%s%s%s", mod.spacer, mod.prefix, value, mod.suffix[0], mod.spacer);
        free(value);
    }

    return out;
}
