/* 

BSD 2-Clause License

Copyright (c) 2022, Leonardo A. Alvarenga
All rights reserved.

*/

#include "config.h"

char* join_path(const char *base, const char *dir) {
    unsigned int len = strlen(base);
    
    char *dest = (char *)(malloc(sizeof(char)*(strlen(base) + strlen(dir) + 2)));

    if (len > 0 && dest[len-1] != '/') sprintf(dest, "%s/%s", base, dir);
    else sprintf(dest, "%s%s", base, dir);

    return dest;
}

BatteryInfo* get_battery_info() {
    if (!has_battery) return NULL;

    int len;
    char *status_path = join_path(battery_device, "status");
    char *charge_now_path = join_path(battery_device, "charge_now");
    char *charge_full_path = join_path(battery_device, "charge_full");
    char *charge_full_design_path = join_path(battery_device, "charge_full_design");

    FILE *f;
    BatteryInfo *bat = (BatteryInfo *)(malloc(sizeof(BatteryInfo)));

    if (bat == NULL) return NULL;

    f = fopen(status_path, "r");
    bat->status = (char *)(malloc(sizeof(char)*20 + 2));
    fgets(bat->status, sizeof(char)*20, f);
    fclose(f);

    len = strlen(bat->status);
    if (bat->status[len-1] == '\n')
        bat->status = strndup(bat->status, len-1);

    f = fopen(charge_now_path, "r");
    fscanf(f, "%d", &bat->cur_charge);
    fclose(f);

    f = fopen(charge_full_path, "r");
    fscanf(f, "%d", &bat->max_charge);
    fclose(f);

    f = fopen(charge_full_design_path, "r");
    fscanf(f, "%d", &bat->des_charge);
    fclose(f);

    bat->health = (bat->max_charge * 100) / bat->des_charge;
    bat->percentage = (bat->cur_charge * 100) / bat->max_charge;

    free(status_path);
    free(charge_now_path);
    free(charge_full_path);
    free(charge_full_design_path);

    return bat;
}

LoadAvg* get_loadavg() {
    LoadAvg *avg = (LoadAvg*)(malloc(sizeof(LoadAvg)));
    if (avg == NULL) return NULL;

    FILE *f;
    f = fopen(loadavg_path, "r");

    int pid;
    char *ent;
    float one, five, fifteen;

    ent = (char*)(malloc(sizeof(char) * 8));
    fscanf(f, "%f %f %f %s %d", &one, &five, &fifteen, ent, &pid);
    fclose(f);

    avg->one_min = one;
    avg->five_min = five;
    avg->fifteen_min = fifteen;
    avg->entities = ent;
    avg->newest_proc = pid;

    return avg;
}
