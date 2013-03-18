#include "pos.h"
#include <time.h>

int main(int argc, char *argv[]) {
    time_t current_ts = time(NULL);
    time_t mon_day_ts;
    time_t mon_end_ts;

    struct tm current_tm;
    struct tm mon_day_tm;
    struct tm mon_end_tm;

    int mon_str_max = 21;
    char mon_str[mon_str_max];
    char day_str[3];
    int n, r, c;

    localtime_r(&current_ts, &current_tm);

    mon_day_tm = current_tm;
    mon_day_tm.tm_mday = 1;
    mon_day_ts = mktime(&mon_day_tm);

    mon_end_tm = mon_day_tm;
    mon_end_tm.tm_mon++;
    mon_end_ts = mktime(&mon_end_tm);

    n = strftime(mon_str, mon_str_max, "%B %Y", &current_tm);
    printf("%*s\n", n + ((mon_str_max - n) / 2), mon_str);
    printf("Su Mo Tu We Th Fr Sa\n");

    for (r = 0; r < 6; r++) {
        for (c = 0; c < 7; c++) {
            if ((r == 0 && c < mon_day_tm.tm_wday) || mon_day_ts >= mon_end_ts) {
                snprintf(day_str, 3, "  ");
            } else {
                snprintf(day_str, 3, "%2d", mon_day_tm.tm_mday);
                mon_day_tm.tm_mday++;
                mon_day_ts = mktime(&mon_day_tm);
            }
            printf("%s%c", day_str, (c < 6) ? ' ' : '\n');
        }
    }

    exit(EXIT_SUCCESS);
}
