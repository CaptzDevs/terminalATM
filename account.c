#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <conio.h> // Console I/O (non-standard)

#include "config.c"
#include "array.c"

#define MAX_LINE_SIZE 1024
#define MAX_FIELDS 10
#define MAX_ROW 1000

const char columnNameArr[10][20] = {
    "ID",
    "First Name",
    "Last Name",
    "Age",
    "Register Time"};

int space[] = {0, 15, 15, 15, 15, 15};

typedef struct User
{
    int _id;
    char id[14];
    char fname[250];
    char lname[250];
    char registerTime[50];
    int age;

} User;

typedef struct UserNode
{
    User data;
    struct UserNode *next;

} UserNode;

typedef struct Table
{
    int numRows;
    int numCols;
    UserNode *list;
    
} Table;

typedef struct List
{
    int numRows;
    int numCols;
    
} List;


UserNode *userHead = NULL;

char m_id[14] = "1909300007905";
char m_fname[100] = "SIWAKORN";
char m_lname[100] = "JANSANGSRI";
int m_age = 21;

typedef Table (*selectedTable)();
typedef List (*selectedList)();


/* int selectRow(int min, int max, char fileName[], selectedTable tableCallBack);  old*/

int selectListRow(int min, int max, UserNode *list , selectedList tableCallBack)
{
    char ch;
    int i = 0;
    int num = 1;
    tableCallBack(list, num);

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            /* printf("%d",ch); */
            if (ch <= 57 && ch >= 48)
            {
                num = ch - 48;
                system("cls");

                tableCallBack(list, num);
            }
            else
            {

                switch (ch)
                {
                case 72: // Up arrow key
                    if (num > min)
                    {
                        num -= 1;
                    }
                    else
                    {
                        num = max;
                    }
                    system("cls");
                    tableCallBack(list, num);

                    break;
                case 80: // Down arrow key

                    if (num < max)
                    {
                        num += 1;
                    }
                    else
                    {
                        num = min;
                    }
                    system("cls");
                    tableCallBack(list, num);
                    break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == 13)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return num;
}

char *getCurrentTime()
{
    time_t current_time;
    struct tm *time_info;
    char *time_string = (char *)malloc(50 * sizeof(char)); // Allocate memory for the string

    if (time_string == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); // Exit the program if memory allocation fails
    }

    // Get the current time
    time(&current_time);

    // Convert the current time to a struct tm
    time_info = localtime(&current_time);

    // Format the time as a string
    strftime(time_string, 50, "%Y-%m-%d %H:%M:%S", time_info);
    return time_string; // Return the dynamically allocated string
}

User Register(const char id[], const char fname[], const char lname[], int age)
{
    User newUser;

    strncpy(newUser.id, id, sizeof(newUser.id) - 1);
    newUser.id[sizeof(newUser.id) - 1] = '\0';

    strncpy(newUser.fname, fname, sizeof(newUser.fname) - 1);
    newUser.fname[sizeof(newUser.fname) - 1] = '\0';

    strncpy(newUser.lname, lname, sizeof(newUser.lname) - 1);
    newUser.lname[sizeof(newUser.lname) - 1] = '\0';

    newUser.age = age;

    strncpy(newUser.registerTime, getCurrentTime(), sizeof(newUser.registerTime) - 1);
    newUser.registerTime[sizeof(newUser.registerTime) - 1] = '\0';

    return newUser;
}

int fileExists(const char *filename)
{
    return access(filename, F_OK) != -1;
}

void saveUser(const char *filename, const User *userData)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int numColumns = 5;
    char combinedString[200] = ""; // Initialize an empty string to store the result

    for (int i = 0; i < numColumns; i++)
    {
        strcat(combinedString, columnNameArr[i]);

        // Append a comma and space if it's not the last column
        if (i < numColumns - 1)
        {
            strcat(combinedString, ",");
        }
    }
    // Write the header row
    /* fprintf(file, "ID,First Name,Last Name,Age\n"); */
    fprintf(file, strcat(combinedString, "\n"));

    // Write User data
    fprintf(file, "%s,%s,%s,%d,%s\n", userData->id, userData->fname, userData->lname, userData->age, userData->registerTime);

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
    // Write User data

    fprintf(file, "%s,%s,%s,%d,%s\n", userData->id, userData->fname, userData->lname, userData->age, userData->registerTime);

    // Close the file
    fclose(file);
}

void processValueCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), file))
    {
        // Tokenize the line using strtok
        char *token = strtok(line, ",");
        while (token)
        {
            // Process the token (field) here
            printf("Field: %s\n", token);

            // Get the next token
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
}

List showLinkedList(UserNode *head, int choice){

    printf("=================== Linked List ===================\n");

    UserNode *current = NULL , *detail = NULL;
    List userList;
       if (head == NULL)
    { // show list
        printf(" List is Empty");
        /* printf("\npress any key to continue...\n"); */
    }
    else
    {
        current = head;

        while (current != NULL)
        {
            if (choice == current->data._id){

                printf("\033[1;32m\033[4m");
                detail = current;
            }

            printf(" %-5d|", current->data._id);

       
            
            printf(" %-15s|", current->data.id);
            printf(" %-15s|", current->data.fname);
            printf(" %-15s|", current->data.lname);
            printf(" %-2d |\n", current->data.age);

            if (choice == current->data._id)
                printf("\033[0m");

            current = current->next;
        }

        printf("\n============== Detail ==============\n");

        printf(" #              : %d \n", detail->data._id);
        printf(" ID             : %s \n", detail->data.id);
        printf(" fname          : %s \n", detail->data.fname);
        printf(" lname          : %s \n", detail->data.lname);
        printf(" age            : %d \n", detail->data.age);
        printf(" registerDate   : %s \n", detail->data.registerTime);


        return userList;
    }
}

void processCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_SIZE];
    char *fieldNames[MAX_FIELDS]; // Array to store field names
    int fieldCount = 0;

    // Read the first line to get field names
    if (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
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
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int fieldIndex = 0;
        while (token && fieldIndex < fieldCount)
        {
            // Remove trailing newline character from the token, if present
            size_t len = strlen(token);
            if (token[len - 1] == '\n')
            {
                token[len - 1] = '\0';
            }

            // Access the field name and value based on fieldIndex
            printf("%s: %s\n", fieldNames[fieldIndex], token);

            // Get the next token
            token = strtok(NULL, ",");
            fieldIndex++;
        }
        printf("\n");
    }

    // Free memory for field names
    for (int i = 0; i < fieldCount; i++)
    {
        free(fieldNames[i]);
    }

    fclose(file);
}

void copyTo(char *dest, const char *src, size_t destSize)
{
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
}

Table processTableCSV(const char *filename, int choice)
{
    FILE *file = fopen(filename, "r");
    Table csvDataTable;
    User userData;

    UserNode *current = NULL;

    printf("\n=================== User Table ===================\n");

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

    // Print field names as the first row of the table
    for (int i = 0; i < fieldCount; i++)
    {
        printf("%-*s\t", space[i], fieldNames[i]);
    }

    printf("\n");

    // Process the data lines
    while (fgets(line, sizeof(line), file) && currentRow <= MAX_ROW)
    {
        char *token = strtok(line, ",");
        int currentField = 0;

        if (choice == currentRow)
            printf("\033[1;32m\033[4m");

        printf("%d\t", currentRow); // Print the row number

        if (choice == currentRow)
            printf("\033[0m");

        while (token && currentField < fieldCount)
        {

            if (strcmp(fieldNames[0],"#") == 0)
                userData._id = currentRow;

            if (strcmp(fieldNames[currentField + 1], columnNameArr[0]) == 0)
                copyTo(userData.id, token, sizeof(userData.id));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[1]) == 0)
                copyTo(userData.fname, token, sizeof(userData.fname));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[2]) == 0)
                copyTo(userData.lname, token, sizeof(userData.lname));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[3]) == 0)
                userData.age = atoi(token);

            if (strcmp(fieldNames[currentField + 1], columnNameArr[4]) == 0)
                copyTo(userData.registerTime, token, sizeof(userData.registerTime));

            // Remove trailing newline character from the token, if present
            size_t len = strlen(token);

            if (token[len - 1] == '\n')
            {
                token[len - 1] = '\0';
            }

            // Print data value with tab separator
            if (choice == currentRow)
                printf("\033[1;32m");

            printf("%-*s\t", space[currentField], token);

            if (choice == currentRow)
                printf("\033[0m");

            // Get the next token
            token = strtok(NULL, ",");
            currentField++;
        }

        printf("\n");

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

        /* printf("%s\n", newUserNode->data.id); */

        currentRow++; // Increment the row number
    }

    // Free memory for field names
    for (int i = 0; i < fieldCount; i++)
    {
        free(fieldNames[i]);
    }

    fclose(file);

    printf("\n============== Table Detail ==============\n");

    csvDataTable.numRows = currentRow - 1;
    csvDataTable.numCols = fieldCount;
    csvDataTable.list = userHead;

    printf("Cols            : %d \n", fieldCount);
    printf("Rows            : %d \n", currentRow - 1);
    printf("Selected At Row : %d \n", choice);

    printf("\n");

    printf("\n============== Last insert row ==============\n");


    printf(" #              : %d \n", currentRow-1);
    printf(" ID             : %s \n", userData.id);
    printf(" fname          : %s \n", userData.fname);
    printf(" lname          : %s \n", userData.lname);
    printf(" age            : %d \n", userData.age);
    printf(" registerDate   : %s \n", userData.registerTime);


    

    return csvDataTable;
}

void generateRandomUserData(User *u)
{
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Generate a random ID
    for (int i = 0; i < 13; i++)
    {
        u->id[i] = '0' + rand() % 10;
    }
    u->id[13] = '\0'; // Null-terminate the string

    // Generate random first name and last name
    const char *firstNames[] = {"John", "Jane", "Michael", "Emma", "William"};
    const char *lastNames[] = {"Smith", "Johnson", "Brown", "Davis", "Wilson"};

    strcpy(u->fname, firstNames[rand() % 5]);
    strcpy(u->lname, lastNames[rand() % 5]);
    strcpy(u->registerTime, getCurrentTime());
    // Generate a random age between 18 and 60
    u->age = 18 + rand() % 43;
}

int main(int argc, char const *argv[])
{

    /*  User registeredUser = Register(m_id, m_fname, m_lname, m_age); */
     printf("\033[?25l");
    User registeredUser;
    UserNode *UserList;

    generateRandomUserData(&registeredUser);

    printf("================ New User ============\n");
    printf("ID                  : %s \n", registeredUser.id);
    printf("User                : %s %s\n", registeredUser.fname, registeredUser.lname);
    printf("Age                 : %d \n", registeredUser.age);
    printf("Register Time       : %s \n", registeredUser.registerTime);

    printf("======================================\n");

    if (fileExists("Users"))
    {
        printf(_ADMIN_DEBUG ? "file is exist : APPENDED\n" : "");
        appendToCSV("Users", &registeredUser);
    }
    else
    {
        printf(_ADMIN_DEBUG ? "file is not exist : CREATED\n" : "");
        saveUser("Users", &registeredUser);
    }

    printf("========================\n");
    printf("Table USERS \n");
    printf("========================\n");

    Table userData = processTableCSV("Users", 1);
    UserList = userData.list;
    
    /* showLinkedList(UserList); */
    /* selectRow(1, userData.numRows, "Users", processTableCSV); */

    selectListRow(1, userData.numRows, UserList, showLinkedList);

    return 0;
}