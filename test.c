#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_SIZE 1024

int getRowDataArray(int index){
    // Generate an account ID based on max+1 of number of Users.csv Data
    FILE *file = fopen("./Data/Users.csv", "r"); // Replace "data.csv" with your CSV file name
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int columnToRead = index; // Specify the column index you want to read (0-based index)
    int skip = 0;
    char line[MAX_LINE_SIZE];
    

    while (fgets(line, sizeof(line), file) ) {
        char *token = strtok(line, ",");
        int columnIndex = 0;
        
        while (token != NULL && skip > 0) {
            if (columnIndex == columnToRead) {
                printf("Column %d: %s\n", columnIndex, token);
                break;
            }

            token = strtok(NULL, ",");
            columnIndex++;
        }

        skip++;
    }

    fclose(file);

    return 0;

}

int main() {

   

    char strN[10];
    int maxID = 5+1;

    itoa(maxID,strN ,10);
    printf(">>> %s \n",strN);

    int maxIdLength = strlen(strN);

    char result[11];
    int i2  = 0;

    while (i2 < 10-maxIdLength)
    {
        strcat(result,"0");
        i2++;
    }

    strcat(result,strN);
    printf("%s",result);

    return 0;
}