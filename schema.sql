-- Create the Entries table for the journal
CREATE TABLE IF NOT EXISTS Entries (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    Date TEXT NOT NULL,
    Subject TEXT NOT NULL,
    Achievement TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT (datetime('now', 'localtime'))
);
