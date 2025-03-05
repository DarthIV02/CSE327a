#define _XOPEN_SOURCE 700  // Required for strptime()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <time.h>
#include "get_hw.h"

struct tm get_time_from_hwclock() {
    char buffer[128];
    FILE *fp;
    struct tm dt = {0}; // Initialize to zero

    // Execute the hwclock command
    fp = popen("sudo hwclock -r", "r");
    if (fp == NULL) {
        perror("Failed to run hwclock");
        return dt;
    }

    // Read the output
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        perror("Failed to read hwclock output");
        pclose(fp);
        return dt;
    }

    pclose(fp);

    // Extract the minute from the time string
    if (strptime(buffer, "%Y-%m-%d %H:%M:%S", &dt) == NULL) {
        printf("Failed to parse date string\n");
    }
    return dt;
}