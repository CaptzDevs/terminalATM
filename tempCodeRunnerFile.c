   // Read the first line to get field names
    if (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        while (token && fieldCount < MAX_FIELDS) {
            // Store the field name in the array
            fieldNames[fieldCount] = strdup(token);

            // Remove trailing newline character, if present
            size_t len = strlen(fieldNames[fieldCount]);
            if (fieldNames[fieldCount][len - 1] == '\n') {
                fieldNames[fieldCount][len - 1] = '\0';
            }

            // Get the next token
            token = strtok(NULL, ",");
            fieldCount++;
        }
    }