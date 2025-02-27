#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int get_minutes_from_hwclock() {
    char buffer[128];
    FILE *fp;

    // Execute the hwclock command
    fp = popen("sudo hwclock -r", "r");
    if (fp == NULL) {
        perror("Failed to run hwclock");
        return -1;
    }

    // Read the output
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        perror("Failed to read hwclock output");
        pclose(fp);
        return -1;
    }

    pclose(fp);

    // Extract the minute from the time string
    int year, month, day, hour, minute, second;
    if (sscanf(buffer, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) == 6) {
        return minute; // Return the extracted minute
    } else {
        fprintf(stderr, "Failed to parse time format\n");
        return -1;
    }
}

int main() {
    int last_minute = -1;

    while (1) {
        int current_minute = get_minutes_from_hwclock();
        if (current_minute == -1) {
            sleep(10); // Retry after 10 seconds if there was an error
            continue;
        }

        // Check if 5 minutes have passed
        if (last_minute == -1 || (current_minute - last_minute + 60) % 60 >= 5) {
            printf("5 minutes have passed! Current time from RTC: %d minutes\n", current_minute);
            last_minute = current_minute;
        }

        sleep(30); // Sleep for 30 seconds before checking again
    }

    return 0;
}
