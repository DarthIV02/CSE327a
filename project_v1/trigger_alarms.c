#include <stdio.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main() {
    char buffer[128];
    FILE *fp;

    // Execute the hwclock command and capture the output
    fp = popen("sudo hwclock -r", "r");
    if (fp == NULL) {
        perror("Failed to run hwclock");
        return 1;
    }

    // Read the output from the hwclock command
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("RTC Time: %s", buffer);
    }

    // Close the file pointer
    fclose(fp);

    return 0;
}
