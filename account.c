#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <conio.h> // Console I/O (non-standard)
#include <math.h>
#include <sys/time.h>

#include "config.c"
#include "array.c"

#define MAX_LINE_SIZE 1024
#define MAX_FIELDS 10
#define MAX_ROW 1000


#define MAX_LIST_ROW 10


const char columnNameArr[10][20] = {
    "ID",
    "First Name",
    "Last Name",
    "Age",
    "Register Time"};

int space[] = {0, 15, 15, 15, 15, 15};

typedef struct User
{
    int _id;
    char id[14];
    char fname[250];
    char lname[250];
    char registerTime[50];
    int age;

    char accountID[14];

} User;

typedef struct UserNode
{
    User data;
    struct UserNode *next;
    
} UserNode;

typedef struct Table
{
    int numRows;
    int numCols;
    int arraySize;
    UserNode *list;
    User *array;
    
} Table;

typedef struct List
{
    int numRows;
    int numCols;
    int currentRow;
    int currentID;

    struct List *list;
    User *array;

} List;


UserNode *userHead = NULL;

char m_id[14] = "1909300007905";
char m_fname[100] = "SIWAKORN";
char m_lname[100] = "JANSANGSRI";
int m_age = 21;

typedef Table (*selectedTable)();
typedef List (*selectedList)();
typedef List (*selectedArray)();



User* linkedListToArray(UserNode* head, int* arraySize) {
    // First, count the number of nodes
    int count = 0;
    UserNode* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Allocate memory for the array of User structures
    User* userArray = (User*)malloc(count * sizeof(User));
    if (userArray == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy the data from nodes into the array
    current = head;
    for (int i = 0; i < count; i++) {
        userArray[i] = current->data;
        current = current->next;
    }

    *arraySize = count;
    return userArray;
}

List showUserArray(User* userArray, int arraySize, int choice , int page) {
      if(_CREAR_LIST) system("cls");


    printf("\n=============== User Array =================\n");

   
    float rowPerPage = ceil((float)arraySize / MAX_LIST_ROW);

    int start = MAX_LIST_ROW*(page-1)+1;
    int stop =  page*MAX_LIST_ROW; 

    printf("page %d\n",page);

    User *detail = NULL;
    List userList;
    

    if(start == 0){
        start = 1;
        stop = MAX_LIST_ROW;
    }

    int i = start-1; // run number
    int line = 1;
    int c = 1; // run line that can show per page
    int countRows = 0;
    
    printf("choice  > %d \n",choice);
    printf("start   > %d \n",start);
    printf("stop    > %d \n",stop);

   
        if(_SHOW_LIST_LINE) printf("|%-5s", "Line");
        printf("|%-5s",  "_ID");
        printf("|%-15s", "ID");
        printf("|%-20s", "Fname");
        printf("|%-20s", "Lname");
        printf("|%-4s", "Age\n");
        
        printf("\n");

        while (i < stop && i < arraySize){

            if (choice == line){
                
                printf("\033[1;32m\033[4m");
                detail = &userArray[i];
            }
            
         

               /*  printf("c %-5d|", c); */
                if(_SHOW_LIST_LINE) printf("|%-5d", line);
                printf("|%-5d", userArray[i]._id);
                printf("|%-15s", userArray[i].id);
                printf("|%-20s", userArray[i].fname);
                printf("|%-20s", userArray[i].lname);
                printf("|%-4d\n", userArray[i].age);

                countRows++;

            if (choice == line) printf("\033[0m");

            
            i++;
            line++;
            c++;

    }


        /* printf("c : %d\n",c); */
      /*   printf("========================\n");
        printf("Show %d row(s) (%d-%d)  from %d/%.0f row(s) \n",countRows,start,c-1,i-1 , listSize);
 */
        int max = i-1;
        
        printf("========================\n");
        int p = 1;
       /*  printf("%.2f \n",rowPerPage); */

      /*  printf("%d\n",start);
       printf("%d\n",stop);
 */


    int l = 1;
    int pageListPerRow = 10;
    printf(">> %.2f \n",rowPerPage);
    
  /*    for (p = 1; p <= rowPerPage; p++)
    {
        if(page == p) printf("\033[1;32m");
        printf("|%-3d ",p);
        if(page == p) printf("\033[0m");
    } */

    

        

        if(detail){
               printf("\n============== Detail ==============\n");

         printf(" #              : %d \n", detail->_id);
         printf(" ID             : %s \n", detail->id);
         printf(" fname          : %s \n", detail->fname);
         printf(" lname          : %s \n", detail->lname);
         printf(" age            : %d \n", detail->age);
         printf(" registerDate   : %s \n", detail->registerTime);

        /* printf("%d , %d",i,choice);
 */
        }

        userList.array = userArray;
        return userList;

}
void saveLinkedListToCSV(const char *filename, UserNode *head) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    // Write the header line
    fprintf(file, "ID,First Name,Last Name,Age,Register Time\n");

    UserNode *current = head;
    while (current != NULL) {
        User user = current->data;
        fprintf(file, "%s,%s,%s,%d,%s", user.id, user.fname, user.lname, user.age, user.registerTime);
        current = current->next;
    }

    fclose(file);
}


/* int selectRow(int min, int max, char fileName[], selectedTable tableCallBack);  old*/
int getListSize(UserNode *list){
    UserNode *current = list;
    int count = 0;
    if (current == NULL){
        return 0;
    }else{
        while (current)
        {
            count++;
            current = current->next;
        }
       
        return count;
    }
} 
UserNode* SearchUser(UserNode *list , int id){
    UserNode *current;
    current = list;
    if(!current){
        return current;
    }
    while (current)
    {
        if(current->data._id == id){
            printf("user id     : %s \n",current->data.id);
            printf("user fname  : %s \n",current->data.fname);
            printf("user lname  : %s \n",current->data.lname);

            return current;
        }
        current = current->next;
    }


    return current;
    
};

UserNode *deleteUser(UserNode *list, int id) {

    UserNode *head = list, *current = list, *prev = NULL, *del = NULL;

    // Handle the case where the list is empty
    if (!current) {
        return list;
    }

    // Check if the first node should be deleted
    if (current != NULL && current->data._id == id) {
        del = current;
        list = current->next;
        free(current);

        if(del){
            printf("\033[1;32m");
            printf("\n================ Delete User ===============\n");
            printf("#       : %d \n", del->data._id);
            printf("ID      : %s \n", del->data.id);
            printf("fname   : %s \n", del->data.fname);
            printf("lname   : %s \n", del->data.lname);
            printf("================ ============= ===============\n");
            printf("\033[0m");
        }

        return list; // Return the new head of the list
    }

    // Search for the node with the given ID
    while (current != NULL && current->data._id != id) {
        del = current;
        prev = current;
        current = current->next;

    }

    // If the node with the given ID is found
    if (current != NULL) {

        del = current;
        prev->next = current->next;

        // Free the memory of the deleted node
    } else {
        // Handle the case where the node with the given ID was not found
        printf("User with ID %d not found in the list.\n", id);
    }

        // Print information about the deleted user (optional)

        if(del){
            printf("\n================ Delete User ===============\n");
            printf("\033[1;32m");
            printf("#       : %d \n", del->data._id);
            printf("ID      : %s \n", del->data.id);
            printf("fname   : %s \n", del->data.fname);
            printf("lname   : %s \n", del->data.lname);
            printf("\033[0m");
            printf("================ ============= ===============\n");
            free(del);
        }



    return list; // Return the head of the modified list

}

int selectArrayRow(int min, int max, User *list , selectedArray tableCallBack){
  
    char ch;
    int i = 0;
    int row = 1;
    int page = 1;
    int currnentID;
    int flag;
    float numRows = 0;
    
    tableCallBack(list,max,row,page);

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            printf("%d \n",ch);

            if (ch <= 57 && ch >= 48)
            {
                row = ch - 48;

            }
            else
            {
                switch (ch)
                {
                case 72: // Up arrow key
                  
                            flag = max % MAX_LIST_ROW;

                            row -= 1;
                            if(row == 0){
                                row = flag;
                            }
                            if((float) page > (float) max/MAX_LIST_ROW && row > flag){
                                row = flag;
                            }

                            tableCallBack(list,max,row,page);
                    break;

                    case 75: // Up left key
                        {
                    
                         page -=1;

                        if(page <= 0){
                            page = 1; 
                        }
                             tableCallBack(list,max,row,page);

                    }
                    break;
                    case 77: // Up right key
                        {
                         page +=1;
                         

                               tableCallBack(list,max,row,page);
                       }
                    break;
                case 80: // Down arrow key

                    if (row < MAX_LIST_ROW)
                    {
                        row += 1;
                    }
                    else
                    {
                        row = 1;
                    }
                     flag = max % MAX_LIST_ROW;

                    if((float) page > (float) max/MAX_LIST_ROW && row > flag){
                        row = 1;
                    }
                        tableCallBack(list,max,row,page);
                    
                    break;

                    case 83: // Delete
                       {

                      /*   char ch2;
                        printf("\033[0;31m Do you want to delete this data ? \033[0m\n");
                        ch2 = getch();


                        if(ch2 == 13){  
                            list = deleteUser(list ,currnentID);
                            saveLinkedListToCSV("UserUpdated.csv",list);

                            if(row == max){
                                row = getListSize(list);
                            }
                            printf("%d \n",row);
                            printf("%d \n",max);
                            printf("%d \n",currnentID);


                            max -= 1;

                            getch();
                            rowDetail = tableCallBack(list, row,page);

                            currnentID = rowDetail.currentID;
                            row = rowDetail.currentRow;
                        }else{
                            rowDetail = tableCallBack(list, row,page);

                            currnentID = rowDetail.currentID;
                            row = rowDetail.currentRow;
                        } */
                       }
                    break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == 13)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return row;

}
int selectListRow(int min, int max, UserNode *list , selectedList tableCallBack)
{
    char ch;
    int i = 0;
    int row = 1;
    int page = 1;
    int currnentID;
    float numRows = 0;
    List rowDetail;
    rowDetail = tableCallBack(list, row,page);

    currnentID = rowDetail.currentID;
    row = rowDetail.currentRow;

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            printf("%d \n",ch);

            if (ch <= 57 && ch >= 48)
            {
                row = ch - 48;


                currnentID = rowDetail.currentID;
                row = rowDetail.currentRow;

            }
            else
            {
                switch (ch)
                {
                case 72: // Up arrow key
                  
                   if(row == MAX_LIST_ROW*(page-1)+1){
                        row = MAX_LIST_ROW*(page);
                    }
                    else if (row > MAX_LIST_ROW*(page-1)+1)
                    {
                        row -= 1;
                   
                    }
                    else
                    {
                        row = MAX_LIST_ROW*(page);
                    }

                   

                        rowDetail = tableCallBack(list, row,page);

                        printf("%d",MAX_LIST_ROW*(page-1)+1);
        
                        currnentID = rowDetail.currentID;
                        row = rowDetail.currentRow;

                    break;

                    case 75: // Up left key
                        {
                        float rowPerPage = ceil(numRows/MAX_LIST_ROW) ;
                        page -= 1;
                        
                        if(page <= 0) page = 1;
                         float flag = row%MAX_LIST_ROW;
                        row = MAX_LIST_ROW*(page-1)+flag; 


                        rowDetail = tableCallBack(list, row,page);

                        numRows = rowDetail.numRows;
                    }
                    break;
                    case 77: // Up right key
                        {
                         float flag = row%MAX_LIST_ROW;
                         page +=1;
                         
                        row = MAX_LIST_ROW*(page-1)+flag; 

                         rowDetail = tableCallBack(list, row,page);
                         numRows = rowDetail.numRows;
                       }
                    break;
                case 80: // Down arrow key

                    if (row < MAX_LIST_ROW*(page))
                    {
                        row += 1;
                    }
                    else
                    {
                        row = MAX_LIST_ROW*(page-1)+1;
                    }

                    
                        rowDetail = tableCallBack(list, row,page);
                        currnentID = rowDetail.currentID;
                        row = rowDetail.currentRow;
                    break;

                    case 83: // Delete
                       {

                        char ch2;
                        printf("\033[0;31m Do you want to delete this data ? \033[0m\n");
                        ch2 = getch();


                        if(ch2 == 13){  
                            list = deleteUser(list ,currnentID);
                            saveLinkedListToCSV("UserUpdated.csv",list);

                            if(row == max){
                                row = getListSize(list);
                            }
                           /*  printf("%d \n",row);
                            printf("%d \n",max);
                            printf("%d \n",currnentID); */


                            max -= 1;

                            getch();
                            rowDetail = tableCallBack(list, row,page);

                            currnentID = rowDetail.currentID;
                            row = rowDetail.currentRow;
                        }else{
                            rowDetail = tableCallBack(list, row,page);

                            currnentID = rowDetail.currentID;
                            row = rowDetail.currentRow;
                        }
                       }
                    break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == 13)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return row;
}

char *getCurrentTime()
{
    time_t current_time;
    struct tm *time_info;
    char *time_string = (char *)malloc(50 * sizeof(char)); // Allocate memory for the string

    if (time_string == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); // Exit the program if memory allocation fails
    }

    // Get the current time
    time(&current_time);

    // Convert the current time to a struct tm
    time_info = localtime(&current_time);

    // Format the time as a string
    strftime(time_string, 50, "%Y-%m-%d %H:%M:%S", time_info);
    return time_string; // Return the dynamically allocated string
}

User Register(const char id[], const char fname[], const char lname[], int age)
{
    User newUser;

    strncpy(newUser.id, id, sizeof(newUser.id) - 1);
    newUser.id[sizeof(newUser.id) - 1] = '\0';

    strncpy(newUser.fname, fname, sizeof(newUser.fname) - 1);
    newUser.fname[sizeof(newUser.fname) - 1] = '\0';

    strncpy(newUser.lname, lname, sizeof(newUser.lname) - 1);
    newUser.lname[sizeof(newUser.lname) - 1] = '\0';

    newUser.age = age;

    strncpy(newUser.registerTime, getCurrentTime(), sizeof(newUser.registerTime) - 1);
    newUser.registerTime[sizeof(newUser.registerTime) - 1] = '\0';

    return newUser;
}

int fileExists(const char *filename)
{
    return access(filename, F_OK) != -1;
}

void saveUser(const char *filename, const User *userData)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    int numColumns = 5;
    char combinedString[200] = ""; // Initialize an empty string to store the result

    for (int i = 0; i < numColumns; i++)
    {
        strcat(combinedString, columnNameArr[i]);

        // Append a comma and space if it's not the last column
        if (i < numColumns - 1)
        {
            strcat(combinedString, ",");
        }
    }
    // Write the header row
    /* fprintf(file, "ID,First Name,Last Name,Age\n"); */
    fprintf(file, strcat(combinedString, "\n"));

    // Write User data
    fprintf(file, "%s,%s,%s,%d,%s\n", userData->id, userData->fname, userData->lname, userData->age, userData->registerTime);

    // Close the file
    fclose(file);
}

void appendToCSV(const char *filename, const User *userData)
{
    FILE *file = fopen(filename, "a"); // Open in append mode
    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }
    // Write User data

    fprintf(file, "%s,%s,%s,%d,%s\n", userData->id, userData->fname, userData->lname, userData->age, userData->registerTime);

    // Close the file
    fclose(file);
}

void processValueCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_SIZE];
    while (fgets(line, sizeof(line), file))
    {
        // Tokenize the line using strtok
        char *token = strtok(line, ",");
        while (token)
        {
            // Process the token (field) here
            printf("Field: %s\n", token);

            // Get the next token
            token = strtok(NULL, ",");
        }
    }

    fclose(file);
}



List showLinkedList(UserNode *list, int choice , int page){
    if(_CREAR_LIST) system("cls");

    float listSize = getListSize(list);
    float rowPerPage = ceil(listSize/MAX_LIST_ROW) ;
    int start = MAX_LIST_ROW*(page-1)+1;
    int stop =  page*MAX_LIST_ROW; 

    printf("page %d\n",page);

    printf("=================== Linked List ===================\n");
 

    if(start == 0){
        start = 1;
        stop = MAX_LIST_ROW;
    }

    int i = 1; // run number
    int c = 1; // run line that can show per page
    int countRows = 0;

    printf("choice  > %d \n",choice);
    printf("start   > %d \n",start);
    printf("stop    > %d \n",stop);


    UserNode *current = NULL , *detail = NULL;
    List userList;
       if (list == NULL)
    { // show list
        printf(" List is Empty");
        /* printf("\npress any key to continue...\n"); */
    }
    else
    {
        current = list;
        if(_SHOW_LIST_LINE) printf("|%-5s", "Line");
        printf("|%-5s",  "_ID");
        printf("|%-15s", "ID");
        printf("|%-20s", "Fname");
        printf("|%-20s", "Lname\n");
        printf("\n");

        while (current != NULL && i <= stop){

            if (choice == i){
                
                printf("\033[1;32m\033[4m");
                detail = current;
                userList.currentRow = i;
                userList.currentID = current->data._id;
                
            }
            
            if(c >= start){
               /*  printf("c %-5d|", c); */
                if(_SHOW_LIST_LINE) printf("|%-5d", i);
                printf("|%-5d", current->data._id);
                printf("|%-15s", current->data.id);
                printf("|%-20s", current->data.fname);
                printf("|%-20s\n", current->data.lname);
                /* printf(" %-2d |\n", current->data.age); */
                countRows++;
            }
          

            if (choice == i)
                printf("\033[0m");


                

            current = current->next;
            
            i++;
            c++;

              if(c == stop){
                break;
            }

        }

        /* printf("c : %d\n",c); */
        printf("========================\n");
        printf("Show %d row(s) (%d-%d)  from %d/%.0f row(s) \n",countRows,start,c-1,i-1 , listSize);

        int max = i-1;
        
        printf("========================\n");
        int p = 1;
       /*  printf("%.2f \n",rowPerPage); */

      /*  printf("%d\n",start);
       printf("%d\n",stop);
 */

    int l = 1;
    int pageListPerRow = 10;
    /* printf(">> %.2f \n",rowPerPage); */
    
     for (p = 1; p <= rowPerPage; p)
    {
        for (l = 1; l <= pageListPerRow  && p <= rowPerPage; l++)
        {
                 if(page == p) printf("\033[1;32m");
                printf("|%-3d ",p);
                if(page == p) printf("\033[0m");
            p++;
        }
                printf("\n");
    }

    

        

        if(detail){
               printf("\n============== Detail ==============\n");

        printf(" #              : %d \n", detail->data._id);
        printf(" ID             : %s \n", detail->data.id);
        printf(" fname          : %s \n", detail->data.fname);
        printf(" lname          : %s \n", detail->data.lname);
        printf(" age            : %d \n", detail->data.age);
        printf(" registerDate   : %s \n", detail->data.registerTime);

        /* printf("%d , %d",i,choice);
 */
        }else{
            userList.currentRow = MAX_LINE_SIZE*(page-1)+1;
        }

        userList.numRows = i-1;
        return userList;
    }
}

void processCSV(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_SIZE];
    char *fieldNames[MAX_FIELDS]; // Array to store field names
    int fieldCount = 0;

    // Read the first line to get field names
    if (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
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
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int fieldIndex = 0;
        while (token && fieldIndex < fieldCount)
        {
            // Remove trailing newline character from the token, if present
            size_t len = strlen(token);
            if (token[len - 1] == '\n')
            {
                token[len - 1] = '\0';
            }

            // Access the field name and value based on fieldIndex
            printf("%s: %s\n", fieldNames[fieldIndex], token);

            // Get the next token
            token = strtok(NULL, ",");
            fieldIndex++;
        }
        printf("\n");
    }

    // Free memory for field names
    for (int i = 0; i < fieldCount; i++)
    {
        free(fieldNames[i]);
    }

    fclose(file);
}

void copyTo(char *dest, const char *src, size_t destSize)
{
    strncpy(dest, src, destSize - 1);
    dest[destSize - 1] = '\0';
}


Table processTableCSV(const char *filename, int choice)
{
    FILE *file = fopen(filename, "r");
    Table csvDataTable;
    User userData;

    UserNode *current = NULL;

    printf("\n=================== User Table ===================\n");

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

    // Print field names as the first row of the table
    for (int i = 0; i < fieldCount; i++)
    {
        printf("%-*s\t", space[i], fieldNames[i]);
    }

    printf("\n");

    // Process the data lines
    while (fgets(line, sizeof(line), file) && currentRow <= MAX_ROW)
    {
        char *token = strtok(line, ",");
        int currentField = 0;

        if (choice == currentRow)
            printf("\033[1;32m\033[4m");

        printf("%d\t", currentRow); // Print the row number

        if (choice == currentRow)
            printf("\033[0m");

        while (token && currentField < fieldCount)
        {

            if (strcmp(fieldNames[0],"#") == 0)
                userData._id = currentRow;

            if (strcmp(fieldNames[currentField + 1], columnNameArr[0]) == 0)
                copyTo(userData.id, token, sizeof(userData.id));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[1]) == 0)
                copyTo(userData.fname, token, sizeof(userData.fname));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[2]) == 0)
                copyTo(userData.lname, token, sizeof(userData.lname));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[3]) == 0)
                userData.age = atoi(token);

            if (strcmp(fieldNames[currentField + 1], columnNameArr[4]) == 0)
                copyTo(userData.registerTime, token, sizeof(userData.registerTime));

            // Remove trailing newline character from the token, if present
            size_t len = strlen(token);

            if (token[len - 1] == '\n')
            {
                token[len - 1] = '\0';
            }

            // Print data value with tab separator
            if (choice == currentRow)
                printf("\033[1;32m");

            printf("%-*s\t", space[currentField], token);

            if (choice == currentRow)
                printf("\033[0m");

            // Get the next token
            token = strtok(NULL, ",");
            currentField++;
        }

        printf("\n");

        UserNode *newUserNode = NULL;
        newUserNode = malloc(sizeof(struct UserNode));
        newUserNode->data = userData;
        newUserNode->next = NULL;

        current = userHead;

        if (userHead == NULL)
        {
            userHead = newUserNode;
        }
        else
        {
            while (current)
            {
                if (!current->next)
                {
                    current->next = newUserNode;

                    break;
                }
                current = current->next;
            }
        }

        /* printf("%s\n", newUserNode->data.id); */

        currentRow++; // Increment the row number
    }

    // Free memory for field names
    for (int i = 0; i < fieldCount; i++)
    {
        free(fieldNames[i]);
    }

    fclose(file);

    printf("\n============== Table Detail ==============\n");

    csvDataTable.numRows = currentRow - 1;
    csvDataTable.numCols = fieldCount;
    csvDataTable.list = userHead;

    printf("Cols            : %d \n", fieldCount);
    printf("Rows            : %d \n", currentRow - 1);
    printf("Selected At Row : %d \n", choice);

    printf("\n");

    printf("\n============== Last insert row ==============\n");


    printf(" #              : %d \n", currentRow-1);
    printf(" ID             : %s \n", userData.id);
    printf(" fname          : %s \n", userData.fname);
    printf(" lname          : %s \n", userData.lname);
    printf(" age            : %d \n", userData.age);
    printf(" registerDate   : %s \n", userData.registerTime);


    

    return csvDataTable;
}


Table processCSVToLinkedList(const char *filename, int choice)
{
    FILE *file = fopen(filename, "r");
    Table csvDataTable;
    User userData,*userArray;
    UserNode *current = NULL ;


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

    // Print field names as the first row of the table
   /*  for (int i = 0; i < fieldCount; i++)
    {
        printf("%-*s\t", space[i], fieldNames[i]);
    } */

  /*   printf("\n"); */

    // Process the data lines
    while (fgets(line, sizeof(line), file) && currentRow <= MAX_ROW)
    {
        char *token = strtok(line, ",");
        int currentField = 0;

        while (token && currentField < fieldCount)
        {

            if (strcmp(fieldNames[0],"#") == 0)
                userData._id = currentRow;

            if (strcmp(fieldNames[currentField + 1], columnNameArr[0]) == 0)
                copyTo(userData.id, token, sizeof(userData.id));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[1]) == 0)
                copyTo(userData.fname, token, sizeof(userData.fname));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[2]) == 0)
                copyTo(userData.lname, token, sizeof(userData.lname));

            if (strcmp(fieldNames[currentField + 1], columnNameArr[3]) == 0)
                userData.age = atoi(token);

            if (strcmp(fieldNames[currentField + 1], columnNameArr[4]) == 0)
                copyTo(userData.registerTime, token, sizeof(userData.registerTime));

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

        UserNode *newUserNode = NULL;
        newUserNode = malloc(sizeof(struct UserNode));
        newUserNode->data = userData;
        newUserNode->next = NULL;

        current = userHead;

        if (userHead == NULL)
        {
            userHead = newUserNode;
        }
        else
        {
            while (current)
            {
                if (!current->next)
                {
                    current->next = newUserNode;

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

    userArray = linkedListToArray(userHead,&arraySize);

    csvDataTable.numRows = currentRow - 1;
    csvDataTable.numCols = fieldCount;
    csvDataTable.list = userHead;
    csvDataTable.array = userArray;
    csvDataTable.arraySize = arraySize;


    
    printf("\n Process User Table [/]\n");
    return csvDataTable;
}

void generateRandomUserData(User *u)
{
    // Seed the random number generator with the current time

       struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    // Generate a random ID
    for (int i = 0; i < 13; i++)
    {
        u->id[i] = '0' + rand() % 10;
    }
    u->id[13] = '\0';
    
     // Null-terminate the string

    // Generate random first name and last name
       const char *firstNames[] = {"John", "Jane", "Michael", "Emma", "William", "Olivia", "David", "Sarah", "James", "Emily"};
    const char *lastNames[] = {"Smith", "Johnson", "Brown", "Davis", "Wilson", "Anderson", "Clark", "Hall", "Lee", "Moore"};

    strcpy(u->fname, firstNames[rand() % 5]);
    strcpy(u->lname, lastNames[rand() % 5]);
    strcpy(u->registerTime, getCurrentTime());
    // Generate a random age between 18 and 60
    u->age = 18 + rand() % 43;
}

int main(int argc, char const *argv[])
{

    /*  User registeredUser = Register(m_id, m_fname, m_lname, m_age); */
     printf("\033[?25l");
    int i = 0;
    int numUsers = 500;

    User registeredUser[numUsers] ;
    UserNode *UserList;
     

  /*  while (i < numUsers)
   {
        generateRandomUserData(&registeredUser[i]);
        appendToCSV("Users.csv", &registeredUser[i]);
        i++;
   }
    */


/*     printf("================ New User ============\n");
    printf("ID                  : %s \n", registeredUser.id);
    printf("User                : %s %s\n", registeredUser.fname, registeredUser.lname);
    printf("Age                 : %d \n", registeredUser.age);
    printf("Register Time       : %s \n", registeredUser.registerTime); */

    
/*     printf("======================================\n");

       if (fileExists("Users.csv"))
    {
        printf(_ADMIN_DEBUG ? "file is exist : APPENDED\n" : "");
        appendToCSV("Users.csv", &registeredUser);
    }
    else
    {
        printf(_ADMIN_DEBUG ? "file is not exist : CREATED\n" : "");
        saveUser("Users.csv", &registeredUser);
    } */

    printf("========================\n");
    printf("Table USERS \n");
    printf("========================\n");

   /*  Table userData = processTableCSV("Users", 1);
    UserList = userData.list; */

    /* showLinkedList(UserList);
    selectRow(1, userData.numRows, "Users", processTableCSV); */

    User *UserArray;

    Table userData = processCSVToLinkedList("Users.csv", 1);
    UserList = userData.list;
    UserArray = userData.array;

    
    selectListRow(1, userData.numRows, UserList, showLinkedList);

/*     UserNode *userF;

    userF = SearchUser(UserList,1);

    printf("> %s",userF->data.fname); */

   
   /*  selectArrayRow(1,userData.arraySize,UserArray,showUserArray); */



    return 0;
}