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

#include "model/User.c"

#include "config.c"
#include "lib/key.c"
#include "lib/array.c"
#include "lib/sort.c"

#include "lib/file.c"
#include "lib/search.c"

#include "./auth.c"


typedef enum TransectionType {
    Check = 1 ,
    Deposit = 2,
    Withdraw = 3,
    Transfers = 4,

} TransectionType;

typedef struct Transection {
    int result;
    double amount;
    char time[50];
    char balance[50];
    TransectionType type;

} Transection;


Transection check(User *userData);
Transection deposit(User *userData , double amount);
Transection withdraw(User *userData , double amount);
Transection transfers(User *userData ,char* destinationAccount, double amount);


Transection check(User *userData){
    printf("========================\n");

    printf("Checking\n");
    printf("Account ID  >   %s\n",userData->accountID);
    printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);
    printf("========================\n");



}

Transection deposit(User *userData , double amount){

    Transection transectionDetail;
    TransectionType type = Deposit;

    userData->balance += amount;

    saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

    printf("========================\n");

    printf("\033[1;32mDeposit %.2lf Success \033[0m\n", amount);
    printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);
    printf("========================\n");

    
    transectionDetail.result = 1;
    strcpy(transectionDetail.time,getCurrentTime());
    transectionDetail.type = type;
    transectionDetail.amount = amount;

    return transectionDetail;

}

Transection withdraw(User *userData , double amount){

    Transection transectionDetail;
    TransectionType type = Withdraw;

    userData->balance -= amount;

    saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

    printf("========================\n");

    printf("\033[1;32mWithdraw %.2lf Success \033[0m\n", amount);
    printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);
    printf("========================\n");
    
    transectionDetail.result = 1;
    strcpy(transectionDetail.time,getCurrentTime());
    transectionDetail.type = type;
    transectionDetail.amount = amount;

    return transectionDetail;

}


Transection transfers(User *userData ,char* destinationAccount, double amount){
    
    Transection transectionDetail;
    TransectionType type = Transfers;

    qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByAccountID); 
    
    SearchData destAccount = searchAccount(destinationAccount);

    
    if(destAccount.result == 1){
        destAccount.user->balance += amount;

         userData->balance -= amount;

        saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

        printf("========================\n");

        printf("\033[1;32mTransfers %.2lf Success \033[0m\n", amount);
        printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);

        printf("========================\n");
        
        transectionDetail.result = 1;
        strcpy(transectionDetail.time,getCurrentTime());
        transectionDetail.type = type;
        transectionDetail.amount = amount; 

        return transectionDetail;
    }

}

int main(int argc, char const *argv[])
{
    Table userData = processCSVToLinkedList(USERS_DATA, 1);

    Login loginData = login("1909300007098");

    if(loginData.isLogin == 1){
        printf("Logged : %s",loginData.loginTime);

        int i = 0;

        printf("\n============================\n");
        printf("Welcome back \n");
        printf("%s %s \n",USER_SEESION.User->fname , USER_SEESION.User->lname);
        printf("============================\n");

    }else{
        return 0;
    }


  /*   deposit(USER_SEESION.User,100);
    withdraw(USER_SEESION.User,200); */

    deposit(USER_SEESION.User,100);
    transfers(USER_SEESION.User,"00000000010",200);
    withdraw(USER_SEESION.User,200);


    check(USER_SEESION.User);
    

    






    

    
}

