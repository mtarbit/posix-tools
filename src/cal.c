#include "pos.h"
#include <time.h>

void usage(const char *program_name) {
    die_msg("Usage: %s [[month] year]\n", program_name);
}

int get_arg_month(const char *str) {
    int m = atoi(str);
    if (m < 1 || m > 12) {
        die_msg("%s is not a valid month in the range 1 to 12\n", str);
    }
    return m;
}

int get_arg_year(const char *str) {
    int y = atoi(str);
    if (y < 1 || y > 9999) {
        die_msg("%s is not a valid year in the range 1 to 9999\n", str);
    }
    return y;
}

int main(int argc, char *argv[]) {
    int y = 0;
    int m = 0;

    time_t current_ts = time(NULL);
    time_t mon_day_ts;
    time_t mon_end_ts;

    struct tm current_tm;
    struct tm mon_day_tm;
    struct tm mon_end_tm;

    char mon_str[21];
    char day_str[3];
    int n, r, c;

    if (argc > 3) {
        usage(argv[0]);
    } else if (argc == 3) {
        m = get_arg_month(argv[1]);
        y = get_arg_year(argv[2]);
    } else if (argc == 2) {
        y = get_arg_year(argv[1]);
    }

    localtime_r(&current_ts, &current_tm);

    mon_day_tm = current_tm;
    mon_day_tm.tm_mday = 1;
    if (m) mon_day_tm.tm_mon = -1 + m;
    if (y) mon_day_tm.tm_year = -1900 + y;
    mon_day_ts = mktime(&mon_day_tm);

    mon_end_tm = mon_day_tm;
    mon_end_tm.tm_mon++;
    mon_end_ts = mktime(&mon_end_tm);

    n = strftime(mon_str, sizeof mon_str, "%B %Y", &mon_day_tm);
    printf("%*s\n", (int)((sizeof mon_str) + n) / 2, mon_str);
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
