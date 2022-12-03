/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#include "config.h"
#include <string.h>

void help();
bool contains(const char *a, const char *b);
char** retrieve();

int main(int argc, char *argv[]) {
    if (argc > 1) {
        help();
        return 0;
    }

    char **data = retrieve();
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

char** retrieve() {
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

            if (contains(cur.name, "percentage+")) {
                which_suffix = 3;

                if (strcmp(bat->status, "Discharging") == 0)
                    which_suffix = 0;
                else if (strcmp(bat->status, "Charging") == 0)
                    which_suffix = 1;
                else if (strcmp(bat->status, "Full") == 0)
                    which_suffix = 2;

                sprintf(value, "%d", bat->percentage);
            } else if (contains(cur.name, "percentage")) {
                sprintf(value, "%d", bat->percentage);
            } else if (contains(cur.name, "rate")) {
                sprintf(value, "%d", bat->percentage);
            } else if (contains(cur.name, "charge")) {
                sprintf(value, "%d", bat->cur_charge);
            } else if (contains(cur.name, "full_at")) {
                sprintf(value, "%d", bat->max_charge);
            } else if (contains(cur.name, "full_des")) {
                sprintf(value, "%d", bat->des_charge);
            } else if (contains(cur.name, "health")) {
                sprintf(value, "%d", bat->health);
            } else if (contains(cur.name, "status")) {
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

        } else if (contains(cur.name, "uptime")) {
            if (uptime != NULL) free(uptime);
            
            uptime = get_uptime();
            if (uptime == NULL) continue;

            if (contains(cur.name, "all") || contains(cur.name, "dhms")) {
                sprintf(
                    value,
                    "%d%s%s%d%s%s%d%s%s%d%s",
                    uptime->days, day_suffix, cur.spacer,
                    uptime->hrs, hrs_suffix, cur.spacer,
                    uptime->min, min_suffix, cur.spacer,
                    uptime->s, sec_suffix
                );
            } else if (contains(cur.name, "hms")) {
                uptime->hrs += (uptime->days * 24);

                sprintf(
                    value,
                    "%d%s%s%d%s%s%d%s",
                    uptime->hrs, hrs_suffix, cur.spacer,
                    uptime->min, min_suffix, cur.spacer,
                    uptime->s, sec_suffix
                );
            } else if (contains(cur.name, "ms")) {
                uptime->hrs += (uptime->days * 24);
                uptime->min += (uptime->hrs * 60);

                sprintf(
                    value,
                    "%d%s%s%d%s",
                    uptime->min, min_suffix, cur.spacer,
                    uptime->s, sec_suffix
                );
            } else if (contains(cur.name, "s")) {
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

        } else if (contains(cur.name, "loadavg")) {
            if (load == NULL) load = get_loadavg();
            if (load == NULL) continue;

            // (*) Alter the formatting on the following sprintf calls to change the decimal field count
            if (contains(cur.name, "fifteen") || contains(cur.name, "15")) {
                sprintf(value, "%.2f", load->fifteen_min);
            } else if (contains(cur.name, "five") || contains(cur.name, "5")) {
                sprintf(value, "%.2f", load->five_min);
            } else if (contains(cur.name, "one") || contains(cur.name, "1")) {
                sprintf(value, "%.2f", load->one_min);
            } else if (contains(cur.name, "entities")) {
                sprintf(value, "%s", load->entities);
            } else if (contains(cur.name, "pid")) {
                sprintf(value, "%d", load->newest_proc);
            } else {
                free(value);
                value = NULL;
            }
        }

        if (value != NULL) {
            sprintf(out[i], "%s%s%s%s%s", cur.spacer, cur.prefix, value, cur.suffix[which_suffix], cur.spacer);
            free(value);
        }
    }

    if (bat != NULL) free(bat);

    return out;
}
