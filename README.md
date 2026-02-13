# Dev Journal

Simple console application for tracking your learning achievements. This program enables you to create journal entries with the current date, the subject you are learning, and your daily achievements. The entries are saved in a database.

Use an appropriate compiler to build the system, considering your operating system and preferences.
Upon running the program, you will be prompted to enter the subject you are learning and your daily achievements.

`Use of sqlcipher:` **Accessing the database** will require a password which is set after the first journal entry;

**PRAGMA key = 'your_password';** 

**SELECT * FROM Entries;**

## make commands
`make install` -  install program and database in system

`make uninstall` - uninstall program

`make run` - run bin

`make cleanall` - delete database & bin


### Additional resources

[Using sqlite in C](https://zetcode.com/db/sqlitec/)

[Sqlite Commands](https://www.sqlitetutorial.net/sqlite-commands//)
