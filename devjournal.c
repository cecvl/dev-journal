#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
#include <sys/stat.h>
#include <unistd.h>

// Function to get the database path in user's home directory
char *get_db_path() {
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Cannot determine HOME directory\n");
        return NULL;
    }
    
    // Create data directory path: ~/.local/share/devjournal
    char *data_dir = malloc(strlen(home) + 30);
    sprintf(data_dir, "%s/.local/share/devjournal", home);
    
    // Create directory if it doesn't exist
    mkdir(data_dir, 0755);
    
    // Create full database path
    char *db_path = malloc(strlen(data_dir) + 20);
    sprintf(db_path, "%s/journal.db", data_dir);
    
    free(data_dir);
    return db_path;
}

// Function to initialize database schema
int init_database(sqlite3 *db) {
    const char *schema = 
        "CREATE TABLE IF NOT EXISTS Entries ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Date TEXT NOT NULL, "
        "Subject TEXT NOT NULL, "
        "Achievement TEXT NOT NULL, "
        "created_at TIMESTAMP DEFAULT (datetime('now', 'localtime'))"
        ");";
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, schema, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }
    return 0;
}

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

    // Get database path
    char *db_path = get_db_path();
    if (!db_path) {
        free(subject);
        free(achievement);
        return 1;
    }

    // Open the database
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(db_path);
        free(subject);
        free(achievement);
        return 1;
    }

    // Initialize database schema
    if (init_database(db) != 0) {
        sqlite3_close(db);
        free(db_path);
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
    free(db_path);
    free(subject);
    free(achievement);

    return 0;
}