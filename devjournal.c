#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>

// Function to read a line of input dynamically
char *read_line(FILE *stream) {
    size_t size = 64;  // Initial buffer size
    size_t len = 0;    // Length of the input
    char *buffer = malloc(size);  // Allocate initial buffer
    if (!buffer) return NULL;     // Check for allocation failure

    while (fgets(buffer + len, size - len, stream)) {
        len += strlen(buffer + len);  // Update length of input
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';  // Remove newline
            return buffer;
        }

        // Resize buffer if needed
        size *= 2;
        char *new_buffer = realloc(buffer, size);
        if (!new_buffer) {
            free(buffer);
            return NULL;
        }
        buffer = new_buffer;
    }

    // Handle EOF or error
    if (len == 0) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

int main() {
    char date[12];
    char *subject = NULL;
    char *achievement = NULL;

    // Get the current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Get the user input
    printf("\nWelcome to your Journal, dear Christian Cecil ;)\n");
    printf("\nDate: %s\n", date);
    printf("\nWhat are you learning? Which language do you wanna talk about?\n");
    subject = read_line(stdin);
    if (!subject) {
        fprintf(stderr, "Error reading topic.\n");
        return 1;
    }
    printf("What did you learn or achieve?\n");
    achievement = read_line(stdin);
    if (!achievement) {
        fprintf(stderr, "Error reading achievement.\n");
        free(subject);
        return 1;
    }

    // Write to SQLite database
    sqlite3 *db;
    char *err_msg = 0;

    // Open the database
    int rc = sqlite3_open("journal.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(subject);
        free(achievement);
        return 1;
    }

    // Prepare the INSERT statement
    char *insert_sql = "INSERT INTO Entries (Date, Subject, Achievement) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(subject);
        free(achievement);
        return 1;
    }

    // Bind values to the statement
    sqlite3_bind_text(stmt, 1, date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, subject, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, achievement, -1, SQLITE_STATIC);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Entry saved to journal, dear Cecil ;)\n");
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Free dynamically allocated memory
    free(subject);
    free(achievement);

    return 0;
}