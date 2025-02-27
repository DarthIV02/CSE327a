#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int year, month, day;
    int hour, minute, second;
} DateTime;

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
    DateTime dt = {0}; // Initialize to zero
    int year, month, day, hour, minute, second;
    if (sscanf(buffer, "%d-%d-%d %d:%d:%d", &dt.year, &dt.month, &dt.day, &dt.hour, &dt.minute, &dt.second) != 6) 
    {
        fprintf(stderr, "Failed to parse date-time.\n");
        exit(EXIT_FAILURE);
    }
    return dt;
}

int main() {
    int last_minute = -1;

    while (1) {
        DateTime current_dt = get_minutes_from_hwclock();
        if (current_dt.minute == -1) {
            sleep(10); // Retry after 10 seconds if there was an error
            continue;
        }

        // Check if 5 minutes have passed
        if (last_minute == -1 || (current_dt.minute - last_minute + 60) % 60 >= 1) {
            printf("5 minutes have passed! Current time from RTC: %d minutes\n", current_minute);
            last_minute = current_dt.minute;
        }

        sleep(30); // Sleep for 30 seconds before checking again
    }

    return 0;
}
