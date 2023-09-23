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

#include "config.c"
#include "module/array.c"

#define MAX_LINE_SIZE 1024
#define MAX_FIELDS 10
#define MAX_ROW 30000

// Key
#define EXIST_KEY 27
#define ENTER_KEY 13
#define DELETE_KEY 83

// Arrow Key Code
#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75
#define RIGHT_KEY 77

#define MAX_LIST_ROW 10

#define MAX_ACCCOUNT_ID_SIZE 11

// Money Color \033[38;5;48m

#define USERS_DATA "./Data/Users.csv"
#define ACCOUNTS_DATA "./Data/Accounts.csv"

char *USER_MENU[] = {
    "EDIT",
    "DELETE",
    "ACTIVE && SUSPEND CARD",
    "ENABLE && DISABLE CARD"};

const char *FIELD_NAME[] = {
    "ID",
    "AccountID",
    "First Name",
    "Last Name",
    "Age",
    "Balance",
    "Active",
    "Status",
    "Register Time"};

const char *FIELD_TYPE[] = {
    "%s",
    "%s",
    "%s",
    "%s",
    "%d",
    "%.2lf",
    "%d",
    "%d",
    "%s"};

const int FIELD_NAME_SIZE = sizeof(FIELD_NAME) / sizeof(FIELD_NAME[0]);
const int FIELD_TYPE_SIZE = sizeof(FIELD_TYPE) / sizeof(FIELD_TYPE[0]);
const int USER_MENU_SIZE = sizeof(USER_MENU) / sizeof(USER_MENU[0]);

int space[] = {0, 15, 15, 15, 15, 15};

typedef struct User
{
    int _id; // for index
    char id[15];
    char accountID[15];
    char fname[250];
    char lname[250];
    int age;
    double balance;
    int active;
    int status;
    char registerTime[50];

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
    int arraySize;
    UserNode *list;
    User *array;

} Table;

typedef struct List
{
    int numRows;
    int numCols;

    int currentRow;
    int currentID;
    struct List *list;

    UserNode *userData;
    User *array;

} List;

typedef struct SearchData
{
    int result;
    User *user;
} SearchData;

UserNode *USER_LIST = NULL;
User *USER_ARR = NULL;

int USER_ARR_SIZE = 0;

typedef Table (*selectedTable)();
typedef List (*selectedList)();
typedef List (*selectedArray)();
typedef void (*selectedUserMenu)();

/* ============================ */
void saveUser(const char *filename, const User *userData);
void appendToCSV(const char *filename, const User *userData);

void concatenateWithCommas(const char *fieldNames[], int fieldSize, char *result);
void saveLinkedListToCSV(const char *filename, UserNode *head);
int getListSize(UserNode *list);
int checkFileChange(const char *filename, float interval);
int checkFileChangeOnce(const char *filename);

int isfileExists(const char *filename);
int getStringInput(char *input, int maxSize);
int selectUserArray(int min, int max, User *list, selectedArray tableCallBack);
List displayUserArray(User *userArray, int arraySize, int choice, int page);
User *linkedListToArray(UserNode *head, int linkSize, int *arraySize);
UserNode *deleteUser(int id);
Table processCSVToLinkedList(const char *filename, int choice);

time_t lastestTime;

/* ============================ */

/* 2 Type of menu function
    1. Controller Function      [C]
    2. Display Function         [D]
*/

UserNode *editUserData(UserNode *userDetail)
{

    
    printf("ID           %-5s %s \n", ":", userDetail->data.id);
    printf("FirstName    %-5s %s \n", ":", userDetail->data.fname);
    printf("Lastname     %-5s %s \n", ":", userDetail->data.lname);
    printf("Age          %-5s %d\n", ":", userDetail->data.age);
    printf("\n");
    printf("\n\033[4m\033[38;5;50mLeave it blank for not change\033[0m\n\n");

    char nFname[250];
    char nLname[250];
    char nAge[5];

    int age = userDetail->data.age;

    strcpy(nFname, userDetail->data.fname);
    strcpy(nLname, userDetail->data.lname);
    sprintf(nAge, "%d", userDetail->data.age);

    fflush(stdin);

    printf("New First Name : ");
    printf("\033[38;5;75m");
    int lFname = getStringInput(nFname, sizeof(nFname));
    printf("\033[0m");
    printf("New Last Name : ");
    printf("\033[38;5;75m");
    int lLname = getStringInput(nLname, sizeof(nLname));
    printf("\033[0m");

    printf("New Age : ");
    printf("\033[38;5;75m");
    int lAge = getStringInput(nAge, sizeof(nAge));
    printf("\033[0m");

    /*  printf("%d %d %d",lFname,lLname,lAge); */

    if (lFname == 1)
        strcpy(nFname, userDetail->data.fname);
    if (lLname == 1)
        strcpy(nLname, userDetail->data.lname);
    if (lAge == 1)
        sprintf(nAge, "%d", userDetail->data.age);

    printf("\n");
    printf("Check new data \n");
    printf("=====================================\n");
    printf("%-15s -> %s \n", userDetail->data.fname, nFname);
    printf("%-15s -> %s \n", userDetail->data.lname, nLname);
    printf("%-15d -> %s \n", userDetail->data.age, nAge);
    printf("=====================================\n");

    printf("Commit the change ? \n");
    printf("\033[1;32m > Enter  |   Save \n");
    printf("\033[1;31m > ESC    |   Discard \n");
    printf("\033[0m");

    strcpy(userDetail->data.fname, nFname);
    strcpy(userDetail->data.lname, nLname);
    userDetail->data.age = atoi(nAge);

    return userDetail;
}

int getStringInput(char *input, int maxSize)
{
    fgets(input, maxSize, stdin);

    // Remove the newline character from the end of the string
    int length = strlen(input);
    if (length > 0 && input[length - 1] == '\n')
    {
        input[length - 1] = '\0';
    }

    return length;
}

/* [D] */
void displayUserMenu(int choice, char *arr[], char header[], UserNode *userDetail)
{

    if (_RFMENU)
        system("cls");

    printf("\n============== User Detail ==============\n");

    printf(" #              : %d \n", userDetail->data._id);
    printf(" ID             : %s \n", userDetail->data.id);
    printf(" fname          : %s \n", userDetail->data.fname);
    printf(" lname          : %s \n", userDetail->data.lname);
    printf(" age            : %d \n", userDetail->data.age);
    printf(" accountID      : %s \n", userDetail->data.accountID);
    printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", userDetail->data.balance);
    printf("\n================= Card Status =============\n");
    printf(" active         : %s \n", userDetail->data.active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
    printf(" status         : %s \n", userDetail->data.status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
    printf(" registerDate   : %s \n", userDetail->data.registerTime);
    printf("=====================================\n");

    /*     char(*optionPtr)[50];

        optionPtr = &arr[0]; */
    int optionIndex = 1;

    printf("%s :\n", header);

    int i = 0;
    while (i < USER_MENU_SIZE)
    {
        if (choice == optionIndex)
            printf("\033[1;32m \033[4m");

        if (choice == 3 && optionIndex == 3)
            if (userDetail->data.active == 1)
                printf("\033[1;31m");
        if (choice == 4 && optionIndex == 4)
            if (userDetail->data.status == 1)
                printf("\033[1;31m");
        ;

        printf("> [%d] : %s\n", optionIndex, arr[i]);

        if (choice == optionIndex)
            printf("\033[0m");
        i++;
        optionIndex++;
    }

    if (choice == 0)
    {
        printf("\033[1;32m> \033[4m");
    }
    printf("> [0] : Quit\n");
    if (choice == 0)
    {
        printf("\033[0m");
    }

    printf("\n \033[38;5;50m( Use Arrow and Enter for Select ) \033[0m \n");
}

/* [C] */
int selectUserMenu(int min, char *arr[], selectedUserMenu displayMenuCallback, char header[], UserNode *userDetail)
{
    char ch;
    int i = 0;
    int num = 1;
    int max = USER_MENU_SIZE;

    displayMenuCallback(num, arr, header, userDetail);

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
                displayMenuCallback(num, arr, header, userDetail);
            }
            else
            {

                switch (ch)
                {
                case UP_KEY: // Up arrow key
                    if (num > min)
                    {
                        num -= 1;
                    }
                    else
                    {
                        num = max;
                    }

                    displayMenuCallback(num, arr, header, userDetail);

                    break;
                case DOWN_KEY: // Down arrow key

                    if (num < max)
                    {
                        num += 1;
                    }
                    else
                    {
                        num = min;
                    }

                    displayMenuCallback(num, arr, header, userDetail);
                    break;

                case ENTER_KEY: // Down arrow key
                    if (_ADMIN_DEBUG)
                        printf("SELECT IN USER MENU\n");

                    switch (num)
                    {
                        // Edit
                    case 1:
                        printf("===============================\n");
                        printf("\t   %s\n", USER_MENU[num - 1]);
                        printf("===============================\n");

                        UserNode temp;
                        strcpy(temp.data.fname, userDetail->data.fname);
                        strcpy(temp.data.lname, userDetail->data.lname);
                        temp.data.age = userDetail->data.age;


                        editUserData(userDetail);
                        
                        char ch2;
                        ch2 = getch();
                        printf("> %d",ch2);
                        getch();
                        switch (ch2)
                        {
                        case ENTER_KEY:

                            saveLinkedListToCSV(USERS_DATA, USER_LIST);

                            printf("=====================================\n");
                            printf("\033[1;32m> Update Data Success \n\033[0m");
                            printf("=====================================\n");

                            getch();
                            displayMenuCallback(num, arr, header, userDetail);

                            break;

                        case EXIST_KEY:

                                strcpy(userDetail->data.fname, temp.data.fname);
                                strcpy(userDetail->data.lname, temp.data.lname);
                                userDetail->data.age = temp.data.age;

                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);

                            break;

                        default:
                            break;
                        }

                        break;

                    case 2: // Delete
                        printf("=====================\n");
                        printf("%s\n", USER_MENU[num - 1]);
                        printf("=====================\n");

                        printf("\033[0;31m Do you want to delete this data ? \033[0m\n");
                        char chDelete;
                        chDelete = getch();
                        switch (chDelete)
                        {
                        case ENTER_KEY:
                        {
                            USER_LIST = deleteUser(userDetail->data._id);
                            saveLinkedListToCSV(USERS_DATA, USER_LIST);
                            int listSize = getListSize(USER_LIST);
                            int arrSize;
                            USER_ARR = linkedListToArray(USER_LIST, listSize, &arrSize);
                            free(userDetail);
                            num = 0;
                            break;
                        }
                        case EXIST_KEY:
                        {
                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);
                        }
                        default:
                            break;
                        }

                        break;

                    case 3: // Suspend card
                        printf("=====================\n");
                        printf("%s\n", userDetail->data.active == 1 ? "SUSPEND CARD" : "ACTIVE CARD");
                        printf("=====================\n");

                        printf(userDetail->data.active == 1 ? "\033[0;31mDo you want to Suspend this card" : "\033[0;32mDo you want to Active this card");

                        printf(" ?\033[0m \n");

                        char chActive;
                        chActive = getch();

                        switch (chActive)
                        {
                        case ENTER_KEY:
                        {
                            if (userDetail->data.active == 1)
                            {
                                userDetail->data.active = 0;
                                saveLinkedListToCSV(USERS_DATA, USER_LIST);
                                printf("\033[0;31m> Suspended card [/]\033[0m ");
                                USER_MENU[2] = "Active Card";
                            }
                            else if (userDetail->data.active == 0)
                            {
                                userDetail->data.active = 1;
                                saveLinkedListToCSV(USERS_DATA, USER_LIST);
                                printf("\033[0;32m> Active card [/]\033[0m ");
                                USER_MENU[2] = "Suspended Card";
                            }
                            displayMenuCallback(num, arr, header, userDetail);

                            break;
                        }
                        case EXIST_KEY:
                        {
                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);
                        }
                        default:
                            break;
                        }

                        break;
                    case 4: // Disable card
                        printf("=====================\n");
                        printf("%s\n", userDetail->data.status == 1 ? "DISABLE CARD" : "ENABLE CARD");
                        printf("=====================\n");
                        printf(userDetail->data.status == 1 ? "\033[0;31mDo you want to Disable this card" : "\033[0;32mDo you want to Enable this card");
                        printf("?\033[0m \n");

                        char chStatus;
                        chStatus = getch();

                        switch (chStatus)
                        {
                        case ENTER_KEY:
                        {
                            if (userDetail->data.status == 1)
                            {
                                userDetail->data.status = 0;
                                saveLinkedListToCSV(USERS_DATA, USER_LIST);
                                printf("\033[0;31m> Disable card [/]\033[0m ");
                                USER_MENU[3] = "Enable Card";
                            }
                            else if (userDetail->data.status == 0)
                            {
                                userDetail->data.status = 1;
                                saveLinkedListToCSV(USERS_DATA, USER_LIST);
                                printf("\033[0;32m> Enable card [/]\033[0m ");
                                USER_MENU[3] = "Disable Card";
                            }

                            displayMenuCallback(num, arr, header, userDetail);

                            break;
                        }
                        case EXIST_KEY:
                        {
                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);
                        }
                        default:
                            break;
                        }
                        break;
                    case 0:
                        return 0;
                        break;
                    default:
                        break;
                    }

                    break;
                case EXIST_KEY: // exit User detail menu
                    return 0;
                    break;
                default:
                    printf("\n");
                    break;
                }
            }
            // Enter Key
        }

        /* printf("Select : %d \n",num); */
    }

    return num;
};

// [D]
List displayUserArray(User *userArray, int arraySize, int choice, int page)
{
    if (_CREAR_LIST)
        system("cls");

    printf("\n=============== User Array =================\n");

    float rowPerPage = ceil((float)arraySize / MAX_LIST_ROW);

    int start = MAX_LIST_ROW * (page - 1) + 1;
    int stop = page * MAX_LIST_ROW;

    printf("page %d\n", page);

    User *detail = NULL;
    List userList;

    if (start == 0)
    {
        start = 1;
        stop = MAX_LIST_ROW;
    }

    int i = start - 1; // run number
    int line = 1;
    int c = 1; // run line that can show per page
    int countRows = 0;

    printf("choice  > %d \n", choice);
    printf("start   > %d \n", start);
    printf("stop    > %d \n", stop);

    if (_SHOW_LIST_LINE)
        printf("|%-5s", "Line");
    printf("|%-5s", "_ID");
    printf("|%-15s", "ID");
    printf("|%-20s", "Fname");
    printf("|%-20s", "Lname");
    printf("|%-4s", "Age\n");

    printf("\n");

    while (i < stop && i < arraySize)
    {

        if (choice == line)
        {

            printf("\033[1;32m\033[4m");
            detail = &userArray[i];
        }

        /*  printf("c %-5d|", c); */
        if (_SHOW_LIST_LINE)
            printf("|%-5d", line);
        printf("|%-5d", userArray[i]._id);
        printf("|%-15s", userArray[i].id);
        printf("|%-20s", userArray[i].fname);
        printf("|%-20s", userArray[i].lname);
        printf("|%-4d\n", userArray[i].age);

        countRows++;

        if (choice == line)
            printf("\033[0m");

        i++;
        line++;
        c++;
    }

    /* printf("c : %d\n",c); */
    /*   printf("========================\n");
      printf("Show %d row(s) (%d-%d)  from %d/%.0f row(s) \n",countRows,start,c-1,i-1 , listSize);
*/
    int max = i - 1;

    printf("========================\n");
    int p = 1;
    /*  printf("%.2f \n",rowPerPage); */

    /*  printf("%d\n",start);
     printf("%d\n",stop);
*/

    int l = 1;
    int pageListPerRow = 10;
    printf(">> %.2f \n", rowPerPage);

    /*    for (p = 1; p <= rowPerPage; p++)
      {
          if(page == p) printf("\033[1;32m");
          printf("|%-3d ",p);
          if(page == p) printf("\033[0m");
      } */

    if (detail)
    {
        printf("\n============== User Detail ==============\n");

        printf(" #              : %d \n", detail->_id);
        printf(" ID             : %s \n", detail->id);
        printf(" fname          : %s \n", detail->fname);
        printf(" lname          : %s \n", detail->lname);
        printf(" age            : %d \n", detail->age);
        printf(" accountID      : %s \n", detail->accountID);
        printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", detail->balance);
        printf("\n================= Card Status =============\n");
        printf(" active         : %s \n", detail->active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
        printf(" status         : %s \n", detail->status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
        printf(" registerDate   : %s \n", detail->registerTime);
        printf("=====================================\n");

        /* printf("%d , %d",i,choice);
         */
    }

    userList.array = userArray;
    return userList;
}

// [C]
int selectUserArray(int min, int max, User *list, selectedArray tableCallBack)
{

    char ch;
    int i = 0;
    int row = 1;
    int page = 1;
    int currnentID;
    int flag;
    float numRows = 0;

    tableCallBack(list, max, row, page);

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            printf("%d \n", ch);

            if (ch <= 57 && ch >= 48)
            {
                row = ch - 48;
            }
            else
            {
                switch (ch)
                {
                case UP_KEY: // Up arrow key

                    flag = max % MAX_LIST_ROW;

                    row -= 1;
                    if (row == 0)
                    {
                        row = flag;
                    }
                    if ((float)page > (float)max / MAX_LIST_ROW && row > flag)
                    {
                        row = flag;
                    }

                    tableCallBack(list, max, row, page);
                    break;

                case LEFT_KEY: // Up left key
                {

                    page -= 1;

                    if (page <= 0)
                    {
                        page = 1;
                    }
                    tableCallBack(list, max, row, page);
                }
                break;
                case RIGHT_KEY: // Up right key
                {
                    page += 1;

                    tableCallBack(list, max, row, page);
                }
                break;
                case DOWN_KEY: // Down arrow key

                    if (row < MAX_LIST_ROW)
                    {
                        row += 1;
                    }
                    else
                    {
                        row = 1;
                    }
                    flag = max % MAX_LIST_ROW;

                    if ((float)page > (float)max / MAX_LIST_ROW && row > flag)
                    {
                        row = 1;
                    }
                    tableCallBack(list, max, row, page);

                    break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == ENTER_KEY)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return row;
}

// [D]
List displayUserList(int choice, int page)
{
    if (_CREAR_LIST)
        system("cls");

    float listSize = getListSize(USER_LIST);
    float rowPerPage = ceil(listSize / MAX_LIST_ROW);
    int start = MAX_LIST_ROW * (page - 1) + 1;
    int stop = page * MAX_LIST_ROW;

    printf("page %d\n", page);

    printf("=================== Linked List ===================\n");

    if (start == 0)
    {
        start = 1;
        stop = MAX_LIST_ROW;
    }

    int i = 1; // run number
    int c = 1; // run line that can show per page
    int countRows = 0;

    printf("choice  > %d \n", choice);
    printf("start   > %d \n", start);
    printf("stop    > %d \n", stop);

    UserNode *current = NULL, *detail = NULL;
    List userList;
    if (USER_LIST == NULL)
    { // show list
        printf(" List is Empty");
        /* printf("\npress any key to continue...\n"); */
    }
    else
    {
        current = USER_LIST;
        if (_SHOW_LIST_LINE)
        printf("|%-5s", "Line");
        printf("|%-5s", "_ID");
        printf("|%-15s", "ID");
        printf("|%-13s", "AccountID");
        printf("|%-15s", "Fname");
        printf("|%-15s", "Lname\n");
        printf("\n");

        while (current != NULL && i <= stop)
        {

            if (choice == i)
            {

                printf("\033[1;32m\033[4m");
                detail = current;
                userList.currentRow = i;
                userList.currentID = current->data._id;
            }

            if (c >= start)
            {
                /*  printf("c %-5d|", c); */
                if (_SHOW_LIST_LINE)
                    printf("|%-5d", i);
                printf("|%-5d", current->data._id);
                printf("|%-15s", current->data.id);
                printf("|%-13s", current->data.accountID);
                printf("|%-15s", current->data.fname);
                printf("|%-15s\n", current->data.lname);
                /* printf(" %-2d |\n", current->data.age); */
                countRows++;
            }

            if (choice == i)
                printf("\033[0m");

            current = current->next;

            i++;
            c++;

            if (c == stop + 1)
            {
                break;
            }
        }

        printf("========================\n");
        int maxRow;
        if (start <= c - 1)
        {
            maxRow = c - 1;
        }
        else if (start > c - 1)
        {
            maxRow = start + MAX_LIST_ROW;
        }

        printf("Show %d row(s) (%d-%d)  from %d/%.0f row(s) \n", countRows, start, maxRow, i - 1, listSize);

        int max = i - 1;

        printf("========================\n");
        int p = 1;

        int l = 1;
        int pageListPerRow = 10;

        for (p = 1; p <= rowPerPage; p)
        {
            for (l = 1; l <= pageListPerRow && p <= rowPerPage; l++)
            {
                if (page == p)
                    printf("\033[1;32m");
                printf("|%-3d ", p);
                if (page == p)
                    printf("\033[0m");
                p++;
            }
            printf("\n");
        }

        if (detail)
        {
            printf("\n============== User Detail ==============\n");

            printf(" #              : %d \n", detail->data._id);
            printf(" ID             : %s \n", detail->data.id);
            printf(" fname          : %s \n", detail->data.fname);
            printf(" lname          : %s \n", detail->data.lname);
            printf(" age            : %d \n", detail->data.age);
            printf(" accountID      : %s \n", detail->data.accountID);
            printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", detail->data.balance);
            printf("\n================= Card Status =============\n");
            printf(" active         : %s \n", detail->data.active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
            printf(" status         : %s \n", detail->data.status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
            printf(" registerDate   : %s \n", detail->data.registerTime);
            printf("=====================================\n");
        }
        else
        {
            userList.currentRow = MAX_LINE_SIZE * (page - 1) + 1;
        }

        userList.numRows = i - 1;
        userList.userData = detail;
        return userList;
    }
}

// [C]
int selectUserList(int min, int max, selectedList tableCallBack)
{
    char ch;
    int i = 0;
    int row = 1;
    int page = 1;
    int currnentID;
    float numRows = 0;
    List rowDetail;
    rowDetail = tableCallBack(row, page);

    currnentID = rowDetail.currentID;
    row = rowDetail.currentRow;

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            // printf("%d \n", ch);

            if (ch <= 57 && ch >= 48)
            {
                row = ch - 48;

                currnentID = rowDetail.currentID;
                row = rowDetail.currentRow;
            }
            else
            {
                switch (ch)
                {
                case UP_KEY: // Up arrow key

                    if (row == MAX_LIST_ROW * (page - 1) + 1)
                    {
                        row = MAX_LIST_ROW * (page);
                    }
                    else if (row > MAX_LIST_ROW * (page - 1) + 1)
                    {
                        row -= 1;
                    }
                    else
                    {
                        row = MAX_LIST_ROW * (page);
                    }

                    checkFileChangeOnce(USERS_DATA);
                    rowDetail = tableCallBack(row, page);

                    /* printf("%d", MAX_LIST_ROW * (page - 1) + 1); */

                    currnentID = rowDetail.currentID;
                    row = rowDetail.currentRow;

                    break;

                case LEFT_KEY: // Up left key
                {
                    checkFileChangeOnce(USERS_DATA);

                    float rowPerPage = ceil(numRows / MAX_LIST_ROW);
                    page -= 1;

                    if (page <= 0)
                        page = 1;

                    float flag = row % MAX_LIST_ROW;
                    if (flag == 0)
                        flag = MAX_LIST_ROW;

                    row = MAX_LIST_ROW * (page - 1) + flag;

                    rowDetail = tableCallBack(row, page);
                    numRows = rowDetail.numRows;
                }
                break;
                case RIGHT_KEY: // Up right key
                {
                    checkFileChangeOnce(USERS_DATA);

                    float flag = row % MAX_LIST_ROW;
                    page += 1;
                    if (flag == 0)
                        flag = MAX_LIST_ROW;

                    row = MAX_LIST_ROW * (page - 1) + flag;

                    rowDetail = tableCallBack(row, page);
                    numRows = rowDetail.numRows;
                }
                break;
                case DOWN_KEY: // Down arrow key

                    if (row < MAX_LIST_ROW * (page))
                    {
                        row += 1;
                    }
                    else
                    {
                        row = MAX_LIST_ROW * (page - 1) + 1;
                    }

                    checkFileChangeOnce(USERS_DATA);

                    rowDetail = tableCallBack(row, page);
                    currnentID = rowDetail.currentID;
                    row = rowDetail.currentRow;

                    break;

                case ENTER_KEY:
                {

                    USER_MENU[2] = rowDetail.userData->data.active == 0 ? "Active Card" : "Suspend Card";
                    USER_MENU[3] = rowDetail.userData->data.status == 0 ? "Enable Card" : "Disabled Card";

                    checkFileChangeOnce(USERS_DATA);
                    int exitUserMenu = selectUserMenu(0, USER_MENU, displayUserMenu, "User Action", rowDetail.userData);

                    if (exitUserMenu == 0)
                    {
                        rowDetail = tableCallBack(row, page);
                    }

                    break;
                }
                case DELETE_KEY: // Delete
                {

                    char ch2;
                    printf("\033[0;31m Do you want to delete this data ? \033[0m\n");
                    ch2 = getch();

                    if (ch2 == ENTER_KEY)
                    {
                        USER_LIST = deleteUser(currnentID);
                        saveLinkedListToCSV(USERS_DATA, USER_LIST);
                        int listSize = getListSize(USER_LIST);
                        int arrSize;

                        USER_ARR = linkedListToArray(USER_LIST, listSize, &arrSize);

                        /* selectUserArray(1,arrSize,USER_ARR,displayUserArray); */

                        if (row == max)
                        {
                            row = getListSize(USER_LIST);
                        }

                        max -= 1;

                        getch();
                        rowDetail = tableCallBack(row, page);

                        currnentID = rowDetail.currentID;
                        row = rowDetail.currentRow;
                    }
                    else
                    {
                        rowDetail = tableCallBack(row, page);

                        currnentID = rowDetail.currentID;
                        row = rowDetail.currentRow;
                    }
                }
                break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == EXIST_KEY)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");

                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return row;
}

/* Utilities Function */

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
    printf("D");
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
                user.age,
                user.balance,
                user.active,
                user.status,
                user.registerTime);

        current = current->next;
    }

    fclose(file);
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

User searchUser(User *userArray, int id)
{

    return userArray[id - 1];
};

UserNode *SearchUserList(UserNode *list, int id)
{
    UserNode *current;
    current = list;
    if (!current)
    {
        return current;
    }
    while (current)
    {
        if (current->data._id == id)
        {
            printf("user id     : %s \n", current->data.id);
            printf("user fname  : %s \n", current->data.fname);
            printf("user lname  : %s \n", current->data.lname);

            return current;
        }
        current = current->next;
    }

    return current;
};

UserNode *deleteUser(int id)
{

    UserNode *head = USER_LIST, *current = USER_LIST, *prev = NULL, *del = NULL;

    // Handle the case where the list is empty
    if (!current)
    {
        return USER_LIST;
    }

    // Check if the first node should be deleted
    if (current != NULL && current->data._id == id)
    {
        del = current;
        USER_LIST = current->next;
        free(current);

        if (del)
        {
            printf("\033[1;32m");
            printf("\n================ Deleted User ===============\n");
            printf("#       : %d \n", del->data._id);
            printf("ID      : %s \n", del->data.id);
            printf("fname   : %s \n", del->data.fname);
            printf("lname   : %s \n", del->data.lname);
            printf("================ ============= ===============\n");
            printf("\033[0m");

            printf("\033[1;32m");
            printf("> Delete Success\n");
            printf("\033[0m");
            printf("\033[38;5;50m( Any key to continue ) \033[0m \n");
        }

        return USER_LIST; // Return the new head of the list
    }

    // Search for the node with the given ID
    while (current != NULL && current->data._id != id)
    {
        del = current;
        prev = current;
        current = current->next;
    }

    // If the node with the given ID is found
    if (current != NULL)
    {

        del = current;
        prev->next = current->next;

        // Free the memory of the deleted node
    }
    else
    {
        // Handle the case where the node with the given ID was not found
        printf("User with ID %d not found in the list.\n", id);
    }

    // Print information about the deleted user (optional)

    if (del)
    {
        printf("\n================ Delete User ===============\n");
        printf("\033[1;32m");
        printf("#       : %d \n", del->data._id);
        printf("ID      : %s \n", del->data.id);
        printf("fname   : %s \n", del->data.fname);
        printf("lname   : %s \n", del->data.lname);
        printf("\033[0m");
        printf("================ ============= ===============\n");

        printf("\033[1;32m");
        printf("> Delete Success\n");
        printf("\033[0m");
        printf("\033[38;5;50m( Any key to continue ) \033[0m \n");
        free(del);
    }

    return USER_LIST; // Return the head of the modified list
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

int getRowsByColumn(int index){
    // Generate an account ID based on max+1 of number of Users.csv Data
    FILE *file = fopen(USERS_DATA, "r"); // Replace "data.csv" with your CSV file name
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int columnToRead = index; // Specify the column index you want to read (0-based index)
    char line[MAX_LINE_SIZE];
    int skip =0;
    int length = 0;
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int columnIndex = 0;

        while (token != NULL && skip > 0) {
            
            if (columnIndex == columnToRead) {
                printf("# %d: %s\n", columnIndex, token);
                break;
            }

            token = strtok(NULL, ",");
            columnIndex++;
            length++;
        }
        skip++;
    }

    fclose(file);
    return length;

}

int getMaxAccountID(){
    // Generate an account ID based on max+1 of number of Users.csv Data
    FILE *file = fopen(USERS_DATA, "r"); // Replace "data.csv" with your CSV file name
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int columnToRead = 1; // Specify the column index you want to read (0-based index)
    char line[MAX_LINE_SIZE];
    int skip =0;
    int length = 0;
    int max = 0;
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int columnIndex = 0;

        while (token != NULL && skip > 0) {
            
            if (columnIndex == columnToRead) {
                if(atoi(token) > max){
                    max = atoi(token);
                }
                break;
            }

            token = strtok(NULL, ",");
            columnIndex++;
            length++;
        }
        skip++;
    }

    fclose(file);
    return max;

}

void getNewAccountID(User *user){
    char maxStrLen[MAX_ACCCOUNT_ID_SIZE];
    int maxID = getMaxAccountID(1)+1;

    itoa(maxID,maxStrLen ,10);
    printf("Max AccountID : %s \n",maxStrLen);

    int maxIdLength = strlen(maxStrLen);

    char result[MAX_ACCCOUNT_ID_SIZE] = "";
    int l  = 0;

    while (l < MAX_ACCCOUNT_ID_SIZE-maxIdLength)
    {
        strcat(result,"0");
        l++;
    }

    strcat(result,maxStrLen);


    strcpy(user->accountID , result);
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

    getNewAccountID(&newUser);
    newUser.active = 1;
    newUser.status = 1;
    newUser.balance =  0.00;

    
    printf("================ New User ============\n");
    printf("ID                  : %s \n", newUser.id);
    printf("Full Name           : %s %s\n", newUser.fname, newUser.lname);
    printf("Age                 : %d \n", newUser.age);
    printf("-------------------------------------\n");
    printf("AccountID           : %s \n", newUser.accountID);
    printf("Balance             :\033[38;5;48m %.2lf \033[0m \n", newUser.balance);
    printf("Register Time       : %s \n", newUser.registerTime);
    printf("======================================\n");

    if (isfileExists(USERS_DATA))
    {
        printf(_ADMIN_DEBUG ? "file is exist : Appended\n" : "");
        appendToCSV(USERS_DATA, &newUser);
    }
    else
    {
        printf(_ADMIN_DEBUG ? "file is not exist : Create\n" : "");
        saveUser(USERS_DATA, &newUser);
    }

    return newUser;
}

/* FILES Utilities Function */
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
            userData->age,
            userData->balance,
            userData->active,
            userData->status,
            userData->registerTime);

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

    fprintf(file,"\n");
    // Write User data
    fprintf(file, dataType,
            userData->id,
            userData->accountID,
            userData->fname,
            userData->lname,
            userData->age,
            userData->balance,
            userData->active,
            userData->status,
            userData->registerTime);

    // Close the file
    fclose(file);
}

void copyTo(char *dest, const char *src, size_t destSize)
{
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
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
                userData.age = atoi(token);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[5]) == 0)
                userData.balance = strtod(token, NULL);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[6]) == 0)
                userData.active = atoi(token);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[7]) == 0)
                userData.status = atoi(token);

            if (strcmp(fieldNames[currentField + 1], FIELD_NAME[8]) == 0)
                copyTo(userData.registerTime, token, sizeof(userData.registerTime));

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
    csvDataTable.arraySize = arraySize;

    USER_ARR = userArray;
    USER_ARR_SIZE = arraySize;

    printf("\n Process User Table [/]\n");
    return csvDataTable;
}

void generateRandomUserData(User *u)
{
    // Seed the random number generator with the current time

    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    // Generate a random ID
    for (int i = 0; i < 13; i++)
    {
        u->id[i] = '0' + rand() % 10;
    }
    u->id[13] = '\0';

    for (int i = 0; i < 13; i++)
    {
        u->accountID[i] = '0' + rand() % 10;
    }

    u->id[13] = '\0';

    // Null-terminate the string

    // Generate random first name and last name
    const char *firstNames[] = {"John", "Jane", "Michael", "Emma", "William", "Olivia", "David", "Sarah", "James", "Emily"};
    const char *lastNames[] = {"Smith", "Johnson", "Brown", "Davis", "Wilson", "Anderson", "Clark", "Hall", "Lee", "Moore"};

    strcpy(u->fname, firstNames[rand() % 5]);
    strcpy(u->lname, lastNames[rand() % 5]);
    strcpy(u->registerTime, getCurrentTime());
    // Generate a random age between 18 and 60
    u->age = 18 + rand() % 43;
}

SearchData binarySearchAccountID(User arr[], int size, int target) {
    int left = 0;
    int right = size - 1;
    SearchData FoundUser;


    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (atoi(arr[mid].accountID) == target) {
            FoundUser.result = 1;
            FoundUser.user = &arr[mid];

            return FoundUser; // Found the target value
        } else if (atoi(arr[mid].accountID) < target) {
            left = mid + 1; // Search the right half
        } else {
            right = mid - 1; // Search the left half
        }
    }

    FoundUser.result = 0;
    FoundUser.user = NULL;

    return FoundUser;
}

void searchAccount(char arrcountID[]){


    SearchData result = binarySearchAccountID(USER_ARR, USER_ARR_SIZE ,atoi(arrcountID)
    );
    
        if(result.result){
            printf("\n============== Seearch Result ==============\n");

            printf(" #              : %d \n", result.user->_id);
            printf(" ID             : %s \n", result.user->id);
            printf(" fname          : %s \n", result.user->fname);
            printf(" lname          : %s \n", result.user->lname);
            printf(" age            : %d \n", result.user->age);
            printf(" accountID      : %s \n", result.user->accountID);
            printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", result.user->balance);
            printf("\n============== Card Status =============\n");
            printf(" active         : %s \n", result.user->active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
            printf(" status         : %s \n", result.user->status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
            printf(" registerDate   : %s \n", result.user->registerTime);
            printf("==============================================\n");

        }else{
            printf("\n============== Seearch Result ==============\n");
            printf("ID  \033[38;5;75m %s  \033[0m Is Not Found\n",arrcountID);
            printf("==============================================\n");

        }
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

int main(int argc, char const *argv[])
{
    printf("\033[?25l"); // hide cursor

    if(_APP_START_CLEAR){
        system("cls");

    }

    struct tm *localTime;
    // Get the current time
    time(&lastestTime);
    localTime = localtime(&lastestTime);

    printf("Program started at: %s", asctime(localTime));

    /* User registeredUser = Register("1909300007092", "Captain", "Siwakron", 21); */

    getch();


    User *UserArray;
    UserNode *UserList;

    // Init User List
    Table userData = processCSVToLinkedList(USERS_DATA, 1);
    UserList = userData.list;
    UserArray = userData.array;

    printf("Users Numbers : %d \n",userData.arraySize);

    int i = 0;
    while (i != userData.arraySize )
    {
        printf("> %s \n",UserArray[i].accountID);
        i++;
    }
    
    searchAccount("00000000001");

    getch();

    USER_LIST = userData.list;

    selectUserList(1, userData.numRows, displayUserList);

    getch();
    return 0;
}