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
#include "lib/key.c"
#include "lib/array.c"
#include "lib/file.c"


typedef enum Transection_type {
    Check = 1 ,
    Deposit = 2,
    Withdraw = 3,
    Transfers = 4,

} Transection_type;

typedef struct Transection {
    int result;
} Transection;


Transection check();
Transection deposit(char *accountID[] , double amount);
Transection withdraw();
Transection transfers();


Transection deposit(char *accountID[] , double amount){

    saveLinkedListToCSV(USERS_DATA,USER)
    
}

int main(int argc, char const *argv[])
{
    Transection_type t = Withdraw;
    printf("%d",t);
    return 0;
}
