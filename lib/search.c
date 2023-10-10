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

typedef struct SearchData
{
    int result;
    User *user;

} SearchData;


SearchData binarySearchID(User arr[], int size, int target);
SearchData binarySearchAccountID(User arr[], int size, int target);
SearchData binarySearchTel(User arr[], int size, int target);




void showSearchResult(SearchData result , char* prop ,char* propName ){

       if(_SEARCH_RESULT){

        if (result.result)
        {
            printf("\n============== Search Result ==============\n");

            printf(" #              : %d \n", result.user->_id);
            printf(" ID             : %s \n", result.user->id);
            printf(" fname          : %s \n", result.user->fname);
            printf(" lname          : %s \n", result.user->lname);
            printf(" tel            : %s \n", result.user->tel);
            printf(" accountID      : %s \n", result.user->accountID);
            printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", result.user->balance);
            printf("\n============== Card Status =============\n");
            printf(" active         : %s \n", result.user->active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
            printf(" status         : %s \n", result.user->status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
            printf(" registerDate   : %s \n", result.user->registerTime);
            printf("==============================================\n");
        }
        else
        {
            printf("\n============== Search Result ==============\n");
            printf("%s \033[38;5;75m %s  \033[0m Is Not Found\n",propName,prop );
            printf("==============================================\n");
        }
    }
}


SearchData searchID(char id[14])
{
    qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByID);

    SearchData result = binarySearchID(USER_ARR, USER_ARR_SIZE, atol(id));

    showSearchResult(result , id, "ID. : ");

    return result;
}

SearchData searchAccount(char arrcountID[])
{
    SearchData result = binarySearchAccountID(USER_ARR, USER_ARR_SIZE, atol(arrcountID));

    //showSearchResult(result , arrcountID, "Account ID : ");

    return result;
}


SearchData searchTel(char tel[10])
{
    qsort(USER_ARR, USER_ARR_SIZE, sizeof(User), sortByTel);

    SearchData result = binarySearchTel(USER_ARR, USER_ARR_SIZE, atol(tel));

    showSearchResult(result , tel, "Tel. : ");
    return result;
}



SearchData binarySearchID(User arr[], int size, int target)
{   
    int left = 0;
    int right = size - 1;
    SearchData FoundUser;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (atol(arr[mid].id) == target)
        {
            FoundUser.result = 1;
            FoundUser.user = &arr[mid];

            return FoundUser; // Found the target value
        }
        else if (atol(arr[mid].id) < target)
        {
            left = mid + 1; // Search the right half
        }
        else
        {
            right = mid - 1; // Search the left half
        }
    }

    FoundUser.result = 0;
    FoundUser.user = NULL;

    return FoundUser;
}

SearchData binarySearchTel(User arr[], int size, int target)
{   
    int left = 0;
    int right = size - 1;
    SearchData FoundUser;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        
        if (atol(arr[mid].tel) == target)
        {
            FoundUser.result = 1;
            FoundUser.user = &arr[mid];

            return FoundUser; // Found the target value
        }
        else if (atol(arr[mid].tel) < target)
        {
            left = mid + 1; // Search the right half
        }
        else
        {
            right = mid - 1; // Search the left half
        }
    }

    FoundUser.result = 0;
    FoundUser.user = NULL;

    return FoundUser;
}

SearchData binarySearchAccountID(User arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;
    SearchData FoundUser;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (atol(arr[mid].accountID) == target)
        {
            FoundUser.result = 1;
            FoundUser.user = &arr[mid];

            return FoundUser; // Found the target value
        }
        else if (atol(arr[mid].accountID) < target)
        {
            left = mid + 1; // Search the right half
        }
        else
        {
            right = mid - 1; // Search the left half
        }
    }

    FoundUser.result = 0;
    FoundUser.user = NULL;

    return FoundUser;
}


