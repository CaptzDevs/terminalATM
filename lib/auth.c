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





Login USER_SESSION;


char *getPassword(int pass_len);
int checkPassword(char* storedPassword);


char *getPassword(int pass_len)
{
    const int PASS_LEN = pass_len;
    char *password = (char *)malloc((PASS_LEN + 1) * sizeof(char)); // +1 for null terminator
    char ch;
    int i = 0;

    printf("> ");

    while (i < PASS_LEN)
    {
        ch = getch();
        if (ch == BACKSPACE_KEY)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else if (ch >= '0' && ch <= '9' && i < PASS_LEN)
        { // Only accept numeric characters and up to 6 digits
            password[i] = ch;
            printf("*");
            i++;
        }
    }

    password[i] = '\0';

    printf("\n");

    return password;
}


int checkPassword(char* storedPassword){
    //Validate Password

    printf("> Enter your password: ");

    char *password = getPassword(6);
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

            return 0;
        }
        
        printf("> Enter your password: ");
        password = getPassword(6);
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

            USER_SESSION.isLogin = 1;
            strcpy(USER_SESSION.loginTime, getCurrentTime());

            qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByAccountID); 

            userData = searchAccount(accID);

            USER_SESSION.User = userData.user;

            USER_SESSION = USER_SESSION;

            saveLoginDataToCSV(LOGIN_LOG_DATA,USER_SESSION,2);
            return USER_SESSION;
         }else{
            USER_SESSION.isLogin = 0;
            strcpy(USER_SESSION.loginTime, "\0");

            return USER_SESSION;
         }

    }else{
             printf("Hello New User \n");
            USER_SESSION.User = NULL;
            USER_SESSION.isLogin = 0;

            return USER_SESSION;
    }
}


Login loginCard(User* cardData){

    SearchData userData;
    int isValidPassword;

    printf("Login \n");
    printf("=========================\n");
        char accID[14];

        strcpy(accID,cardData->accountID);
         isValidPassword = checkPassword(cardData->password);

         if(isValidPassword){

            USER_SESSION.isLogin = 1;
            strcpy(USER_SESSION.loginTime, getCurrentTime());

            USER_SESSION.User = cardData;
            USER_SESSION = USER_SESSION;
            saveLoginDataToCSV(LOGIN_LOG_DATA,USER_SESSION,1);
            return USER_SESSION;
         }else if(isValidPassword){
            USER_SESSION.isLogin = 0;
            strcpy(USER_SESSION.loginTime, "\0");
            return USER_SESSION;
         }

   
}


