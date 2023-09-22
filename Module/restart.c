#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/stat.h>


int isFileChanged(const char *filename, time_t *lastModifiedTime)
{
    struct stat fileStat;

    // Get the file's status (including modification time)
    if (stat(filename, &fileStat) != 0)
    {
        perror("Error getting file status");
        return -1; // Error
    }

    // Check if the modification time has changed
    if (fileStat.st_mtime != *lastModifiedTime)
    {
        *lastModifiedTime = fileStat.st_mtime; // Update last modified time
        return 1;                              // File has changed
    }

    return 0; // File has not changed
}

int checkFileChange(const char *filename, float interval)
{
    time_t lastModifiedTime = 0;
    int i = 0;
    while (1)
    {
        int result = isFileChanged(filename, &lastModifiedTime);

        if (result == -1)
        {
            return 1; // Error occurred
        }
        else if (result == 1 && i != 0)
        {
            printf("File has changed. Last modified time: %s", ctime(&lastModifiedTime));
            printf("Restarting...");
            return 1;
        }
        else
        {
            printf("File has not changed.\n");
        }

        // Sleep for a while before checking again (adjust as needed)
        sleep(interval);
        i = 1;
    }
}

int main() {
    int result;


    if(checkFileChange("account.c",1)){

        // Run a series of commands
        result = system("cd \"c:\\Users\\ASUS\\Desktop\\CPROJECT\\\" && gcc account.c -o account_re && .\\account");

        if (result == 0) {
            printf("Commands executed successfully.\n");
            
        } else {
            printf("Commands failed to execute.\n");
        }

    }

 

    return 0;
}
