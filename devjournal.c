#include <stdio.h>
#include  <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRING_LENGTH 1000

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(){
    FILE *file;
    char date[12];
    char subject[MAX_STRING_LENGTH];
    char achievement[MAX_STRING_LENGTH];


    //Get the current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    //user input
    printf("\nWelcome to your Dev Journal\n");
    printf("\nDate: %s\n", date);
    printf("\nWhat are you learning? Which language do you wanna talk about?\n");
    fgets(subject, sizeof(subject), stdin);
    clearInputBuffer(); 
    printf("What did you learn/achieve?\n");
    fgets(achievement, sizeof(achievement), stdin);
    clearInputBuffer();

    //open file for appending
    file = fopen("deventries.csv", "a");
    if (file == NULL){
        printf("ERROR!There was an error your saving entry!\n");
        printf("\nPlease close the entries file if it is open in another application.\n");
        printf("\nPress ENTER to exit.\n");
        clearInputBuffer();

        return 1;
    }
    fprintf(file, "%s,%s,%s\n",date, subject, achievement);
    printf("Entry saved. ;)\n");
    fclose(file);
    printf("\nPress ENTER to exit.\n");
    clearInputBuffer();
}