/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#include "config.h"

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
        printf(">> '%s'\n", data[i]);
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
    LoadAvg *load = NULL;
    
    char *value;
    const char *cur, *prefix;
    for (int i = 0; i < m_count; ++i) {
        cur = modules[i];
        prefix = prefixes[i];

        /* value is a variable used as shorthand to the formatted data */
        value = (char *)(malloc(sizeof(char) * 16));
        
        if (contains(cur, "battery")) {
            if (bat == NULL) bat = get_battery_info();
            if (bat == NULL) continue;

            if (contains(cur, "percentage")) {
                sprintf(value, "%d", bat->percentage);
            } else if (contains(cur, "charge")) {
                sprintf(value, "%d", bat->cur_charge);
            } else if (contains(cur, "full_at")) {
                sprintf(value, "%d", bat->max_charge);
            } else if (contains(cur, "full_des")) {
                sprintf(value, "%d", bat->des_charge);
            } else if (contains(cur, "health")) {
                sprintf(value, "%d", bat->health);
            } else if (contains(cur, "status")) {
                sprintf(value, "%s", bat->status);
            } else {
                free(value);
                value = NULL;
            }

        } else if (contains(cur, "loadavg")) {
            if (load == NULL) load = get_loadavg();
            if (load == NULL) continue;

            // (*) Alter the formatting on the following sprintf calls to change the decimal field count
            if (contains(cur, "fifteen") || contains(cur, "15")) {
                sprintf(value, "%.2f", load->fifteen_min);
            } else if (contains(cur, "five") || contains(cur, "5")) {
                sprintf(value, "%.2f", load->five_min);
            } else if (contains(cur, "one") || contains(cur, "1")) {
                sprintf(value, "%.2f", load->one_min);
            } else if (contains(cur, "entities")) {
                sprintf(value, "%s", load->entities);
            } else if (contains(cur, "pid")) {
                sprintf(value, "%d", load->newest_proc);
            } else {
                free(value);
                value = NULL;
            }
        }

        if (value != NULL) {
            sprintf(out[i], "%s%s%s%s", spacer, prefix, value, spacer);
            free(value);
        }
    }

    if (bat != NULL) free(bat);

    return out;
}
