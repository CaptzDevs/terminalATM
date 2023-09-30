#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <conio.h> 
#include <math.h>
#include <sys/time.h>
#include <sys/stat.h>

/* #include "model/User.c"

#include "config.c"
#include "lib/key.c"
#include "lib/array.c"
#include "lib/sort.c"

#include "lib/file.c"
#include "lib/search.c" */



typedef struct Login
{
    int isLogin;
    char loginTime[50];
    User *User;
    
} Login;

Login USER_SEESION;


char* getPassword();
int checkPassword(char* storedPassword);


char* getPassword(){
    char* password = (char*)malloc(9 * sizeof(char));; 
    char ch;
    int i = 0;

    printf("> Enter your password: ");

    while (1) {
        ch = getch(); 
        if (ch == ENTER_KEY) 
            break;
        else if (ch == BACKSPACE_KEY) {
            if (i > 0) {
                i--;
                printf("\b \b"); 
            }
        }
      else if (ch >= '0' && ch <= '9' && i < 6) { // Only accept numeric characters and up to 8 digits
            password[i] = ch;
            printf("*");
            i++;
        }
    }

    password[i] = '\0'; 
    //printf("\nYour password is: %s\n", password);
    return password;
}

int checkPassword(char* storedPassword){
    //Validate Password
    char *password = getPassword();
    int checkPassword = 1;

    while (checkPassword <= 3 && strcmp(password,storedPassword) != 0)
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

        if(strcmp(password,storedPassword) == 0){
            printf("\033[1;32m \r| Password CORRECT [/]        \033[1;0m\n");
            return 1;
        }
}


Login login(char userID[14]){

    SearchData userData = searchID(userID);
    int isValidPassword;

    printf("Login \n");
    printf("=========================\n");

    char accID[14];

    if(userData.result == 1){

        strcpy(accID,userData.user->accountID);
         isValidPassword = checkPassword(userData.user->password);
         
         if(isValidPassword){

            USER_SEESION.isLogin = 1;
            strcpy(USER_SEESION.loginTime, getCurrentTime());

            qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByAccountID); 

            userData = searchAccount(accID);

            USER_SEESION.User = userData.user;

            USER_SEESION = USER_SEESION;

            return USER_SEESION;
         }else{
            USER_SEESION.isLogin = 0;
            strcpy(USER_SEESION.loginTime, '\0');

            return USER_SEESION;
         }

    }else{
             printf("Hello New User \n");
            USER_SEESION.User = NULL;
            USER_SEESION.isLogin = 0;

            return USER_SEESION;
    }
}



