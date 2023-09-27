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
#include <stdbool.h>

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
    Receive = 5,


} TransactionType;

typedef struct Transaction {
    int _id;
    int result;
    double amount;
    char time[50];
    char sourceAccount[14];
    char destAccount[14];
    User *sourceUser;
    User *destUser;

    TransactionType type;

} Transaction;

typedef struct TransactionNode
{
    Transaction data;
    struct TransactionNode *next;

} TransactionNode;


typedef struct TableTransaction
{
    int numRows;
    int numCols;
    int size;
    TransactionNode *list;
    Transaction *array;

} TableTransaction;

typedef struct Summary {
    int transactionNumbers;
    double totalReceive;
    double totalDeposits;
    double totalWithdrawals;
    double totalTransfers;
    double balance;
} Summary;


char transactionType[5][10] = {"","Deposit","Withdraw","Transfers","Receive"};

char TRANSACTION_DATA[30];

Transaction check(User *userData);
Transaction deposit(User *userData , double amount);
Transaction withdraw(User *userData , double amount);
Transaction transfers(User *userData ,char* destinationAccount, double amount);


void saveTransaction(const char *filename , Transaction transactionDetail);
void showTransaction(Transaction TransactionDetail );


Transaction check(User *userData){
    Transaction transactionDetail;
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
    transactionDetail.sourceUser = userData;
    transactionDetail.destUser = userData;

    strcpy(transactionDetail.sourceAccount,userData->accountID);
    strcpy(transactionDetail.destAccount,userData->accountID);


    saveTransaction(TRANSACTION_DATA,transactionDetail);
    showTransaction(transactionDetail);

    
    return transactionDetail;

}

Transaction withdraw(User *userData , double amount){

    Transaction transactionDetail;
    TransactionType type = Withdraw;

     if(userData->balance < amount){

        printf("\033[1;31mInsufficient balance! \n");
        printf("Can't Not Withdraw\033[0m");

    printf("\n=================================\n");

        transactionDetail.result = 0;
        transactionDetail.type = type;
        transactionDetail.amount = amount; 
        transactionDetail.sourceUser = userData;
        transactionDetail.destUser = userData;

        strcpy(transactionDetail.time,getCurrentTime());
        strcpy(transactionDetail.sourceAccount,userData->accountID);
        strcpy(transactionDetail.destAccount,userData->accountID);

        return transactionDetail;
    }

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
    transactionDetail.sourceUser = userData;
    transactionDetail.destUser = userData;
    

    strcpy(transactionDetail.sourceAccount,userData->accountID);
    strcpy(transactionDetail.destAccount,userData->accountID);

    saveTransaction(TRANSACTION_DATA,transactionDetail);
    showTransaction(transactionDetail);


    return transactionDetail;

}


Transaction transfers(User *userData ,char* destinationAccount, double amount){
    
    Transaction transactionDetail;
    TransactionType type = Transfers;

    if(userData->balance < amount){

        printf("\033[1;31mInsufficient balance! \n");
        printf("Can't Not Transfers\033[0m");

        printf("\n*=============================\n");

        transactionDetail.result = 0;
        transactionDetail.type = type;
        transactionDetail.amount = amount; 
        transactionDetail.sourceUser = userData;
        transactionDetail.destUser = userData;

        strcpy(transactionDetail.time,getCurrentTime());
        strcpy(transactionDetail.sourceAccount,userData->accountID);
        strcpy(transactionDetail.destAccount,destinationAccount);

        return transactionDetail;
    }


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
        transactionDetail.type = type;
        transactionDetail.amount = amount; 
        transactionDetail.sourceUser = userData;
        transactionDetail.destUser = userData;

        strcpy(transactionDetail.time,getCurrentTime());
        strcpy(transactionDetail.sourceAccount,userData->accountID);
        strcpy(transactionDetail.destAccount,destinationAccount);

        char destAccTransaction[30];
        destAccTransaction[0] = '\0';

        strcat(destAccTransaction,"./transactions/");
        strcat(destAccTransaction,destinationAccount);
        strcat(destAccTransaction,".csv");

        saveTransaction(TRANSACTION_DATA,transactionDetail);

        showTransaction(transactionDetail);

        transactionDetail.type = 5;
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


TableTransaction processTransactionCSVToLinkedList(const char *filename, int choice)
{
    FILE *file = fopen(filename, "r");
    TableTransaction csvDataTable;
    Transaction transactionData, *userArray;
    TransactionNode *current = NULL;
    TransactionNode *userHead = NULL;

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
                transactionData._id = currentRow;

            if (strcmp(fieldNames[currentField + 1], "sourceAccount") == 0)
                copyTo(transactionData.sourceAccount, token, sizeof(transactionData.sourceAccount));

            if (strcmp(fieldNames[currentField + 1], "destAccount") == 0)
                copyTo(transactionData.destAccount, token, sizeof(transactionData.destAccount));

            if (strcmp(fieldNames[currentField + 1], "amount") == 0)
                transactionData.amount = strtod(token, NULL);

            if (strcmp(fieldNames[currentField + 1],"result") == 0)
                transactionData.result = atoi(token);

            if (strcmp(fieldNames[currentField + 1],"type") == 0)
                transactionData.type = atoi(token);

            if (strcmp(fieldNames[currentField + 1], "time") == 0)
                copyTo(transactionData.time, token, sizeof(transactionData.time));



       

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

        TransactionNode *newTrasactionNode = NULL;
        newTrasactionNode = malloc(sizeof(struct UserNode));
        newTrasactionNode->data = transactionData;
        newTrasactionNode->next = NULL;

        current = userHead;

        if (userHead == NULL)
        {
            userHead = newTrasactionNode;
        }
        else
        {
            while (current)
            {
                if (!current->next)
                {
                    current->next = newTrasactionNode;

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
/*     userArray = linkedListToArray(userHead, currentRow - 1, &arraySize); */

    csvDataTable.numRows = currentRow - 1;
    csvDataTable.numCols = fieldCount;

    csvDataTable.list = userHead;
    csvDataTable.array = userArray; 
    csvDataTable.size = arraySize;


    printf("\n Process User Table [/]\n");

    return csvDataTable;
}

// Function to parse date and time strings and compare them
bool  isDateInRange(const char* dateStr, const char* startDateStr, const char* endDateStr) {
    return (strcmp(dateStr, startDateStr) >= 0 && strcmp(dateStr, endDateStr) <= 0);
}

// Function to select transactions within a date and time range
Summary  selectTransactionsByDateTimeRange(TransactionNode *transactionData, int numTransactions, const char* startDateTime, const char* endDateTime , int type) {

    TransactionNode *current = transactionData;
    Summary summary = {0.0, 0.0, 0.0, 0.0};
    int n = 0;
    while (current)
    {

        if (isDateInRange(current->data.time, startDateTime, endDateTime)) {
            
            if(type == 0 || current->data.type == type){

                switch (current->data.type) {
                    case 2:
                        summary.totalDeposits += current->data.amount;
                        break;
                    case 3:
                        summary.totalWithdrawals += current->data.amount;
                        break;
                    case 4:
                        summary.totalTransfers += current->data.amount;
                        break;
                    case 5:
                        summary.totalReceive += current->data.amount;
                        break;
                    default:
                        break;
                }

                if(current->data.type == 5){
                    printf("%-10s |","Receive");
                }else{
                    printf("%-10s |",transactionType[current->data.type-1]);
                }
                printf("%-10s ",current->data.sourceAccount);
                printf(" -> ");
                printf("%-10s ",current->data.destAccount);

                if(current->data.type == 2 || current->data.type == 5) printf("\033[1;32m");

                if(current->data.type == 3 || current->data.type == 4 ){
                    printf("\033[1;31m");
                } else if(current->data.type == 4){
                    printf("\033[1;32m");
                }
                    
                
                printf("%.2lf ",current->data.amount);

                
                printf("\033[0m");

                printf("\n");
                n+=1;
            }
        }


            current = current->next;

    }
    summary.transactionNumbers = n;

    double WT = (summary.totalWithdrawals+summary.totalTransfers);
    double DR = (summary.totalDeposits+summary.totalReceive);

    if(WT == 0){
        summary.balance = DR;
    }else{
       summary.balance = WT-DR;
    }

    return summary;
}


void showTransaction(Transaction TransactionDetail ){

    
    printf("==================================\n");
    printf("             %s     \n",transactionType[TransactionDetail.type-1]);
    printf("       %s     \n", TransactionDetail.time);

    printf("----------------------------------\n");
    printf("From            : %s %s\n", TransactionDetail.sourceUser->fname,TransactionDetail.sourceUser->lname);
    printf("                : %s\n", TransactionDetail.sourceAccount);
    printf("\n");
    printf("To              : %s %s\n", TransactionDetail.destUser->fname,TransactionDetail.destUser->lname);
    printf("                : %s\n", TransactionDetail.destAccount);

    printf("----------------------------------\n");
    printf("Amount          : \033[38;5;48m%.2lf\033[0m\n", TransactionDetail.amount);
    printf("==================================\n");

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

        strcat(TRANSACTION_DATA,"./transactions/");
        strcat(TRANSACTION_DATA,USER_SEESION.User->accountID);
        strcat(TRANSACTION_DATA,".csv");

        printf(">> %s \n",TRANSACTION_DATA);


    }else{
        return 0;
    }


   transfers(USER_SEESION.User,"00000000010",100);
 /*    deposit(USER_SEESION.User,1500);
    transfers(USER_SEESION.User,"00000000010",22300);
    withdraw(USER_SEESION.User,999999999);
    check(USER_SEESION.User);
     */

    TableTransaction transactionData = processTransactionCSVToLinkedList(TRANSACTION_DATA,1);


    // Define the date and time range
    const char* startDateTime = "2023-01-01 00:00:00";
    const char* endDateTime = "2030-12-31 23:59:59";


    // Call the function to select transactions within the date and time range
    Summary summary = selectTransactionsByDateTimeRange(transactionData.list, transactionData.numRows, startDateTime, endDateTime,0);

    printf("\n");
    printf("============== Summary ==============\n");
    printf("Transaction(s)    : %d \n",summary.transactionNumbers);
    printf("----------------------------\n");
    printf("Deposits          : %.2lf\n", summary.totalDeposits);
    printf("Withdrawals       : %.2lf\n", summary.totalWithdrawals);
    printf("Transfers         : %.2lf\n", summary.totalTransfers);
    printf("Receive           : %.2lf\n", summary.totalReceive);
    
    
    printf("\n----------------------------\n");
    printf("Balance           : \033[38;5;48m%.2lf\033[0m\n", summary.balance);
    printf("============================\n");


    
}

