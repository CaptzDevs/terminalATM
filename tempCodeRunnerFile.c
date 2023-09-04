 while (i < strlen(str))
    {
        sumStr[i] = str[i];
        printf("%s\r", sumStr ); // Print the cursor character and move the cursor back
        fflush(stdout); // Flush the output buffer to ensure immediate printing
        nanosleep(&delay, NULL);
        i++;
    }
