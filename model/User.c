typedef struct User
{
    int _id; // for index
    char id[15];
    char accountID[15];
    char fname[250];
    char lname[250];
    char tel[11];
    double balance;
    int active;
    int status;
    char registerTime[50];
    char password[7];

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
    int size;
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

    UserNode *userData;
    User *array;

} List;

