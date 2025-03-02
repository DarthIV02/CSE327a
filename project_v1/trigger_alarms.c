#define _XOPEN_SOURCE 700  // Required for strptime()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <time.h>

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

int write_json(){

}

int isMedtriggered(Medicine med, struct tm current_time){

    time_t current = mktime(&current_time);
    time_t last = mktime(&med.taken);

    print("Med %s needs to be taken every %d h %d m\n", med.name, med.repeat.tm_hour, med.repeat.tm_min);

    if (difftime(current, last) > (med.repeat.tm_hour * 60 * 60 + med.repeat.tm_min * 60)){
        return 1;

        //Debugging print
        printf("Medicine %c triggered at time: %d:%d\n", med.name, current_time.tm_hour, current_time.tm_min);
    }

    return 0;
}

int main() {
    printf("Start");
    struct tm last_dt = {0}; // Initialize to -1;

    // Read JSON to find active medicines
    read_json();
    //Debugging purposes

    int medicine_active[num_medicine]; // Medicine needs to be taken today
    int medicine_triggered[num_medicine]; // Medicine flag is triggered

    while (1) {
        //Debugging purposes
        struct tm current_dt = get_time_from_hwclock();
        if (current_dt.tm_min == 0) {
            sleep(10); // Retry after 10 seconds if there was an error
            continue;
        }

        if (last_dt.tm_mday == 0 || current_dt.tm_mday != last_dt.tm_mday){ 
            // If its a new day -> check if medicine needs to be taken today

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
        printf("Medicine Active: [ ");
        for (int i = 0; i < num_medicine; i++) {
            printf("%d ", medicine_active[i]);  // Print each element
        }
        printf("]\n");

        for(int i = 0; i < num_medicine; i++){
            // Only check active medicines every minute
            if (medicine_active[i] = 1){
                medicine_triggered[i] = isMedtriggered(medicines[i], current_dt);
            }
        }

        //Debugging purposes
        printf("Medicine Triggered: [ ");
        for (int i = 0; i < num_medicine; i++) {
            printf("%d ", medicine_triggered[i]);  // Print each element
        }
        printf("]\n");

        /*MEDICINE FLAG IS TRIGGERED -- ACTION ADDED HERE*/

        //Medicine has been taken
        for(int i = 0; i < num_medicine; i++){
            // Only check active medicines every minute
            if (medicine_triggered[i] = 1){
                medicines[i].taken = current_dt;
                medicine_triggered[i] = 0;
            }
        }

        /*MISSING UPDATING THE JSON FILE*/


        last_dt = current_dt;

        sleep(10); // Sleep for 30 seconds before checking again
    }

    return 0;
}
