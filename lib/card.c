/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <conio.h> // Console I/O (non-standard)
#include <math.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "./../config.c"

#include "./../model/User.c"

#include "./key.c"
#include "./array.c"
#include "./sort.c"

#include "./file.c"
#include "./search.c" */

typedef struct Card
{
    int status;
    User *data;

} Card;


Card readCard(const char *filename)
{
    const char *FIELD_NAME[] = {
        "ID",
        "First Name",
        "Last Name",
    };

    FILE *file = fopen(filename, "r");

    Table csvDataTable;
    User userData;
    Card CardDetail;

    if (!file)
    {
        perror("Error opening file");
        CardDetail.status = 0;

        return CardDetail;
    }


    char line[MAX_LINE_SIZE];
    char *fieldNames[MAX_FIELDS]; // Array to store field names
    int fieldCount = 1;
    int currentRow = 1; // Initialize the row number

    // Read the first line to get field names
    if (fgets(line, sizeof(line), file))
    {

        char *token = strtok(line, ",");
        while (token)
        {
            
            // Store the field name in the array
            fieldNames[fieldCount] = strdup(token);
            
            if (fieldCount == 1)
                copyTo(userData.id, token, sizeof(userData.id));

            if (fieldCount == 2)
                copyTo(userData.fname, token, sizeof(userData.fname));

            if (fieldCount == 3)
                copyTo(userData.lname, token, sizeof(userData.lname));

            // Remove trailing newline character, if present
            size_t len = strlen(fieldNames[fieldCount]);
            if (fieldNames[fieldCount][len - 1] == '\n')
            {
                fieldNames[fieldCount][len - 1] = '\0';
            }

            // Get the next token
            token = strtok(NULL, ",");
            fieldCount++;
        }
    }

    fclose(file);

    printf("\nReaded Card [/]\n");
    CardDetail.status = 1;
    CardDetail.data = &userData;

    return CardDetail;
}

Card checkInsertCard(){

    char *fileName = "./card/SIAM-ID/Data.txt";
    Card userCardData; 
    userCardData.status = 0;

    if (isfileExists(fileName) == 1){
        
            userCardData = readCard(fileName);


            if (userCardData.status)
            {

             /*    printf("ID          :    %s\n", userCardData.data.id);
                printf("First Name  :    %s\n", userCardData.data.fname);
                printf("Last Name   :    %s\n", userCardData.data.lname) */;
                SearchData foundUser = searchID(userCardData.data->id);
                userCardData.data = foundUser.user;
                remove(fileName);
                return userCardData;
            }
        }
}

/* void main()
{



  

} */