#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>

typedef struct {
    int year, month, day;
    int hour, minute, second;
} DateTime;

typedef struct {
    char name[45];
    int start_year, start_month, start_day;
    int repeat_hour, repeat_minute;
    int taken_hour, taken_minute;
} Medicine;

int num_medicine;
Medicine *medicines = NULL;  // Array of Medicine structs

DateTime get_minutes_from_hwclock() {
    char buffer[128];
    FILE *fp;
    DateTime dt = {0}; // Initialize to zero

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
    int year, month, day, hour, minute, second;
    if (sscanf(buffer, "%d-%d-%d %d:%d:%d", &dt.year, &dt.month, &dt.day, &dt.hour, &dt.minute, &dt.second) != 6) 
    {
        fprintf(stderr, "Failed to parse date-time.\n");
        exit(EXIT_FAILURE);
    }
    return dt;
}

int parse_time(const char *time_str, int *hour, int *minute) {
    return sscanf(time_str, "%d:%d", hour, minute);  // Parses HH:MM format
}

int parse_date(const char *date_str, int *year, int *month, int *day) {
    return sscanf(date_str, "%d-%d-%d", year, month, day);  // Parses YYYY-MM-DD format
}

int read_json(){
    // Open the file
    FILE *fp = fopen("user_data/schedule.json", "r");
    if (fp == NULL) {
        printf("Error: Unable to open the file.\n");
        return 1;
    }

    // Read the file contents into a string
    char buffer[1024];  // Buffer to hold the JSON data (adjust size if necessary)
    int len = fread(buffer, 1, sizeof(buffer) - 1, fp);  // Read file contents
    buffer[len] = '\0';  // Ensure the string is null-terminated
    fclose(fp);

    // Parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("Error: %s\n", error_ptr);
        }
        return 1;
    }

    num_medicine = cJSON_GetArraySize(json);
    int medicine_count = 0;

    // Get the number of medicines in the JSON object
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {

        // Reallocate memory for the array
        medicines = realloc(medicines, sizeof(Medicine) * (medicine_count + 1));
        if (medicines == NULL) {
            printf("Memory allocation error\n");
            return 1;
        }

        // Parse and store the values
        Medicine *med = &medicines[medicine_count];  // Declare a Medicine struct
        // Each item is an object, now let's get the individual fields
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *repeat = cJSON_GetObjectItemCaseSensitive(item, "repeat");
        cJSON *start = cJSON_GetObjectItemCaseSensitive(item, "start");
        cJSON *last_rec = cJSON_GetObjectItemCaseSensitive(item, "last_rec");

        // Ensure the fields are not null before accessing their values
        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(med->name, name->valuestring, sizeof(med->name) - 1);
        }
        if (cJSON_IsString(repeat) && repeat->valuestring != NULL) {
            parse_time(repeat->valuestring, &med->repeat_hour, &med->repeat_minute);
        }
        if (cJSON_IsString(start) && start->valuestring != NULL) {
            parse_date(start->valuestring, &med->start_year, &med->start_month, &med->start_day);
        }
        if (cJSON_IsString(last_rec) && last_rec->valuestring != NULL) {
            parse_time(last_rec->valuestring, &med->taken_hour, &med->taken_minute);
        }
        medicine_count += 1;

    }

    // Delete the JSON object to free memory
    cJSON_Delete(json);

    return 0;
}

int main() {
    DateTime last_dt = {-1}; // Initialize to zero = -1;

    // Read JSON to find active medicines
    

    while (1) {
        DateTime current_dt = get_minutes_from_hwclock();
        if (current_dt.minute == -1) {
            sleep(10); // Retry after 10 seconds if there was an error
            continue;
        }

        // Check if 5 minutes have passed
        if (last_dt.minute == -1 || (current_dt.minute - last_dt.minute + 60) % 60 >= 1) {
            printf("1 minutes have passed! Current time from RTC: %d minutes\n", current_dt.minute);
            last_dt.minute = current_dt.minute;
        }

        sleep(30); // Sleep for 30 seconds before checking again
    }

    return 0;
}
