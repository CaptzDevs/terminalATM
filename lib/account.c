

// Money Color \033[38;5;48m

int space[] = {0, 15, 15, 15, 15, 15};

typedef Table (*selectedTable)();
typedef List (*selectedList)();
typedef List (*selectedArray)();
typedef void (*selectedUserMenu)();

/* ============================ */
char *getTel(int pass_len);
char *createPassword();

void displayUserMenu(int choice, char *arr[], char header[], User *userDetail);
int selectUserMenu(int min, char *arr[], selectedUserMenu displayMenuCallback, char header[], User *userDetail);

int sortByTel(const void *a, const void *b);
int getStringInput(char *input, int maxSize);
int selectUserArray(int min, int max, User *list, selectedArray tableCallBack);
int selectUserList(int min, int max, selectedList tableCallBack);

List displayUserList(int choice, int page);
List displayUserArray(User *userArray, int arraySize, int choice, int page);

UserNode *deleteUser(int id);
User *editUserData(User *userDetail);
SearchData searchTel(char tel[10]);

/* ============================ */

/*
    //* 2 Type of menu function
   //* 1. Controller Function      [C]
   //* 2. Display Function         [D]
*/
User *editUserData(User *userDetail)
{
    printf("ID           %-5s %s \n", ":", userDetail->id);
    printf("FirstName    %-5s %s \n", ":", userDetail->fname);
    printf("Lastname     %-5s %s \n", ":", userDetail->lname);
    printf("Tel          %-5s %s\n", ":", userDetail->tel);
    printf("\n");
    printf("\n\033[4m\033[38;5;50mLeave it blank for not change\033[0m\n\n");

    char nFname[250];
    char nLname[250];
    char nTel[11];

    strcpy(nFname, userDetail->fname);
    strcpy(nLname, userDetail->lname);
    strcpy(nTel, userDetail->tel);

    fflush(stdin);

/*     printf("New First Name : ");
    printf("\033[38;5;75m");
    int lFname = getStringInput(nFname, sizeof(nFname));
    printf("\033[0m");
    printf("New Last Name : ");
    printf("\033[38;5;75m");
    int lLname = getStringInput(nLname, sizeof(nLname));
    printf("\033[0m"); */

    printf("New Tel. : ");
    printf("\033[38;5;75m");

    strcpy(nTel, getTel(10));
    int lTel = strlen(nTel);
    printf("\033[0m");

  /*   printf("%d %d %d", lFname, lLname, lTel);

    if (lFname == 1)
        strcpy(nFname, userDetail->fname);
    if (lLname == 1)
        strcpy(nLname, userDetail->lname); */
    if (lTel < 10)
        strcpy(nTel, userDetail->tel);

    printf("\n");
    printf("Check new data \n");
    printf("=====================================\n");
    printf("%-15s -> %s \n", userDetail->tel, nTel);
    printf("=====================================\n");

    printf("Commit the change ? \n");
    printf("\033[1;32m > Enter  |   Save \n");
    printf("\033[1;31m > ESC    |   Discard \n");
    printf("\033[0m");

    strcpy(userDetail->tel, nTel);

    return userDetail;
}

int getStringInput(char *input, int maxSize)
{
    fgets(input, maxSize, stdin);

    // Remove the newline character from the end of the string
    int length = strlen(input);
    if (length > 0 && input[length - 1] == '\n')
    {
        input[length - 1] = '\0';
    }

    return length;
}

/* [D] */
void displayUserMenu(int choice, char *arr[], char header[], User *userDetail)
{

    if (_RFMENU)
        system("cls");

    printf("\n============== User Detail ==============\n");

    printf(" #              : %d \n", userDetail->_id);
    printf(" ID             : %s \n", userDetail->id);
    printf(" fname          : %s \n", userDetail->fname);
    printf(" lname          : %s \n", userDetail->lname);
    printf(" tel            : %s \n", userDetail->tel);
    printf(" accountID      : %s \n", userDetail->accountID);
    printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", userDetail->balance);
    printf("\n================= Card Status =============\n");
    printf(" active         : %s \n", userDetail->active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
    printf(" status         : %s \n", userDetail->status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
    printf(" registerDate   : %s \n", userDetail->registerTime);
    printf("=====================================\n");

    /*     char(*optionPtr)[50];

        optionPtr = &arr[0]; */
    int optionIndex = 1;

    printf("%s :\n", header);

    int i = 0;
    while (i < USER_ACTIVE_MENU_SIZE)
    {
        if (choice == optionIndex)
            printf("\033[1;32m \033[4m");

        if (choice == 2 && optionIndex == 2)
            if (userDetail->active == 1)
                printf("\033[1;31m");
        if (choice ==3 && optionIndex ==3)
            if (userDetail->status == 1)
                printf("\033[1;31m");
        ;

        printf("> [%d] : %s\n", optionIndex, arr[i]);

        if (choice == optionIndex)
            printf("\033[0m");
        i++;
        optionIndex++;
    }

    if (choice == 0)
    {
        printf("\033[1;32m> \033[4m");
    }
    printf("> [0] : Quit\n");
    if (choice == 0)
    {
        printf("\033[0m");
    }

    printf("\n \033[38;5;50m( Use Arrow and Enter for Select ) \033[0m \n");
}

/* [C] */
int selectUserMenu(int min, char *arr[], selectedUserMenu displayMenuCallback, char header[], User *userDetail)
{
    char ch;
    int i = 0;
    int num = 1;
    int max = USER_ACTIVE_MENU_SIZE;

    displayMenuCallback(num, arr, header, userDetail);

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character

        if (ch > 0)
        {
            /* printf("%d",ch); */
            if (ch <= 57 && ch >= 48)
            {
                num = ch - 48;
                displayMenuCallback(num, arr, header, userDetail);
            }
            else
            {

                switch (ch)
                {
                case UP_KEY: // Up arrow key
                    if (num > min)
                    {
                        num -= 1;
                    }
                    else
                    {
                        num = max;
                    }

                    displayMenuCallback(num, arr, header, userDetail);

                    break;
                case DOWN_KEY: // Down arrow key

                    if (num < max)
                    {
                        num += 1;
                    }
                    else
                    {
                        num = min;
                    }

                    displayMenuCallback(num, arr, header, userDetail);
                    break;

                case ENTER_KEY: // Down arrow key
                    if (_ADMIN_DEBUG)
                        printf("SELECT IN USER MENU\n");

                    switch (num)
                    {
                        // Edit
                    case 1:
                        printf("===============================\n");
                        printf("\t   %s\n", USER_MENU[num - 1]);
                        printf("===============================\n");

                        UserNode temp;
                        strcpy(temp.data.fname, userDetail->fname);
                        strcpy(temp.data.lname, userDetail->lname);
                        strcpy(temp.data.tel, userDetail->tel);

                        editUserData(userDetail);

                        char ch2;
                        ch2 = getch();
                        getch();
                        switch (ch2)
                        {
                        case ENTER_KEY:

                            saveArrayToCSV(USERS_DATA,USER_ARR,USER_ARR_SIZE);

                            printf("=====================================\n");
                            printf("\033[1;32m> Update Data Success \n\033[0m");
                            printf("=====================================\n");

                            getch();
                            displayMenuCallback(num, arr, header, userDetail);

                            break;

                        case EXIT_KEY:

                            strcpy(userDetail->fname, temp.data.fname);
                            strcpy(userDetail->lname, temp.data.lname);
                            strcpy(userDetail->tel, temp.data.tel);

                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);

                            break;

                        default:
                            break;
                        }

                        break;

                    case 2: // Suspend card
                        printf("=====================\n");
                        printf("%s\n", userDetail->active == 1 ? "SUSPEND CARD" : "ACTIVE CARD");
                        printf("=====================\n");

                        printf(userDetail->active == 1 ? "\033[0;31mDo you want to Suspend this card" : "\033[0;32mDo you want to Active this card");

                        printf(" ?\033[0m \n");

                        char chActive;
                        chActive = getch();

                        switch (chActive)
                        {
                        case ENTER_KEY:
                        {
                            if (userDetail->active == 1)
                            {
                                userDetail->active = 0;
                                saveArrayToCSV(USERS_DATA, USER_ARR, USER_ARR_SIZE);
                                printf("\033[0;31m> Suspended card [/]\033[0m ");
                                USER_ACTION_MENU[1] = "Active Card";
                            }
                            else if (userDetail->active == 0)
                            {
                                userDetail->active = 1;
                                saveArrayToCSV(USERS_DATA, USER_ARR, USER_ARR_SIZE);
                                printf("\033[0;32m> Active card [/]\033[0m ");
                                USER_ACTION_MENU[1] = "Suspended Card";
                            }
                            displayMenuCallback(num, arr, header, userDetail);

                            break;
                        }
                        case EXIT_KEY:
                        {
                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);
                        }
                        default:
                            break;
                        }

                        break;
                    case 3: // Disable card
                        printf("=====================\n");
                        printf("%s\n", userDetail->status == 1 ? "DISABLE CARD" : "ENABLE CARD");
                        printf("=====================\n");
                        printf(userDetail->status == 1 ? "\033[0;31mDo you want to Disable this card" : "\033[0;32mDo you want to Enable this card");
                        printf("?\033[0m \n");

                        char chStatus;
                        chStatus = getch();

                        switch (chStatus)
                        {
                        case ENTER_KEY:
                        {
                            if (userDetail->status == 1)
                            {
                                userDetail->status = 0;
                                saveArrayToCSV(USERS_DATA, USER_ARR, USER_ARR_SIZE);

                                printf("\033[0;31m> Disable card [/]\033[0m ");
                                USER_ACTION_MENU[2] = "Enable Card";
                            }
                            else if (userDetail->status == 0)
                            {
                                userDetail->status = 1;
                                saveArrayToCSV(USERS_DATA, USER_ARR, USER_ARR_SIZE);

                                printf("\033[0;32m> Enable card [/]\033[0m ");
                                USER_ACTION_MENU[2] = "Disable Card";
                            }

                            displayMenuCallback(num, arr, header, userDetail);

                            break;
                        }
                        case EXIT_KEY:
                        {
                            printf("EXIT \n");
                            displayMenuCallback(num, arr, header, userDetail);
                        }
                        default:
                            break;
                        }
                        break;
                    case 0:
                        return 0;
                        break;
                    default:
                        break;
                    }

                    break;
                case EXIT_KEY: // exit User detail menu
                    return 0;
                    break;
                default:
                    printf("\n");
                    break;
                }
            }
            // Enter Key
        }

        /* printf("Select : %d \n",num); */
    }

    return num;
};

// [D]
List displayUserArray(User *userArray, int arraySize, int choice, int page)
{
    if (_CREAR_LIST)
        system("cls");

    printf("\n=============== User Array =================\n");

    float rowPerPage = ceil((float)arraySize / MAX_LIST_ROW);

    int start = MAX_LIST_ROW * (page - 1) + 1;
    int stop = page * MAX_LIST_ROW;

    printf("page %d\n", page);

    User *detail = NULL;
    List userList;

    if (start == 0)
    {
        start = 1;
        stop = MAX_LIST_ROW;
    }

    int i = start - 1; // run number
    int line = 1;
    int c = 1; // run line that can show per page
    int countRows = 0;

    printf("choice  > %d \n", choice);
    printf("start   > %d \n", start);
    printf("stop    > %d \n", stop);

    if (_SHOW_LIST_LINE)
        printf("|%-5s", "Line");
    printf("|%-5s", "_ID");
    printf("|%-15s", "ID");
    printf("|%-20s", "Fname");
    printf("|%-20s", "Lname");
    printf("|%-10s", "Tel\n");

    printf("\n");

    while (i < stop && i < arraySize)
    {

        if (choice == line)
        {

            printf("\033[1;32m\033[4m");
            detail = &userArray[i];
        }

        /*  printf("c %-5d|", c); */
        if (_SHOW_LIST_LINE)
            printf("|%-5d", line);
        printf("|%-5d", userArray[i]._id);
        printf("|%-15s", userArray[i].id);
        printf("|%-20s", userArray[i].fname);
        printf("|%-20s", userArray[i].lname);
        printf("|%-10s\n", userArray[i].tel);

        countRows++;

        if (choice == line)
            printf("\033[0m");

        i++;
        line++;
        c++;
    }

    /* printf("c : %d\n",c); */
    /*   printf("========================\n");
      printf("Show %d row(s) (%d-%d)  from %d/%.0f row(s) \n",countRows,start,c-1,i-1 , listSize);
*/
    int max = i - 1;

    printf("========================\n");
    int p = 1;
    /*  printf("%.2f \n",rowPerPage); */

    /*  printf("%d\n",start);
     printf("%d\n",stop);
*/

    int l = 1;
    int pageListPerRow = 10;
    printf(">> %.2f \n", rowPerPage);

    /*    for (p = 1; p <= rowPerPage; p++)
      {
          if(page == p) printf("\033[1;32m");
          printf("|%-3d ",p);
          if(page == p) printf("\033[0m");
      } */

    if (detail)
    {
        printf("\n============== User Detail ==============\n");

        printf(" #              : %d \n", detail->_id);
        printf(" ID             : %s \n", detail->id);
        printf(" fname          : %s \n", detail->fname);
        printf(" lname          : %s \n", detail->lname);
        printf(" tel            : %d \n", detail->tel);
        printf(" accountID      : %s \n", detail->accountID);
        printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", detail->balance);
        printf("\n================= Card Status =============\n");
        printf(" active         : %s \n", detail->active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
        printf(" status         : %s \n", detail->status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
        printf(" registerDate   : %s \n", detail->registerTime);
        printf("=====================================\n");

        /* printf("%d , %d",i,choice);
         */
    }

    userList.array = userArray;
    return userList;
}

// [C]
int selectUserArray(int min, int max, User *list, selectedArray tableCallBack)
{

    char ch;
    int i = 0;
    int row = 1;
    int page = 1;
    int currnentID;
    int flag;
    float numRows = 0;

    tableCallBack(list, max, row, page);

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            printf("%d \n", ch);

            if (ch <= 57 && ch >= 48)
            {
                row = ch - 48;
            }
            else
            {
                switch (ch)
                {
                case UP_KEY: // Up arrow key

                    flag = max % MAX_LIST_ROW;

                    row -= 1;
                    if (row == 0)
                    {
                        row = flag;
                    }
                    if ((float)page > (float)max / MAX_LIST_ROW && row > flag)
                    {
                        row = flag;
                    }

                    tableCallBack(list, max, row, page);
                    break;

                case LEFT_KEY: // Up left key
                {

                    page -= 1;

                    if (page <= 0)
                    {
                        page = 1;
                    }
                    tableCallBack(list, max, row, page);
                }
                break;
                case RIGHT_KEY: // Up right key
                {
                    page += 1;

                    tableCallBack(list, max, row, page);
                }
                break;
                case DOWN_KEY: // Down arrow key

                    if (row < MAX_LIST_ROW)
                    {
                        row += 1;
                    }
                    else
                    {
                        row = 1;
                    }
                    flag = max % MAX_LIST_ROW;

                    if ((float)page > (float)max / MAX_LIST_ROW && row > flag)
                    {
                        row = 1;
                    }
                    tableCallBack(list, max, row, page);

                    break;
                default:
                    printf("\n");
                    break;
                }
            }

            if (ch == ENTER_KEY)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return row;
}

// [D]
List displayUserList(int choice, int page)
{
    if (_CREAR_LIST)
        system("cls");

    float listSize = getListSize(USER_LIST);
    float rowPerPage = ceil(listSize / MAX_LIST_ROW);
    int start = MAX_LIST_ROW * (page - 1) + 1;
    int stop = page * MAX_LIST_ROW;

    printf("page %d\n", page);

    printf("=================== Linked List ===================\n");

    if (start == 0)
    {
        start = 1;
        stop = MAX_LIST_ROW;
    }

    int i = 1; // run number
    int c = 1; // run line that can show per page
    int countRows = 0;

    printf("choice  > %d \n", choice);
    printf("start   > %d \n", start);
    printf("stop    > %d \n", stop);

    UserNode *current = NULL, *detail = NULL;
    List userList;
    if (USER_LIST == NULL)
    { // show list
        printf(" List is Empty");
        /* printf("\npress any key to continue...\n"); */
    }
    else
    {
        current = USER_LIST;
        if (_SHOW_LIST_LINE)
            printf("|%-5s", "Line");
        printf("|%-5s", "_ID");
        printf("|%-15s", "ID");
        printf("|%-13s", "AccountID");
        printf("|%-15s", "Fname");
        printf("|%-15s", "Lname\n");
        printf("\n");

        while (current != NULL && i <= stop)
        {

            if (choice == i)
            {

                printf("\033[1;32m\033[4m");
                detail = current;
                userList.currentRow = i;
                userList.currentID = current->data._id;
            }

            if (c >= start)
            {
                /*  printf("c %-5d|", c); */
                if (_SHOW_LIST_LINE)
                    printf("|%-5d", i);
                printf("|%-5d", current->data._id);
                printf("|%-15s", current->data.id);
                printf("|%-13s", current->data.accountID);
                printf("|%-15s", current->data.fname);
                printf("|%-15s\n", current->data.lname);
                /* printf(" %-2d |\n", current->data.age); */
                countRows++;
            }

            if (choice == i)
                printf("\033[0m");

            current = current->next;

            i++;
            c++;

            if (c == stop + 1)
            {
                break;
            }
        }

        printf("========================\n");
        int maxRow;
        if (start <= c - 1)
        {
            maxRow = c - 1;
        }
        else if (start > c - 1)
        {
            maxRow = start + MAX_LIST_ROW;
        }

        printf("Show %d row(s) (%d-%d)  from %d/%.0f row(s) \n", countRows, start, maxRow, i - 1, listSize);

        int max = i - 1;

        printf("========================\n");
        int p = 1;

        int l = 1;
        int pageListPerRow = 10;

        for (p = 1; p <= rowPerPage; p)
        {
            for (l = 1; l <= pageListPerRow && p <= rowPerPage; l++)
            {
                if (page == p)
                    printf("\033[1;32m");
                printf("|%-3d ", p);
                if (page == p)
                    printf("\033[0m");
                p++;
            }
            printf("\n");
        }

        if (detail)
        {
            printf("\n============== User Detail ==============\n");

            printf(" #              : %d \n", detail->data._id);
            printf(" ID             : %s \n", detail->data.id);
            printf(" fname          : %s \n", detail->data.fname);
            printf(" lname          : %s \n", detail->data.lname);
            printf(" tel            : %s \n", detail->data.tel);
            printf(" accountID      : %s \n", detail->data.accountID);
            printf(" balance        : \033[38;5;48m%.2lf $ \033[0m \n", detail->data.balance);
            printf("\n================= Card Status =============\n");
            printf(" active         : %s \n", detail->data.active == 1 ? "\033[0;32mCard is Activated\033[0m" : "\033[0;31mCard is Suspended\033[0m");
            printf(" status         : %s \n", detail->data.status == 1 ? "\033[0;32mCard is Enabled\033[0m" : "\033[0;31mCard is Disabled\033[0m");
            printf(" registerDate   : %s \n", detail->data.registerTime);
            printf("=====================================\n");
        }
        else
        {
            userList.currentRow = MAX_LINE_SIZE * (page - 1) + 1;
        }

        userList.numRows = i - 1;
        userList.userData = detail;

        return userList;
    }
}

// [C]
int selectUserList(int min, int max, selectedList tableCallBack)
{
    char ch;
    int i = 0;
    int row = 1;
    int page = 1;
    int currnentID;
    float numRows = 0;
    List rowDetail;
    rowDetail = tableCallBack(row, page);
    currnentID = rowDetail.currentID;
    row = rowDetail.currentRow;
    numRows = rowDetail.numRows;

    while (1)
    {
        // Arrow keys are typically represented by two characters
        ch = getch(); // Get the second character
        if (ch > 0)
        {

            // printf("%d \n", ch);

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
                case UP_KEY: // Up arrow key

                    if (row == MAX_LIST_ROW * (page - 1) + 1)
                    {
                        if (numRows == 0)
                        {

                            row = MAX_LIST_ROW * (page);
                        }
                        else
                        {
                            row = numRows;
                        }
                    }

                    else if (row > MAX_LIST_ROW * (page - 1) + 1)
                    {
                        row -= 1;
                    }
                    else
                    {
                        row = MAX_LIST_ROW * (page);
                    }

                    checkFileChangeOnce(USERS_DATA);
                    rowDetail = tableCallBack(row, page);
                    currnentID = rowDetail.currentID;
                    row = rowDetail.currentRow;
                    numRows = rowDetail.numRows;

                    break;

                case LEFT_KEY: // Up left key
                {
                    checkFileChangeOnce(USERS_DATA);

                    float rowPerPage = ceil(numRows / MAX_LIST_ROW);
                    page -= 1;

                    if (page <= 0)
                        page = 1;

                    float flag = row % MAX_LIST_ROW;
                    if (flag == 0)
                        flag = MAX_LIST_ROW;

                    row = MAX_LIST_ROW * (page - 1) + flag;

                    rowDetail = tableCallBack(row, page);
                    numRows = rowDetail.numRows;
                }
                break;
                case RIGHT_KEY: // Up right key
                {
                    checkFileChangeOnce(USERS_DATA);

                    float flag = row % MAX_LIST_ROW;
                    page += 1;

                    if (flag == 0)
                        flag = MAX_LIST_ROW;

                    row = MAX_LIST_ROW * (page - 1) + flag;

                    rowDetail = tableCallBack(row, page);
                    numRows = rowDetail.numRows;

                    if (row > numRows)
                    {
                        row = numRows;
                        rowDetail = tableCallBack(row, page);
                        numRows = rowDetail.numRows;
                    }
                }
                break;
                case DOWN_KEY: // Down arrow key

                    if (numRows == 0 && row == MAX_LIST_ROW)
                    {
                        row = MAX_LIST_ROW * (page - 1) + 1;
                    }
                    else if (numRows > 0 && row == numRows)
                    {
                        printf("");
                        row = MAX_LIST_ROW * (page - 1) + 1;
                    }
                    else if (row < MAX_LIST_ROW * (page))
                    {
                        row += 1;
                    }

                    checkFileChangeOnce(USERS_DATA);

                    rowDetail = tableCallBack(row, page);
                    currnentID = rowDetail.currentID;
                    row = rowDetail.currentRow;
                    numRows = rowDetail.numRows;

                    printf("%d \n", row);
                    printf("%.2f \n", numRows);

                    break;

                case ENTER_KEY:
                {

                    if (rowDetail.userData)
                    {
                        USER_ACTION_MENU[1] = rowDetail.userData->data.active == 0 ? "Active Card" : "Suspend Card";
                        USER_ACTION_MENU[2] = rowDetail.userData->data.status == 0 ? "Enable Card" : "Disabled Card";

                        /* checkFileChangeOnce(USERS_DATA); */
                        int exitUserMenu = selectUserMenu(0, USER_ACTION_MENU, displayUserMenu, "User Action", USER_SESSION.User);

                        if (exitUserMenu == 0)
                        {
                            rowDetail = tableCallBack(row, page);
                        }
                    }

                    break;
                }
                case DELETE_KEY: // Delete
                {

                    char ch2;
                    printf("\033[0;31m Do you want to delete this data ? \033[0m\n");
                    ch2 = getch();

                    if (ch2 == ENTER_KEY)
                    {
                        USER_LIST = deleteUser(currnentID);
                        saveLinkedListToCSV(USERS_DATA, USER_LIST);
                        int listSize = getListSize(USER_LIST);
                        int arrSize;

                        USER_ARR = linkedListToArray(USER_LIST, listSize, &arrSize);

                        /* selectUserArray(1,arrSize,USER_ARR,displayUserArray); */

                        if (row == max)
                        {
                            row = getListSize(USER_LIST);
                        }

                        max -= 1;

                        getch();
                        rowDetail = tableCallBack(row, page);
                        currnentID = rowDetail.currentID;
                        row = rowDetail.currentRow;
                        numRows = rowDetail.numRows;
                    }
                    else
                    {
                        rowDetail = tableCallBack(row, page);

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

            if (ch == EXIT_KEY)
            { // Check for Escape key (optional)
                printf("Escape key pressed\n");
                printf("EXIT\n");

                break;
            }
        }

        /* printf("Select : %d \n",num); */
    }

    return row;
}

/* Utilities Function */

char *getTel(int pass_len)
{
    const int PASS_LEN = pass_len;
    char *tel = (char *)malloc((PASS_LEN + 1) * sizeof(char)); // +1 for null terminator
    char ch;
    int i = 0;

    while (i <= PASS_LEN)
    {
        ch = getch();

        if (ch == BACKSPACE_KEY)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else if (ch >= '0' && ch <= '9' && i < PASS_LEN)
        { // Only accept numeric characters and up to 6 digits
            tel[i] = ch;
            printf("%c", tel[i]);
            i++;
        }

        if (ch == ENTER_KEY && i < PASS_LEN)
        {
            tel[0] = '\0';
            break;
        }

        if (i == PASS_LEN && ch == ENTER_KEY)
        {
            tel[i] = '\0';
            SearchData checkTel = searchTel(tel);
            printf("\n");

            if (checkTel.result == 0)
            {
                printf("\033[1;32m");
                printf("You can use this number [/]");
                printf("\033[0m");

                break;
            }
            else
            {
                printf("\033[1;31m");
                printf("This number (%s) is registed by other account\n", tel);
                printf("\033[0m");

                printf("Try Again\n");
                printf("[66+] > ");

                i = 0;
            }
        }
    }

    printf("\n");
    return tel;
}

User searchUser(User *userArray, int id)
{

    return userArray[id - 1];
};

UserNode *SearchUserList(UserNode *list, int id)
{
    UserNode *current;
    current = list;
    if (!current)
    {
        return current;
    }
    while (current)
    {
        if (current->data._id == id)
        {
            printf("user id     : %s \n", current->data.id);
            printf("user fname  : %s \n", current->data.fname);
            printf("user lname  : %s \n", current->data.lname);

            return current;
        }
        current = current->next;
    }

    return current;
};

UserNode *deleteUser(int id)
{

    UserNode *head = USER_LIST, *current = USER_LIST, *prev = NULL, *del = NULL;

    // Handle the case where the list is empty
    if (!current)
    {
        return USER_LIST;
    }

    // Check if the first node should be deleted
    if (current != NULL && current->data._id == id)
    {
        del = current;
        USER_LIST = current->next;
        free(current);

        if (del)
        {
            printf("\033[1;32m");
            printf("\n================ Deleted User ===============\n");
            printf("#       : %d \n", del->data._id);
            printf("ID      : %s \n", del->data.id);
            printf("fname   : %s \n", del->data.fname);
            printf("lname   : %s \n", del->data.lname);
            printf("================ ============= ===============\n");
            printf("\033[0m");

            printf("\033[1;32m");
            printf("> Delete Success\n");
            printf("\033[0m");
            printf("\033[38;5;50m( Any key to continue ) \033[0m \n");
        }

        return USER_LIST; // Return the new head of the list
    }

    // Search for the node with the given ID
    while (current != NULL && current->data._id != id)
    {
        del = current;
        prev = current;
        current = current->next;
    }

    // If the node with the given ID is found
    if (current != NULL)
    {

        del = current;
        prev->next = current->next;

        // Free the memory of the deleted node
    }
    else
    {
        // Handle the case where the node with the given ID was not found
        printf("User with ID %d not found in the list.\n", id);
    }

    // Print information about the deleted user (optional)

    if (del)
    {
        printf("\n================ Delete User ===============\n");
        printf("\033[1;32m");
        printf("#       : %d \n", del->data._id);
        printf("ID      : %s \n", del->data.id);
        printf("fname   : %s \n", del->data.fname);
        printf("lname   : %s \n", del->data.lname);
        printf("\033[0m");
        printf("================ ============= ===============\n");

        printf("\033[1;32m");
        printf("> Delete Success\n");
        printf("\033[0m");
        printf("\033[38;5;50m( Any key to continue ) \033[0m \n");
        free(del);
    }

    return USER_LIST; // Return the head of the modified list
}

int getRowsByColumn(int index)
{
    // Generate an account ID based on max+1 of number of Users.csv Data
    FILE *file = fopen(USERS_DATA, "r"); // Replace "data.csv" with your CSV file name
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int columnToRead = index; // Specify the column index you want to read (0-based index)
    char line[MAX_LINE_SIZE];
    int skip = 0;
    int length = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int columnIndex = 0;

        while (token != NULL && skip > 0)
        {

            if (columnIndex == columnToRead)
            {
                printf("# %d: %s\n", columnIndex, token);
                break;
            }

            token = strtok(NULL, ",");
            columnIndex++;
            length++;
        }
        skip++;
    }

    fclose(file);
    return length;
}

int getMaxAccountID()
{
    // Generate an account ID based on max+1 of number of Users.csv Data
    FILE *file = fopen(USERS_DATA, "r"); // Replace "data.csv" with your CSV file name
    if (file == NULL)
    {
        perror("Error opening file");
        return 0;
    }

    int columnToRead = 1; // Specify the column index you want to read (0-based index)
    char line[MAX_LINE_SIZE];
    int skip = 0;
    int length = 0;
    int max = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int columnIndex = 0;

        while (token != NULL && skip > 0)
        {

            if (columnIndex == columnToRead)
            {
                if (atoi(token) > max)
                {
                    max = atoi(token);
                }
                break;
            }

            token = strtok(NULL, ",");
            columnIndex++;
            length++;
        }
        skip++;
    }

    fclose(file);
    return max;
}

void getNewAccountID(User *user)
{
    char maxStrLen[MAX_ACCCOUNT_ID_SIZE];

    int maxID = getMaxAccountID() + 1;

    itoa(maxID, maxStrLen, 10);
    printf("Max AccountID : %s \n", maxStrLen);

    int maxIdLength = strlen(maxStrLen);

    char result[MAX_ACCCOUNT_ID_SIZE] = "";
    int l = 0;

    while (l < MAX_ACCCOUNT_ID_SIZE - maxIdLength)
    {
        strcat(result, "0");
        l++;
    }

    strcat(result, maxStrLen);

    strcpy(user->accountID, result);
}

User Register(const char id[], const char fname[], const char lname[])
{
    User newUser;

    strncpy(newUser.id, id, sizeof(newUser.id) - 1);
    newUser.id[sizeof(newUser.id) - 1] = '\0';

    strncpy(newUser.fname, fname, sizeof(newUser.fname) - 1);
    newUser.fname[sizeof(newUser.fname) - 1] = '\0';

    strncpy(newUser.lname, lname, sizeof(newUser.lname) - 1);
    newUser.lname[sizeof(newUser.lname) - 1] = '\0';

    printf("Enter your number \n");
    printf("[66+] > ");

    strncpy(newUser.tel, getTel(10), sizeof(newUser.tel) - 1);
    newUser.tel[sizeof(newUser.tel) - 1] = '\0';

    strncpy(newUser.registerTime, getCurrentTime(), sizeof(newUser.registerTime) - 1);
    newUser.registerTime[sizeof(newUser.registerTime) - 1] = '\0';

    getNewAccountID(&newUser);
    newUser.active = 1;
    newUser.status = 1;
    newUser.balance = 0.00;
    printf("Create 6 digit password \n");
    strncpy(newUser.password, createPassword(), sizeof(newUser.password));
    newUser.password[sizeof(newUser.password)] = '\0';

    printf("================ New User ============\n");
    printf("ID                  : %s \n", newUser.id);
    printf("Full Name           : %s %s\n", newUser.fname, newUser.lname);
    printf("Tel                 : %s \n", newUser.tel);
    printf("-------------------------------------\n");
    printf("AccountID           : %s \n", newUser.accountID);
    printf("Balance             :\033[38;5;48m %.2lf \033[0m \n", newUser.balance);
    printf("Register Time       : %s \n", newUser.registerTime);
    printf("Password            : %s \n", newUser.password);
    printf("======================================\n");

    if (isfileExists(USERS_DATA))
    {
        printf(_ADMIN_DEBUG ? "file is exist : Appended\n" : "");
        appendToCSV(USERS_DATA, &newUser);
    }
    else
    {
        printf(_ADMIN_DEBUG ? "file is not exist : Create\n" : "");
        saveUser(USERS_DATA, &newUser);
    }


    return newUser;
}

/* FILES Utilities Function */

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

    for (int i = 0; i < 13; i++)
    {
        u->accountID[i] = '0' + rand() % 10;
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
    /*   u->age = 18 + rand() % 43; */
}

char *createPassword()
{

    while (1)
    {
        char *password, *confirmpassword;

        printf("\nCreate Password: \n");
        password = getPassword(6);
        printf("\nConfirm Password : \n");
        confirmpassword = getPassword(6);

        if (strcmp(password, confirmpassword) == 0)
        {
            printf("%s\n", "\033[1;32mYour passwords is match.\033[0m");
            return password;
        }
        else
        {
            system("cls");
            printf("%s\n", "\033[1;31mYour passwords aren't match!\033[0m");
        }
    }
}

/*
int main(int argc, char const *argv[])
{
    printf("\033[?25l"); // hide cursor

    if (_APP_START_CLEAR)
    {
        system("cls");
    }

    struct tm *localTime;
    time(&lastestTime);
    localTime = localtime(&lastestTime);

    printf("Program started at: %s", asctime(localTime));

    Table userData = processCSVToLinkedList(USERS_DATA, 1);

    User registeredUser = Register("1909300007092", "Captain", "Siwakron");
    getch();

    userData = processCSVToLinkedList(USERS_DATA, 1);
    selectUserList(1, USER_ARR_SIZE, displayUserList);

    getch();
    return 0;
} */