#include <stdio.h>   // Standard I/O operations
#include <stdlib.h>  // Utility functions (memory, process,sysytem)
#include <stdbool.h> // Boolean type and constants
#include <string.h>  // String manipulation functions
#include <signal.h>  // Signal handling
#include <unistd.h>  // POSIX functions (e.g., sleep)
#include <time.h>    // Date and time operations
#include <math.h>    // Mathematical functions
#include <conio.h>   // Console I/O (non-standard)
#include <limits.h>  // Implementation-defined limits

#include <sys/time.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>
#include <malloc.h>

#include "model/User.c"

#include "config.c"
#include "lib/key.c"
#include "lib/array.c"
#include "lib/sort.c"
#include "lib/progress.c"


#include "lib/file.c"
#include "lib/search.c"


#include "lib/transaction.c"

/* ============== CONFIG ================ */

int PASS_SIZE = 8;
char PASSWORD[] = "123";

/* ============================== */

// Makeup data

/* ============================== */

typedef struct Node
{
    int data;
    struct Node *next;
} node;

/* GROBAL */
/* =============== MENU SETTING ===============*/

char MAIN_MENU[][50] = {
    "Deposit",
    "Withdraw",
    "Transfer",
    "Transactions",

};

char ACCOUNT_MENU[_MENU_SIZE][50] = {
    "My Account",
    "Account Setting",
    "Create Account",

};

typedef int (*Operation)();
typedef void (*selectedMenu)();

void renderString(char str[], int duration);
void displayMenu(int ch, char arr[][50], char header[]);
int selectMenu(int min, char arr[][50], selectedMenu fx, char header[]);
void printfcss(int fontColor, int bgColor, int style);
void displayLogo();

/* Utililies Function */

void displayMenu(int choice, char arr[][50], char header[])
{
    if (_RFMENU)
        system("cls");

    char(*optionPtr)[50];
    int optionIndex = 1;
    optionPtr = &arr[0];

    displayLogo();
    printf("%s :\n", header);

    while (*optionPtr[0] != '\0')
    {
        if (choice == optionIndex)
            printf("\033[1;32m> \033[4m");

        printf("[%d] : %s\n", optionIndex, *optionPtr);

        if (choice == optionIndex)
            printf("\033[0m");

        /*if(choice == 1 && optionIndex == 1){
              printf(" - Hello 1 \n");
              printf(" - Hello 1 \n");
              printf(" - Hello 1 \n");
          } */

        optionPtr++;
        optionIndex++;
    }

    if (choice == 0)
    {
        printf("\033[1;32m> \033[4m");
    }
    printf("[0] : Quit\n");
    if (choice == 0)
    {
        printf("\033[0m");
    }

    printf("\n \033[38;5;50m( Use Arrow and Enter for Select ) \033[0m \n");
}; // Print the list of Menu

void loadingBar()
{
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 5 * pow(10, 8);

    printf("[");
    int i;
    int numDots = 2; // Number of dots in the loading animation
    for (i = 0; i < numDots; i++)
    {
        printf("====");
        fflush(stdout); // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
    }

    printf("]\n");
}

void loadingCircle(int timeoutArg)
{

    if (timeoutArg == -1)
    {
        timeoutArg = 8;
    }

    int timeout = timeoutArg;
    int isClearTimeout = 0;
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 5 * pow(10, 8);

    char cursor[] = "|/-\\";
    int i = 0;

    while (i < timeout)
    {
        printf("%c\b", cursor[i % 4]); // Print the cursor character and move the cursor back
        fflush(stdout);                // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
    }
}

int asyncLoadingCircle(int timeoutArg, Operation checkFn)
{

    int timeout = timeoutArg;
    int isClearTimeout = 0;
    struct timespec delay;

    delay.tv_sec = 0;
    delay.tv_nsec = 5 * pow(10, 8);
    char cursor[] = "|/-\\";
    int i = 0;

    while (i < timeout && !isClearTimeout)
    {
        printf("%c\b", cursor[i % 4]); // Print the cursor character and move the cursor back
        fflush(stdout);                // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
        isClearTimeout = checkFn();
    }

    return isClearTimeout;
}

int randInt(int min, int max)
{

    srand(time(NULL));
    int random = rand();
    int result = (random % (max - min + 1)) + min;
    return result;
}

int checkInserCard()
{
    return randInt(0, 1);
}

int insertCard()
{
    bool isInsertCard = 0;
    renderString("\nPlease Insert your card ", 5);

    /* Checking card condition here */
    isInsertCard = asyncLoadingCircle(8, checkInserCard);

    if (isInsertCard)
    {

        renderString("\rCARD IS INSERTED       ", 1);

        return isInsertCard;
    }
    else
    {
        return 0;
    }
}

void renderString(char str[], int duration)
{

    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = duration * pow(10, 7); // .5sec

    int i = 0;
    char sumStr[strlen(str)];

    while (i < strlen(str) && str[i] != '\0')
    {
        printf("%c", str[i]); // Print the cursor character and move the cursor back
        fflush(stdout);       // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
    }
}

void displayLogo()
{

    printf("\033[1;32m"); // Set text color to green and enable bold
    printf("\n-------------------------\n");
    printf("|      ATM CARD APP      |\n");
    printf("|        v1.0.0          |\n");
    printf("------------------------\n");
    if(USER_SEESION.User){

    printf("Balance : \033[38;5;48m%.2lf\033[0m\n",USER_SEESION.User->balance);
    printf("------------------------\n");
    }

    printf("\033[0m"); // Reset text attributes to default
}

void displayMenuHeader(char *menuName)
{
    printf("\033[1;32m"); // Set text color to green and enable bold
    printf("|  %s  |\n", menuName);
    printf("\033[0m");
}

int nsleep(float time, float dec)
{
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = time * pow(10, dec);

    nanosleep(&delay, NULL);
}

void progressBar()
{
    int i, j;
    for (i = 0; i < 100; i++)
    {
        nsleep(1, 7);

        int width = (i + 1) / 4;
        printf("\r[");
        for (j = 0; j < 25; j++)
        {
            if (j < width)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("]");
        fflush(stdout);
    }
}

void moveUp()
{
    printf("\033[1F");
    printf(">");
}

void preLoad2(int sleepTime)
{
    printf("> Initial                ");
    loadingCircle(sleepTime);
    printf("[/] \n");

    Table userData = processCSVToLinkedList(USERS_DATA, 1);

    while (1)
    {
         char *userID = getID();

        printf("\n%s", userID);

        Login loginData = login(userID);


        if (loginData.isLogin == 1)
        {
            printf("Logged : %s", loginData.loginTime);

            int i = 0;

            printf("\n============================\n");
            printf("Welcome back \n");
            printf("%s %s \n", USER_SEESION.User->fname, USER_SEESION.User->lname);
            printf("============================\n");

            strcat(TRANSACTION_DATA, "./transactions/");
            strcat(TRANSACTION_DATA, USER_SEESION.User->accountID);
            strcat(TRANSACTION_DATA, ".csv");

            printf(">> %s \n", TRANSACTION_DATA);
            break;
        }
    }
    
    printf("> Checking User          ");
    loadingCircle(sleepTime);
    printf("[/] \n");

    printf("> Creating Account       ");
    loadingCircle(sleepTime);
    printf("[/] \n");

    /*    checkPassword(); */
    printf("> Loging In              ");
    loadingCircle(sleepTime);
    printf("[/] \n");

    printf("> Checking               ");
    loadingCircle(sleepTime);
    printf("[/] \n");
}

void preLoad()
{
    printf("> Initial                      ");
    loadingCircle(4);

    printf("\r> Creating Account        ");
    loadingCircle(4);

    printf("\r> Loging In                ");
    loadingCircle(4);

    printf("\r> Checking                 ");
    loadingCircle(4);
}

void createFile()
{
    FILE *file; // File pointer

    // Create a file for writing
    file = fopen("example.txt", "w");

    if (file == NULL)
    {
        perror("Error opening file for writing");
        exit(1); // Exit with an error code
    }

    // Write data to the file
    fprintf(file, "Hello, World!\n");
    fprintf(file, "This is a sample file created in C.\n");

    // Close the file
    fclose(file);
}

// Function to read data from a file and print it
void readFile()
{
    FILE *file; // File pointer

    // Open the file for reading
    file = fopen("example.txt", "r");

    if (file == NULL)
    {
        perror("Error opening file for reading");
        exit(1); // Exit with an error code
    }

    char buffer[100]; // Buffer to store read data

    // Read and print the contents of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    // Close the file
    fclose(file);
}

int selectMenu(int min, char arr[][50], selectedMenu displayMenuCallback, char header[])
{
    char ch;
    int i = 0;
    int num = 1;
    int max = lenC(arr);
    displayMenuCallback(num, arr, header);

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
                displayMenuCallback(num, arr, header);
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

                    displayMenuCallback(num, arr, header);

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

                    displayMenuCallback(num, arr, header);
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

/* =========================== */

/* =========================== */
void displayBalance(){
    if(USER_SEESION.User){
        printf("------------------------\n");
        printf("Balance : \033[38;5;48m%.2lf\033[0m\n",USER_SEESION.User->balance);
        printf("------------------------\n");
    }
}
int main()
{

    /*    system("cls");  */
    printf("\a");
    displayLogo();
    /*  createFile();
     readFile();  */

    printf("\033[?25l"); // hide cursor

    if (insertCard())
    {

        printf("\n=======================\n");

        preLoad2(0);

        printf("\033[1;32m\r> SUCCESS !   ");
        printf("\033[0m"); // Reset text attributes to default

        printf("\n=======================\n");
        sleep(2);
        // system("cls");
        // printf("\e[?25h"); // show cursor

        double amount  = 0;
        int choice = selectMenu(0, ACCOUNT_MENU, displayMenu, "Account Menu"); // 0 - max menu's array size

        while (1)
        {
          
            choice = selectMenu(0, MAIN_MENU, displayMenu, "Main Menu"); // 0 - max menu's array size
              
            switch (choice)
            {
            case 1:
                system("cls");
                displayBalance();
                displayMenuHeader("Deposit");
                amount = getAmount();
                if(amount > 0){
                    deposit(USER_SEESION.User, amount);
                    getch();
                }
                break;
            case 2:
                system("cls");
                displayBalance();
                displayMenuHeader("Withdraw");
                amount = getAmount();
                if(amount > 0){

                withdraw(USER_SEESION.User, amount);
                getch();
                }

                break;
            case 3:
               while (1)
               {
                    system("cls");
                    displayBalance();
                    displayMenuHeader("Transfer");
                    char *accountID = getAccountID();
                    if(strcmp(accountID, "0000000000") == 0) break;

                    printf("\n");
                    amount = getAmount();
                    printf("\n");
                    if(amount > 0){

                        Transaction transfersData = transfers(USER_SEESION.User, accountID, amount);
                        printf("%d",transfersData.result);
                        
                        if(transfersData.result == 1) {
                            getch();
                            break;
                        }else{
                            getch();
                        }

                    }

               }
               
                break;
            case 4:
                system("cls");
                displayBalance();
                displayMenuHeader("Transactions List");
                TableTransaction transactionData = processTransactionCSVToLinkedList(TRANSACTION_DATA, 1);
                

                if(transactionData.success == 1 || (transactionData.success == 1 && transactionData.numRows > 0) ){
                    // Define the date and time range
                    const char *startDateTime = "2023-01-01 00:00:00";
                    const char *endDateTime = "2030-12-31 23:59:59";

                    // Call the function to select transactions within the date and time range
                    Summary summary = selectTransactionsByDateTimeRange(
                        transactionData.list,
                        transactionData.numRows, 
                        startDateTime, 
                        endDateTime, 0);

                    getch();
                }else{

                    getch();
                }

                break;
            case 0:

            choice = selectMenu(0, ACCOUNT_MENU, displayMenu, "Account Menu");
     /*            system("cls");
                printf("================ ENDED ================\n");
                printf("Thank you!\n");
                return 0; */
                break;
            default:
                printf("\033[1;91mInvalid choice. Please choose a valid option (1/2/3/4).\n");
                sleep(1);
                system("cls");
            }
        }
    }

    return 0;
}
