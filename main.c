#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h> 
#include <unistd.h> // for sleep function
#include <stdlib.h> // for the system function
#include <time.h>   // for nanosleep
#include <math.h>

#define true 1
#define false 0

typedef struct Node
{
    int data;
    struct Node *next;
} node;

typedef struct User
{
    int id;
    char fname[250];
    char lname[250];
} user;

typedef struct Account
{
    long double balance;

} account;

user Register(int id, char fname[], char lname[])
{

    user newUser;

    newUser.id = id;
    strcpy(newUser.fname, fname);
    strcpy(newUser.lname, lname);

    return newUser;
}

/* GROBAL */
char options[10][20] = {
    "Check Balance",
    "Deposit",
    "Withdraw",
    "Transfer"};

void renderString(char str[],int duration);
/* Utililies Function */

void displayMenu()
{

    char(*optionPtr)[20];
    int optionIndex = 1;
    optionPtr = &options[0];

    printf("Main Menu:\n");

    while (*optionPtr[0] != '\0')
    {
        printf("[%d] : %s\n", optionIndex, *optionPtr);
        optionPtr++;
        optionIndex++;
    }

    printf("[0] : Quit\n");

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


void loadingCircle(int timeoutArg){

    if(timeoutArg == -1){
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
        fflush(stdout); // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
    }
}

typedef int (*Operation)();

int asyncLoadingCircle(int timeoutArg , Operation checkFn){

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
        fflush(stdout); // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
        isClearTimeout = checkFn();
    }

    return isClearTimeout;
}


int randInt(int min, int max) { 

    srand(time(NULL));
    int random = rand();
    int result = (random % (max - min + 1)) + min;
    return result;

}

int checkInserCard(){
    return randInt(0,1);
}

int insertCard(){
    bool isInsertCard = 0;
    renderString("\nPlease Insert your card ",5);

    /* Checking card condition here */
    isInsertCard = asyncLoadingCircle(8,checkInserCard);

    if(isInsertCard){

        renderString("\rCARD IS INSERTED       ",2);

        return isInsertCard;
    }else{
        return 0;
    }
}

void renderString(char str[],int duration){

    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = duration * pow(10, 7); // .5sec 

    int i = 0;
    char sumStr[ strlen(str) ];

    while (i < strlen(str) && str[i] != '\0')
    {
        printf("%c", str[i] ); // Print the cursor character and move the cursor back
        fflush(stdout); // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
    }


}

void printLogo(){
    
    printf("\033[1;32m"); // Set text color to green and enable bold
    printf("-------------------------\n");
    printf("|      ATM CARD APP      |\n");
    printf("|        v1.0.0          |\n");
    printf("------------------------\n");

    printf("\033[0m"); // Reset text attributes to default
}

int main()
{

    printf("\e[?25l"); // hide cursor
    
    printLogo();
    
    if(insertCard()){

        printf("\n=======================\n");
        printf("> Initial ");
        
        loadingCircle(8);

        printf("\r> Checking ");

        loadingCircle(8);
        printf("\r\033[1;32m> SUCCESS!");
        printf("\033[0m"); // Reset text attributes to default

        printf("\n=======================\n");
        sleep(2);
        
        system("cls");

        printf("\e[?25h"); // show cursor

        int choice;

        while (1)
        {
            printLogo();
            displayMenu();

            printf("> ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("You selected Option 1.\n");
                break;
            case 2:
                printf("You selected Option 2.\n");
                break;
            case 3:
                printf("You selected Option 3.\n");
                break;
            case 0:
                printf("Thank you!\n");
                return 0;
            default:
                printf("\033[1;91mInvalid choice. Please choose a valid option (1/2/3/4).\n");
                sleep(1);
                system("cls");
                
            }
        }

    }

    return 0;  
}
