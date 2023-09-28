//* About Contact to CSV FILE
//* CREATE 
//* READ 
//* UPDATE 
//* CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <conio.h> // Console I/O (non-standard)
#include <math.h>
#include <sys/time.h>
#include <sys/stat.h>


#define USERS_DATA "./data/Users.csv"
#define ACCOUNTS_DATA "./data/Accounts.csv"

//*---------------------------------
#define MAX_LINE_SIZE 1024
#define MAX_FIELDS 15
#define MAX_ROW 30000
#define MAX_LIST_ROW 10
#define MAX_ACCCOUNT_ID_SIZE 10

//*---------------------------------

char *USER_MENU[] = {
    "EDIT",
    "DELETE",
    "ACTIVE && SUSPEND CARD",
    "ENABLE && DISABLE CARD"
};

const char *FIELD_NAME[] = {
    "ID",
    "AccountID",
    "First Name",
    "Last Name",
    "Tel",
    "Balance",
    "Active",
    "Status",
    "Register Time",
    "Password"
};

const char *FIELD_TYPE[] = {
    "%s",
    "%s",
    "%s",
    "%s",
    "%s",
    "%.2lf",
    "%d",
    "%d",
    "%s",
    "%s"
};


int USER_ARR_SIZE = 0;
int FIELD_NAME_SIZE = sizeof(FIELD_NAME) / sizeof(FIELD_NAME[0]);
int FIELD_TYPE_SIZE = sizeof(FIELD_TYPE) / sizeof(FIELD_TYPE[0]);
int USER_MENU_SIZE = sizeof(USER_MENU) / sizeof(USER_MENU[0]);

UserNode *USER_LIST = NULL;
User *USER_ARR = NULL;
time_t lastestTime;

void saveLinkedListToCSV(const char *filename, UserNode *head);
void saveUser(const char *filename, const User *userData);
void appendToCSV(const char *filename, const User *userData);
void copyTo(char *dest, const char *src, size_t destSize);
void concatenateWithCommas(const char *fieldNames[], int fieldSize, char *result);

int getListSize(UserNode *list);
int isFileChanged(const char *filename, time_t *lastModifiedTime);
int checkFileChange(const char *filename, float interval);
int checkFileChangeOnce(const char *filename);
int isfileExists(const char *filename);
int getListSize(UserNode *list);

char *getCurrentTime();

Table processCSVToLinkedList(const char *filename, int choice);
User *linkedListToArray(UserNode *head, int linkSize, int *arraySize);



void saveLinkedListToCSV(const char *filename, UserNode *head)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char headerFile[100];
    char dataType[100];

    concatenateWithCommas(FIELD_NAME, FIELD_NAME_SIZE, headerFile);
    concatenateWithCommas(FIELD_TYPE, FIELD_TYPE_SIZE, dataType);

    // Write the header line
    fprintf(file, strcat(headerFile, "\n"));

    UserNode *current = head;
    while (current != NULL)
    {
        User user = current->data;

        fprintf(file, dataType,
                user.id,
                user.accountID,
                user.fname,
                user.lname,
                user.tel,
                user.balance,
                user.active,
                user.status,
                user.registerTime,
                user.password

        );

        fprintf(file, "\n");

        current = current->next;
    }

    fclose(file);
}

void saveArrayToCSV(const char *filename, User *userArr ,int size)
{

    FILE *file = fopen(filename, "w");
    int i = 0;
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char headerFile[100];
    char dataType[100];

    /* qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByAccountID); */

    concatenateWithCommas(FIELD_NAME, FIELD_NAME_SIZE, headerFile);
    concatenateWithCommas(FIELD_TYPE, FIELD_TYPE_SIZE, dataType);

    fprintf(file, strcat(headerFile, "\n"));

    while (i < size)
    {

        fprintf(file, dataType,
                userArr[i].id,
                userArr[i].accountID,
                userArr[i].fname,
                userArr[i].lname,
                userArr[i].tel,
                userArr[i].balance,
                userArr[i].active,
                userArr[i].status,
                userArr[i].registerTime,
                userArr[i].password
        );
        

        i++;
        fprintf(file, "\n");

    }

    fclose(file);
}

void saveUser(const char *filename, const User *userData)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    char headerFile[100];
    char dataType[100];

    concatenateWithCommas(FIELD_NAME, FIELD_NAME_SIZE, headerFile);
    concatenateWithCommas(FIELD_TYPE, FIELD_TYPE_SIZE, dataType);

    // Write the header row
    fprintf(file, strcat(headerFile, "\n"));

    // Write User data
    fprintf(file, dataType,
            userData->id,
            userData->accountID,
            userData->fname,
            userData->lname,
            userData->tel,
            userData->balance,
            userData->active,
            userData->status,
            userData->registerTime,
            userData->password);

    // Close the file
    fclose(file);
}

void appendToCSV(const char *filename, const User *userData)
{
    FILE *file = fopen(filename, "a"); // Open in append mode
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    char headerFile[100];
    char dataType[100];

    concatenateWithCommas(FIELD_NAME, FIELD_NAME_SIZE, headerFile);
    concatenateWithCommas(FIELD_TYPE, FIELD_TYPE_SIZE, dataType);

    // Write the header row
    /*     fprintf(file, strcat(headerFile, "\n")); */

    // Write User data
    fprintf(file, dataType,
            userData->id,
            userData->accountID,
            userData->fname,
            userData->lname,
            userData->tel,
            userData->balance,
            userData->active,
            userData->status,
            userData->registerTime,
            userData->password);
    fprintf(file, "\n");

    // Close the file
    fclose(file);
}

void copyTo(char *dest, const char *src, size_t destSize)
{
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
}

void concatenateWithCommas(const char *fieldNames[], int fieldSize, char *result)
{
    // Initialize the result string
    strcpy(result, "");

    for (int i = 0; i < fieldSize; i++)
    {
        strcat(result, fieldNames[i]);
        if (i < fieldSize - 1)
        {
            strcat(result, ",");
        }
    }
}

int isFileChanged(const char *filename, time_t *lastModifiedTime)
{
    struct stat fileStat;

    // Get the file's status (including modification time)
    if (stat(filename, &fileStat) != 0)
    {
        perror("Error getting file status");
        return -1; // Error
    }

    // Check if the modification time has changed
    if (fileStat.st_mtime != *lastModifiedTime)
    {
        *lastModifiedTime = fileStat.st_mtime; // Update last modified time
        return 1;                              // File has changed
    }

    return 0; // File has not changed
}

int checkFileChange(const char *filename, float interval)
{
    time_t lastModifiedTime = 0;
    int i = 0;
    while (1)
    {
        int result = isFileChanged(filename, &lastModifiedTime);

        if (result == -1)
        {
            return 1; // Error occurred
        }
        else if (result == 1 && i != 0)
        {
            printf("File has changed. Last modified time: %s", ctime(&lastModifiedTime));
            printf("Restarting...");
            /* system("restart.exe"); */
            exit(1);
            return 1;
        }
        else
        {
            printf("File has not changed.\n");
        }

        // Sleep for a while before checking again (adjust as needed)
        sleep(interval);
        i = 1;
    }
}

int checkFileChangeOnce(const char *filename)
{

    time_t lastModifiedTime = 0;
    int result = isFileChanged(filename, &lastModifiedTime);

    // Print the current time in a human-readable format
    printf("Program started at: %s", ctime(&lastestTime));
    printf("File has changed. Last modified time: %s", ctime(&lastModifiedTime));
    if (lastModifiedTime >= lastestTime)
    {
        printf("Loading New Data");
        struct tm *localTime;
        time(&lastestTime);
        localTime = localtime(&lastestTime);

        USER_LIST = NULL;
        Table userData = processCSVToLinkedList(USERS_DATA, 1);
        USER_LIST = userData.list;
    }
    else
    {
        printf("Not Change");
    }
    return 1;
}

int isfileExists(const char *filename)
{
    return access(filename, F_OK) != -1;
}

int getListSize(UserNode *list)
{
    UserNode *current = list;
    int count = 0;
    if (current == NULL)
    {
        return 0;
    }
    else
    {
        while (current)
        {
            count++;
            current = current->next;
        }
        return count;
    }
}


char *getCurrentTime()
{
    time_t current_time;
    struct tm *time_info;
    char *time_string = (char *)malloc(50 * sizeof(char)); // Allocate memory for the string

    if (time_string == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
        // Exit the program if memory allocation fails
    }
    // Get the current time
    time(&current_time);

    // Convert the current time to a struct tm
    time_info = localtime(&current_time);

    // Format the time as a string
    strftime(time_string, 50, "%Y-%m-%d %H:%M:%S", time_info);
    return time_string; // Return the dynamically allocated string
}


Table processCSVToLinkedList(const char *filename, int choice)
{
    FILE *file = fopen(filename, "r");
    Table csvDataTable;
    User userData, *userArray;
    UserNode *current = NULL;
    UserNode *userHead = NULL;

    if (!file)
    {
        perror("Error opening file");
        return csvDataTable;
    }

    char line[MAX_LINE_SIZE];
    char *fieldNames[MAX_FIELDS]; // Array to store field names
    int fieldCount = 1;
    int currentRow = 1; // Initialize the row number

    // Read the first line to get field names
    if (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        fieldNames[0] = strdup("#");
        while (token && fieldCount < MAX_FIELDS)
        {
            // Store the field name in the array
            fieldNames[fieldCount] = strdup(token);

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

    // Process the data lines
    while (fgets(line, sizeof(line), file) && currentRow <= MAX_ROW)
    {
        char *token = strtok(line, ",");
        int currentField = 0;

        while (token && currentField < fieldCount)
        {

            if (strcmp(fieldNames[0], "#") == 0)
                userData._id = currentRow;

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[0]) == 0)
                copyTo(userData.id, token, sizeof(userData.id));

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[1]) == 0)
                copyTo(userData.accountID, token, sizeof(userData.accountID));

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[2]) == 0)
                copyTo(userData.fname, token, sizeof(userData.fname));

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[3]) == 0)
                copyTo(userData.lname, token, sizeof(userData.lname));

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[4]) == 0)
                copyTo(userData.tel, token, sizeof(userData.tel));

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[5]) == 0)
                userData.balance = strtod(token, NULL);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[6]) == 0)
                userData.active = atoi(token);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[7]) == 0)
                userData.status = atoi(token);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[8]) == 0)
                copyTo(userData.registerTime, token, sizeof(userData.registerTime));

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[9]) == 0)
                copyTo(userData.password, token, sizeof(userData.password));

            // Remove trailing newline character from the token, if present
            size_t len = strlen(token);

            if (token[len - 1] == '\n')
            {
                token[len - 1] = '\0';
            }

            // Get the next token
            token = strtok(NULL, ",");
            currentField++;
        }

        /* printf("\n"); */

        UserNode *newUserNode = NULL;
        newUserNode = malloc(sizeof(struct UserNode));
        newUserNode->data = userData;
        newUserNode->next = NULL;

        current = userHead;

        if (userHead == NULL)
        {
            userHead = newUserNode;
        }
        else
        {
            while (current)
            {
                if (!current->next)
                {
                    current->next = newUserNode;

                    break;
                }
                current = current->next;
            }
        }

        currentRow++; // Increment the row number
    }

    // Free memory for field names
    for (int i = 0; i < fieldCount; i++)
    {
        free(fieldNames[i]);
    }

    fclose(file);

    int arraySize;
    userArray = linkedListToArray(userHead, currentRow - 1, &arraySize);


    csvDataTable.numRows = currentRow - 1;
    csvDataTable.numCols = fieldCount;

    csvDataTable.list = userHead;
    csvDataTable.array = userArray;
    csvDataTable.size = arraySize;

    USER_LIST = userHead;
    USER_ARR = userArray;
    USER_ARR_SIZE = arraySize;

    qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByAccountID);

    printf("\n Process User Table [/]\n");

    return csvDataTable;
}

User *linkedListToArray(UserNode *head, int linkSize, int *arraySize)
{
    // First, count the number of nodes
    int count = linkSize;
    UserNode *current = head;

    // Allocate memory for the array of User structures
    User *userArray = (User *)malloc(count * sizeof(User));
    if (userArray == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    // Copy the data from nodes into the array
    current = head;
    for (int i = 0; i < count; i++)
    {
        userArray[i] = current->data;
        current = current->next;
    }

    *arraySize = count;
    return userArray;
}






