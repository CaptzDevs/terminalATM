#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.c"


char* progress_name[] = {
    "Stage 1",
    "Stage 2",
    "Stage 3"
    };

int progress(char* progressName[], int miles){
    int i = 0;
    int len = sizeof(progress_name)/sizeof(progress_name[0]);
    
    miles -= 1;

    while (i < len)
    {
        if(i <= miles){
            printf("\033[1;32m");
        }

        printf("%s ",progressName[i]);

        if(i <= miles){
            printf("[/] ");
            printf("\033[0m");
        }
        printf("\n");

        i++;
    }
}

