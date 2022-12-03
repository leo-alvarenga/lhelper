/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#include "config.h"

void help();

/* helpers */
bool contains(const char *a, const char *b);

/* module handlers */
char *battery_module(const Module mod, const BatteryInfo *bat);
char *uptime_module(const Module mod, Uptime *uptime);
char *loadavg_module(const Module mod, LoadAvg *load);
char** retrieve_all();

int main(int argc, char *argv[]) {
    if (argc > 1) {
        help();
        return 0;
    }

    char **data = retrieve_all();
    for (int i = 0; i < m_count; ++i) {
        printf("%s", data[i]);

        if (i < m_count - 1)
            printf("%s", separator);
    }

    return 0;
}

void help() {
    printf("Help screen....\n");
}

bool contains(const char *a, const char *b) {
    return strstr(a, b) != NULL;
}

char** retrieve_all() {
    char **out;
    out = (char **)(malloc(sizeof(char *) * m_count));

    for (int i = 0; i < m_count; ++i) {
        out[i] = (char *)(malloc(sizeof(char) * 5));
    }
    
    BatteryInfo *bat = NULL;
    Uptime *uptime = NULL;
    LoadAvg *load = NULL;
    
    short which_suffix;
    char *value;
    Module cur;
    for (int i = 0; i < m_count; ++i) {
        cur = modules[i];
        which_suffix = 0;

        /* value is a variable used as shorthand to the formatted data */
        value = (char *)(malloc(sizeof(char) * 16));
        
        if (contains(cur.name, "battery")) {
            if (bat == NULL) bat = get_battery_info();
            if (bat == NULL) continue;

            value = battery_module(cur, bat);
        } else if (contains(cur.name, "uptime")) {
            if (uptime != NULL) free(uptime);
            
            uptime = get_uptime();
            if (uptime == NULL) continue;

            value = uptime_module(cur, uptime);
        } else if (contains(cur.name, "loadavg")) {
            if (load == NULL) load = get_loadavg();
            if (load == NULL) continue;

            value = loadavg_module(cur, load);
        }

        if (value != NULL) {
            strcpy(out[i], value);
            free(value);
        }
    }

    if (bat != NULL) free(bat);

    return out;
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

char *uptime_module(const Module mod, Uptime *uptime) {
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
