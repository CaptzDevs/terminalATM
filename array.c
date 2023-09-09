#include <stdio.h>      // Standard I/O operations
#include <string.h>

int lenN(int arr[]){
    int length = 0;
    while (arr[length] != 0) {
        length++;
    }
    return length;
}

int lenC(char arr[][50]) {
    char(*ptr)[50];
    int i = 0;
    ptr = &arr[0];

    while (*ptr[0] != '\0')
    {
        ptr++;
        i++;
    }
    printf("l : %d \n",i);
    return i;
    
}
/* int n[] = {1,2,3};

int main(int argc, char const *argv[])
{   
    int numRows, maxLength;

    printf("%d",lenC(MAIN_MENU));
    printf("%d",lenN(n));



    return 0;
} */
