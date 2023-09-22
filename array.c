
#include <stdio.h>
#include <stdlib.h>
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

int* stringArrayToIntArray(const char** strArray, int size) {
    int* intArray = (int*)malloc(size * sizeof(int));

    if (intArray == NULL) {
        // Handle memory allocation error
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        intArray[i] = atoi(strArray[i]);
    }

    return intArray;
}


// Function to find the minimum value in an array
int findMin(const int arr[], int size) {
    int min = arr[0]; // Initialize min with the first element of the array

    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }

    return min;
}

// Function to find the maximum value in an array
int findMax(const int arr[], int size) {
    int max = arr[0]; // Initialize max with the first element of the array

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}


/* int main() {
    const char* strArray[] = {"1", "2", "3", "4", "5"};
    int size = sizeof(strArray) / sizeof(strArray[0]);

    int* intArray = stringArrayToIntArray(strArray, size);

    if (intArray == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", intArray[i]);
    }

    free(intArray);

    return 0;
}
 */
