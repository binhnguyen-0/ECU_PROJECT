
#include "csv_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define CSV_FILE "data.csv"

/*****************************************************************************
 * @fn				- csv_getInt
 *
 * @brief			- This function retrieves an integer value from a CSV file.
 *
 * @param[in]		- const char* key: The key to search for in the CSV file.
 *
 * @return			- The integer value associated with the key, or -1 if not found.
 *
 */
int csv_getInt(const char* key) {

    if (key == NULL) {
        fprintf(stderr, "csv_getInt: NULL key provided.\n");
        return -1;
    }

    FILE* file = fopen(CSV_FILE, "r");
    if (!file) {
        perror("csv_getInt: Failed to open CSV file");
        return -1; // or some error value
    }   

    // Buffer to hold each line read from the CSV file
    char line[BUFFER_SIZE];
    
    // Read each line from the CSV file
    while (fgets(line, sizeof(line), file)) {

        char line_copy[BUFFER_SIZE];
        strncpy(line_copy, line, sizeof(line_copy));
        line_copy[sizeof(line_copy) - 1] = '\0'; // Ensure null termination

        char* token_key = strtok(line_copy, ",");
        char* token_value = strtok(NULL, "\n");
        
        if (token_key && token_value && strcmp(token_key, key) == 0) {
            char* endptr;
            errno = 0; // Reset errno before strtol
            long value = strtol(token_value, &endptr, 10);
            if (errno != 0 || endptr == token_value || *endptr != '\0') {
                fprintf(stderr, "csv_getInt: Invalid integer value for key '%s': %s\n", key, token_value);
                fclose(file);
                return -1;
            }
            fclose(file);
            return (int)value;
        }
    }
    // Key was not found
    fclose(file);
    fprintf(stderr, "csv_getInt: Key '%s' not found in CSV file.\n", key);
    return -1; // or some error value if key not found
}

/*****************************************************************************
 * @fn				- csv_setInt
 *
 * @brief			- This function sets an integer value in a CSV file.
 *
 * @param[in]		- const char* key: The key to set the value for.
 * @param[in]		- int value: The integer value to set.
 *
 * @return			- void
 *
 */
void csv_setInt(const char* key, int value) {

    if(key == NULL) {
        fprintf(stderr, "csv_setInt: NULL key provided.\n");
        return -1;
    }
    FILE* raw_file = fopen(CSV_FILE, "r");
    FILE* temp_file = fopen("temp.csv", "w");
    if (!temp_file){
        perror("csv_setInt: Failed to open temporary CSV file");
        if (raw_file) fclose(raw_file);
        return;
    }

    char line[BUFFER_SIZE];
    int found = 0;  

    if (raw_file) {
        while (fgets(line, sizeof(line), raw_file)) {
            char line_copy[BUFFER_SIZE];
            strncpy(line_copy, line, sizeof(line_copy));
            line_copy[sizeof(line_copy) - 1] = '\0'; // Ensure null termination

            char* token_key = strtok(line_copy, ",");
            if (token_key && strcmp(token_key, key) == 0) {
                fprintf(temp_file, "%s,%d\n", key, value);
                found = 1; // Key found and updated
            } else {
                fputs(line, temp_file); // Write the original line
            }
        }
        fclose(raw_file);
    }
    if (!found) {
        fprintf(temp_file, "%s,%d\n", key, value); // Add new key-value pair
    }

    fclose(temp_file);

    if (remove(CSV_FILE) != 0 || rename("temp.csv", CSV_FILE) != 0) {
        perror("csv_setInt: Failed to replace temporary CSV file");
    }
}

/*****************************************************************************
 * @fn				- csv_getString
 *
 * @brief			- This function retrieves a string value from a CSV file.
 *
 * @param[in]		- const char* key: The key to search for in the CSV file.
 *
 * @return			- The string value associated with the key, or NULL if not found.
 *
 */
const char* csv_getString(const char* key) {
    if (key == NULL) {
        fprintf(stderr, "csv_getString: NULL key provided.\n");
        return NULL;
    }

    FILE* file = fopen(CSV_FILE, "r");
    if (!file) {
        perror("csv_getString: Failed to open CSV file");
        return NULL;
    }   

    // Buffer to hold each line read from the CSV file
    char line[BUFFER_SIZE];
    
    // Read each line from the CSV file
    while (fgets(line, sizeof(line), file)) {

        char line_copy[BUFFER_SIZE];
        strncpy(line_copy, line, sizeof(line_copy));
        line_copy[sizeof(line_copy) - 1] = '\0'; // Ensure null termination

        char* token_key = strtok(line_copy, ",");
        char* token_value = strtok(NULL, "\n");

        if (token_key && token_value && strcmp(token_key, key) == 0) {
            char* result = strdup(token_value);  // malloc + strcpy
            fclose(file);
            return result;
        }
    }
    // Key was not found
    fclose(file);
    fprintf(stderr, "csv_getString: Key '%s' not found in CSV file.\n", key);
    return NULL;
}
