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

#include "model/User.c"

#include "config.c"
#include "lib/key.c"
#include "lib/array.c"
#include "lib/sort.c"

#include "lib/file.c"
#include "lib/search.c"



typedef struct Login
{
    int isLogin;
    char loginTime[50];
    User *User;

    
} Login;

char* getPassword();
int checkPassword(char* storedPassword);


char* getPassword(){
    char* password = (char*)malloc(9 * sizeof(char));; 
    char ch;
    int i = 0;

    printf("> Enter your password: ");

    while (1) {
        ch = getch(); 
        if (ch == 13) 
            break;
        else if (ch == 6) {
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


Login login(char accountID[14]){

    SearchData userData = searchID(accountID);
    Login loginData;
    int isValidPassword;
    if(userData.result){

         isValidPassword = checkPassword(userData.user->password);
         
         if(isValidPassword){

            loginData.isLogin = 1;
            strcpy(loginData.loginTime, getCurrentTime());
            loginData.User = userData.user;


            return loginData;
         }else{
            loginData.isLogin = 0;
            strcpy(loginData.loginTime, '\0');

            return loginData;
         }

    }else{
        printf("Hello New User \n");
    }
}





int main(int argc, char const *argv[])
{
    Table userData = processCSVToLinkedList(USERS_DATA, 1);

    Login loginData = login("1909300007094");

    if(loginData.isLogin){
        printf("Logged : %s",loginData.loginTime);
        printf("Logged : %s",loginData.loginTime);

        int i = 0;

        strcpy(loginData.User->fname , "EIEI"); 


        saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

    }

    
}
