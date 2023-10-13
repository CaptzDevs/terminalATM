typedef struct Card
{
    int status;
    User *data;

} Card;

Card readCard(const char *filename)
{
    FILE *file = fopen(filename, "r");
    Card CardDetail;
    CardDetail.data = NULL;
    CardDetail.status = 0;

    if (!file)
    {
        perror("Error opening file");
        return CardDetail;
    }

    char line[MAX_LINE_SIZE];
    char *fieldNames[MAX_FIELDS];
    int fieldCount = 0;
    User userData;

    // Read the first line to get field names
    if (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        while (token && fieldCount < MAX_FIELDS)
        {
            fieldNames[fieldCount] = strdup(token);

            if (fieldCount == 0)
            {
                copyTo(userData.id, token, sizeof(userData.id));
            }
            else if (fieldCount == 1)
            {
                copyTo(userData.fname, token, sizeof(userData.fname));
            }
            else if (fieldCount == 2)
            {
                copyTo(userData.lname, token, sizeof(userData.lname));
            }

            token = strtok(NULL, ",");
            fieldCount++;
        }
    }

    fclose(file);

    printf("\nRead Card [/]\n");
    CardDetail.status = 1;
    CardDetail.data = malloc(sizeof(User));
    if (CardDetail.data != NULL)
    {
        memcpy(CardDetail.data, &userData, sizeof(User));
    }
    else
    {
        perror("Error allocating memory for User data");
        CardDetail.status = 0;
    }

    return CardDetail;
}

Card checkInsertCard()
{

    char *fileName = "./card/SIAM-ID/Data.txt";
    Card userCardData;
    userCardData.status = 0;

    if (isfileExists(fileName) == 1)
    {

        userCardData = readCard(fileName);


        if (userCardData.status)
        {

            SearchData foundUser = searchID(userCardData.data->id);

            if (foundUser.result == 1)
            {
                userCardData.data = foundUser.user;
            }
            else
            {
                //for new User 
                userCardData.status = 2;
            }

            remove(fileName);
            return userCardData;
        }
    }
}
