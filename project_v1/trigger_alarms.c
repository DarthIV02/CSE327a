#define _XOPEN_SOURCE 700  // Required for strptime()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <time.h>
#include "window.h"
#include "trigger_alarms.h"

typedef struct {
    char name[45];
    struct tm start;
    struct tm repeat; 
    struct tm taken;
} Medicine;

int num_medicine;
Medicine *medicines = NULL;  // Array of Medicine structs

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
            strptime(repeat->valuestring, "%H:%M", &med->repeat);
        }
        if (cJSON_IsString(start) && start->valuestring != NULL) {
            strptime(start->valuestring, "%Y-%m-%d %H:%M", &med->start);
        }
        if (cJSON_IsString(last_rec) && last_rec->valuestring != NULL) {
            strptime(last_rec->valuestring, "%Y-%m-%d %H:%M", &med->taken);
        }
        medicine_count += 1;

    }

    // Delete the JSON object to free memory
    cJSON_Delete(json);

    return 0;
}

int update_taken_json(int item_change, struct tm new_time){

    int i = 0;
    // Buffer to hold the formatted string
    char buffer_time[80];
    
    FILE *fp = fopen("user_data/schedule.json", "r");
    if (fp == NULL) {
        printf("Error: Unable to open file.\n");
        return 1;
    }

    // Read the entire file into a buffer
    char buffer[1024];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[len] = '\0'; // Null-terminate the buffer
    fclose(fp);

    // Parse the JSON data
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("Error parsing JSON.\n");
        return 1;
    }

    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {
        if (i == item_change){
            // Update the "last_rec" field
            cJSON *last_rec = cJSON_GetObjectItemCaseSensitive(item, "last_rec");
            if (last_rec != NULL) {
                // Format the time using strftime()
                strftime(buffer_time, sizeof(buffer_time), "%Y-%m-%d %H:%M", &new_time);
                cJSON *new_string_item = cJSON_CreateString(buffer_time);
                cJSON_ReplaceItemInObject(item, "last_rec", new_string_item);
            } else {
                printf("Key 'last_rec' not found.\n");
            }

            // Open the file for writing
            fp = fopen("user_data/schedule.json", "w");
            if (fp == NULL) {
                printf("Error: Unable to open file for writing.\n");
                cJSON_Delete(json);
                return 1;
            }

            // Convert the modified JSON object back to a string
            char *updated_json = cJSON_Print(json);
            if (updated_json == NULL) {
                printf("Error printing JSON.\n");
                cJSON_Delete(json);
                return 1;
            }

            // Write the updated JSON back to the file
            fwrite(updated_json, 1, strlen(updated_json), fp);
            fclose(fp);

            // Clean up
            cJSON_Delete(json);
            free(updated_json);
            return 1;
        }
        i += 1;
    }
}

int isMedtriggered(Medicine med, struct tm current_time){

    time_t current = mktime(&current_time);
    time_t last = mktime(&med.taken);

    // Debug
    /*printf("Last time med taken: %s\n", ctime(&last));
    printf("Difference: %lf\n", difftime(current, last));*/

    if (difftime(current, last) > (med.repeat.tm_hour * 60 * 60 + med.repeat.tm_min * 60)){
        //Debugging print
        printf("Medicine %s triggered at time: %d:%d\n", med.name, current_time.tm_hour, current_time.tm_min);
        
        return 1;
    }

    return 0;
}

void* countdown_alarms() {
    struct tm last_dt = {0}; // Initialize to -1;

    // Read JSON to find active medicines
    read_json();
    //Debugging purposes

    int medicine_active[num_medicine]; // Medicine needs to be taken today
    int medicine_triggered[num_medicine]; // Medicine flag is triggered
    int window_changed = 0;

    while (1) {
        //Debugging purposes
        struct tm current_dt = get_time_from_hwclock();
        if (current_dt.tm_min == 0) {
            sleep(10); // Retry after 10 seconds if there was an error
            continue;
        }

        if (last_dt.tm_mday == 0 || current_dt.tm_mday != last_dt.tm_mday){ 
            // If its a new day -> check if medicine needs to be taken today
            //printf("Checked day\n");

            for(int i = 0; i < num_medicine; i++){
                Medicine med = medicines[i];
                struct tm temp = med.taken;
                temp.tm_hour += med.repeat.tm_hour;
                temp.tm_min += med.repeat.tm_min;           
                if (temp.tm_yday <= current_dt.tm_yday){
                    medicine_active[i] = 1;
                } else {
                    medicine_active[i] = 0;
                }
            }
        }

        //Debugging purposes
        /*printf("Medicine Active: [ ");
        for (int i = 0; i < num_medicine; i++) {
            printf("%d ", medicine_active[i]);  // Print each element
        }
        printf("]\n");*/

        for(int i = 0; i < num_medicine; i++){
            // Only check active medicines every minute
            if (medicine_active[i] == 1){
                medicine_triggered[i] = isMedtriggered(medicines[i], current_dt);
                if(window_changed == 0 && medicine_triggered[i] == 1){
                    change_alarm(1);
                    window_changed = 1;
                }
            }
        }

        //Debugging purposes
        /*printf("Medicine Triggered: [ ");
        for (int i = 0; i < num_medicine; i++) {
            printf("%d ", medicine_triggered[i]);  // Print each element
        }
        printf("]\n---------------------\n");*/

        /*MEDICINE FLAG IS TRIGGERED -- ACTION ADDED HERE*/
        sleep(10);

        //Medicine has been taken
        for(int i = 0; i < num_medicine; i++){
            // Only check active medicines every minute
            if (medicine_triggered[i] == 1){
                medicines[i].taken = current_dt;
                update_taken_json(i, current_dt);
                medicine_triggered[i] = 0;
            }
        }
        if(window_changed == 1){
            change_alarm(0);
            window_changed = 0;
        }

        last_dt = current_dt;

        sleep(60); // Sleep for 30 seconds before checking again
    }

    return NULL;
}
