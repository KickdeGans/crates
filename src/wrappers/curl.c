#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl.h"

char* curl(char* url)
{
    char command[sizeof(url) + 75];

    sprintf(command, "curl -s %s > /tmp/curl_output", url);

    system(command);

    FILE* file = fopen("/tmp/curl_output", "rb");

    char* buffer = 0;
    size_t length;

    if (file)
    {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);

        buffer = calloc(length, length);

        if (buffer)
            fread(buffer, 1, length, file);

        fclose(file);

        return buffer;
    }

    return "404 Not found";
}

int is_404(char* txt)
{
    if (strstr(txt, "404") != NULL)
        return 1;

    return 0;
}