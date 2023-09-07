#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h> 
#include <unistd.h> // for sleep function
#include <stdlib.h> // for the system function
#include <time.h>   // for nanosleep
#include <math.h>
#include <conio.h> 
#include <limits.h>
#include "test.c"



/* ============== CONFIG ================ */

#define true 1
#define false 0
#define _MENU_SIZE 10
#define _RFMENU true //refresh menu 


int PASS_SIZE = 8;
char PASSWORD[] = "123";
// #define DEBUG true

/* ============================== */

    // Makeup data

    int m_id = 123;
    char m_fname[100] = "SIWAKRON";
    char m_lname[100] = "JANSANGSRI";
    int m_age = 21;

/* ============================== */


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
    int age;

} user;

typedef struct Account
{
    int AccountID;
    long double balance;

} account;

user Register(int id, char fname[], char lname[] , int age)
{
    user newUser;
    newUser.id = id;
    strcpy(newUser.fname, fname);
    strcpy(newUser.lname, lname);
    newUser.age = age;

    return newUser;
}

/* GROBAL */
char MAIN_MENU[_MENU_SIZE][20] = {
    "Check Balance",
    "Deposit",
    "Withdraw",
    "Transfer",
    "Account Setting"
};

void renderString(char str[],int duration);
int checkPassword();
void displayMenu(int ch);
void printfcss(int fontColor, int bgColor ,int style);
int selectMenu(int min , int max);
void displayLogo();
/* Utililies Function */

void displayMenu(int choice)
{   
    if(_RFMENU) system("cls");
    char(*optionPtr)[20];
    int optionIndex = 1;
    optionPtr = &MAIN_MENU[0];


    displayLogo();
    printf("Main Menu :\n");

    while (*optionPtr[0] != '\0')
    {
        if(choice == optionIndex){
            printf("\033[1;32m> \033[4m"); // Set text color to green and enable bold
        }
        printf("[%d] : %s\n", optionIndex, *optionPtr);

        if(choice == optionIndex){
            printf("\033[0m"); // Set text color to green and enable bold
        }

      /*if(choice == 1 && optionIndex == 1){
            printf(" - Hello 1 \n");
            printf(" - Hello 1 \n");
            printf(" - Hello 1 \n");
        } */
        

        optionPtr++;
        optionIndex++;
    }

    if(choice == 0){
        printf("\033[1;32m> \033[4m");
    }
        printf("[0] : Quit\n");
    if(choice == 0){
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

        renderString("\rCARD IS INSERTED       ",1);

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


void displayLogo(){
    
    printf("\033[1;32m"); // Set text color to green and enable bold
    printf("\n-------------------------\n");
    printf("|      ATM CARD APP      |\n");
    printf("|        v1.0.0          |\n");
    printf("------------------------\n");

    printf("\033[0m"); // Reset text attributes to default
}

void displayMenuHeader(char *menuName){
    printf("\033[1;32m"); // Set text color to green and enable bold
    printf("|  %s  |\n",menuName);
    printf("\033[0m"); 
}


int nsleep(float time , float dec){
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = time * pow(10, dec); 

    nanosleep(&delay, NULL);

}

void progressBar(){

  for (int i = 0; i < 100; i++) {
            nsleep(1,7);

            int width = (i + 1) / 4;
            printf("\r[");
            for (int j = 0; j < 25; j++) {
                if (j < width) {
                    printf("#");
                } else {
                    printf(" ");
                }
            }
            printf("]");
            fflush(stdout);
        }
    }

void moveUp(){
    printf("\033[1F");
    printf(">");

}


void preLoad2(int sleepTime){
        printf("> Initial                ");
        loadingCircle(sleepTime);
        printf("[/] \n");
     

        printf("> Checking User          ");
        loadingCircle(sleepTime);
        printf("[/] \n");


        printf("> Creating Account       ");
        loadingCircle(sleepTime);
        printf("[/] \n");

        checkPassword();
        printf("> Loging In              ");
        loadingCircle(sleepTime);
        printf("[/] \n");

        printf("> Checking               ");
        loadingCircle(sleepTime);
        printf("[/] \n");

}

void preLoad(){
     printf("> Initial                      ");
        loadingCircle(4);

        printf("\r> Creating Account        ");
        loadingCircle(4);

        printf("\r> Loging In                ");
        loadingCircle(4);

        printf("\r> Checking                 ");
        loadingCircle(4);
}

void createFile() {
    FILE *file; // File pointer

    // Create a file for writing
    file = fopen("example.txt", "w");

    if (file == NULL) {
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
void readFile() {
    FILE *file; // File pointer

    // Open the file for reading
    file = fopen("example.txt", "r");

    if (file == NULL) {
        perror("Error opening file for reading");
        exit(1); // Exit with an error code
    }

    char buffer[100]; // Buffer to store read data

    // Read and print the contents of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // Close the file
    fclose(file);
}

char* getPassword(){
    PASS_SIZE = 2;
    char* password = (char*)malloc(9 * sizeof(char));; 
    char ch;
    int i = 0;

    printf("> Enter your password: ");

    while (1) {
        ch = getch(); 
        if (ch == 13) 
            break;
        else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b"); 
            }
        }
      else if (ch >= '0' && ch <= '9' && i < 8) { // Only accept numeric characters and up to 8 digits
            password[i] = ch;
            printf("*");
            i++;
        }
    }

    password[i] = '\0'; 
    //printf("\nYour password is: %s\n", password);
    return password;
}


int checkPassword(){
    //Validate Password
    char *password = getPassword();
    int checkPassword = 1;

    while (checkPassword <= 3 && strcmp(password,PASSWORD) != 0)
    {

        /* system("cls"); */
      /*   printf("\nchecking...");
        loadingCircle(2); */

        printf("\033[1;91m");
        printf("\rWrong Password %d times Try again\n",checkPassword);
        printf("\033[1;0m");
        

        if(checkPassword == 3){
            printf("\033[1;91m\n\033[1F");
            printf("xxxxxxxxxxxxxxxxxxxxxxxx\n");
            printf("Wrong Password 3 Times\n");
            printf("Exit Program\n");
            printf("xxxxxxxxxxxxxxxxxxxxxxxx\n");
            printf("\033[1;0m");

            exit(0);
        }
        
        password = getPassword();
        checkPassword++;


    }

        if(strcmp(password,PASSWORD) == 0){
            printf("\033[1;32m \r| Password CORRECT [/]        \033[1;0m\n");
            return 1;
        }
}

int selectMenu(int min , int max){
    char ch ;
    int i = 0;
    int num = 1;
    displayMenu(num);
    while (1) {
       // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if(ch > 0){
                printf("%d",ch);
                

            if(ch <= 57 && ch >= 48){
                num = ch-48;
                displayMenu(num);
            }else{

            switch (ch) {
                case 72: // Up arrow key
                         if(num > min){
                        num -= 1;
                    }else{
                        num = max;
                    }
                    displayMenu(num);
                    break;
                case 80: // Down arrow key
                
                    if(num < max){
                        num += 1;
                    }else{
                        num = min;
                    }
                   
                    displayMenu(num);
                    break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == 13) { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                break;
            }
        }

       /* printf("%d",num); */
    }
    
    return num;
}

/* =========================== */


/* =========================== */

int main(){

 /*    system("cls");  */
    displayLogo();
//   /*   createFile();
//     readFile(); */

       printf("\033[?25l"); // hide cursor
    
    if(/* insertCard() */ 1){

   /*      printf("\n=======================\n");

        preLoad2(2);

        printf("\033[1;32m\r> SUCCESS !   ");
        printf("\033[0m"); // Reset text attributes to default

        printf("\n=======================\n");
        sleep(2); */
        //system("cls");
       //printf("\e[?25h"); // show cursor

        int choice;

        while (1)
        {
            
            choice = selectMenu(0, 5); //0 - max menu's array size
            switch (choice)
            {
            case 1:
                system("cls");
                int n;
                displayMenuHeader("Check Balance");
                scanf("%d",&n);
                break;
            case 2:
                system("cls");
                displayMenuHeader("Deposit");

                break;
            case 3:
                system("cls");
                displayMenuHeader("Withdraw");

                break;
            case 4:
                system("cls");
                displayMenuHeader("Transfer");

                break;
            case 0:
                system("cls");
                printf("================ ENDED ================\n");
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
