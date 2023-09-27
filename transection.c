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


typedef enum TransactionType {
    Check = 1 ,
    Deposit = 2,
    Withdraw = 3,
    Transfers = 4,

} TransactionType;

typedef struct Transaction {
    int result;
    double amount;
    char time[50];
    char sourceAccount[14];
    char destAccount[14];
    TransactionType type;

} Transaction;


char TRANSACTION_DATA[30];

Transaction check(User *userData);
Transaction deposit(User *userData , double amount);
Transaction withdraw(User *userData , double amount);
Transaction transfers(User *userData ,char* destinationAccount, double amount);

void saveTransaction(const char *filename , Transaction transactionDetail);

Transaction check(User *userData){
    printf("=================================\n");

    printf("Checking\n");
    printf("Account ID  >   %s\n",userData->accountID);
    printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);

    printf("=================================\n");

}

Transaction deposit(User *userData , double amount){

    Transaction transactionDetail;
    TransactionType type = Deposit;

    userData->balance += amount;

    saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

    printf("=================================\n");

    printf("\033[1;32mDeposit %.2lf Success \033[0m\n", amount);
    printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);

    printf("=================================\n");


    transactionDetail.result = 1;
    strcpy(transactionDetail.time,getCurrentTime());
    transactionDetail.type = type;
    transactionDetail.amount = amount;

    strcpy(transactionDetail.sourceAccount,userData->accountID);
    strcpy(transactionDetail.destAccount,userData->accountID);



    saveTransaction(TRANSACTION_DATA,transactionDetail);

    

    return transactionDetail;

}

Transaction withdraw(User *userData , double amount){

    Transaction transactionDetail;
    TransactionType type = Withdraw;

    userData->balance -= amount;

    saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

    printf("=================================\n");
    printf("\033[1;32mWithdraw %.2lf Success \033[0m\n", amount);
    printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);
    printf("=================================\n");

    
    transactionDetail.result = 1;
    strcpy(transactionDetail.time,getCurrentTime());
    transactionDetail.type = type;
    transactionDetail.amount = amount;
    strcpy(transactionDetail.sourceAccount,userData->accountID);
    strcpy(transactionDetail.destAccount,userData->accountID);

    saveTransaction(TRANSACTION_DATA,transactionDetail);


    return transactionDetail;

}


Transaction transfers(User *userData ,char* destinationAccount, double amount){
    
    Transaction transactionDetail;
    TransactionType type = Transfers;

    qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByAccountID); 
    
    SearchData destAccount = searchAccount(destinationAccount);

    
    if(destAccount.result == 1){
        destAccount.user->balance += amount;

         userData->balance -= amount;

        saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

        printf("============ Transfers ============\n");

        printf("From      %s\n",userData->accountID);
        printf("              |\n");
        printf("              |\n");
        printf("\033[38;5;48m");
        printf("Amount     %.2lf \033[0m\n",amount);
        printf("              |\n");
        printf("              |\n");
        printf("              V\n");
        printf("To        %s\n", destinationAccount);
        printf("----------------------------------\n");
        printf("\033[1;32mTransfers %.2lf Success \033[0m\n", amount);
        printf("Balance     >   \033[38;5;48m\033[4m%.2lf\033[0m \n",userData->balance);

        printf("=================================\n");
        
        transactionDetail.result = 1;
        strcpy(transactionDetail.time,getCurrentTime());
        transactionDetail.type = type;
        transactionDetail.amount = amount; 
        strcpy(transactionDetail.sourceAccount,userData->accountID);
        strcpy(transactionDetail.destAccount,destinationAccount);

        char destAccTransaction[30];
        destAccTransaction[0] = '\0';

        strcat(destAccTransaction,"./transactions/");
        strcat(destAccTransaction,destinationAccount);
        strcat(destAccTransaction,".csv");

        saveTransaction(TRANSACTION_DATA,transactionDetail);
        saveTransaction(destAccTransaction,transactionDetail);
        


        return transactionDetail;
    }

}

void saveTransaction(const char *filename , Transaction transactionDetail)
{
    int isfileExist = isfileExists(filename);

 FILE *file ;
    if(isfileExist == 1) {
        file = fopen(filename, "a");

    }else{
       file = fopen(filename, "w");
    }

    
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    char headerFile[100];
    char dataType[100];

    // Write the header row1
    if(isfileExist == 0) {

        fprintf(file, "sourceAccount,destAccount,amount,result,type,time\n");

    }

    // Write User data
    fprintf(file, "%s,%s,%.2lf,%d,%d,%s",
            transactionDetail.sourceAccount,
            transactionDetail.destAccount,
            transactionDetail.amount,
            transactionDetail.result,
            transactionDetail.type,
            transactionDetail.time

            );
            
            fprintf(file,"\n");
    // Close the file
    fclose(file);
}


int main(int argc, char const *argv[])
{
    Table userData = processCSVToLinkedList(USERS_DATA, 1);

    Login loginData = login("1909300007093");

    if(loginData.isLogin == 1){
        printf("Logged : %s",loginData.loginTime);

        int i = 0;

        printf("\n============================\n");
        printf("Welcome back \n");
        printf("%s %s \n",USER_SEESION.User->fname , USER_SEESION.User->lname);
        printf("============================\n");

        strcat(TRANSACTION_DATA,"./transactions/");
        strcat(TRANSACTION_DATA,USER_SEESION.User->accountID);
        strcat(TRANSACTION_DATA,".csv");

        printf(">> %s",TRANSACTION_DATA);


    }else{
        return 0;
    }


    deposit(USER_SEESION.User,1500);
    transfers(USER_SEESION.User,"00000000009",22300);
    withdraw(USER_SEESION.User,200);
    check(USER_SEESION.User);
    

    
}

