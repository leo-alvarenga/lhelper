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

char* get_local_time(const char *fmt, const char *tz) {
    char buf[135];
    
    char *out = (char *)(malloc(sizeof(char)*135));

    setenv("TZ", tz, 1);
    
    time_t tim;
    struct tm *timtm;
    tim = time(NULL);
    timtm = localtime(&tim);

    if (timtm == NULL)
        return "";

    if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
        return "?";
    }
  
  sprintf(out,"%s", buf);
  return out;
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

UptimeInfo* get_uptime() {
    UptimeInfo *up = (UptimeInfo *)(malloc(sizeof(UptimeInfo)));
    
    int time, _;
    FILE *f = fopen(uptime_path, "r");

    fscanf(f, "%d %d", &time, &_);
    fclose(f);

    up->hrs = time / 3600;
	time = time % 3600;

	up->min = time / 60;
	time = time % 60;

    up->s = time;

	if (up->hrs > 24){
		up->days = up->hrs / 24;
		up->hrs = up->hrs % 24;
	} else {
        up->days = 0;
    }

    return up;
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

BacklightInfo* get_backlight_info() {
    const char *bright = join_path(backlight_path, "brightness");
    const char *max = join_path(backlight_path, "max_brightness");
    if (max == NULL || bright == NULL) return NULL;

    BacklightInfo *back = (BacklightInfo *)(malloc(sizeof(BacklightInfo)));
    FILE *f;

    f = fopen(bright, "r");
    if (f == NULL) return NULL;

    fscanf(f, "%d", &(back->brightness));
    fclose(f);

    f = fopen(max, "r");
    if (f == NULL) return NULL;

    fscanf(f, "%d", &(back->max_brightness));
    fclose(f);

    back->brightness_rate = (back->brightness * 100) / back->max_brightness;

    return back;
};
