#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void print_the_flag()
{
    FILE* fPtr = fopen("/tmp/flag", "r");
    char* fileContentBufPtr = nullptr;

    if (fPtr == nullptr)
    {
        perror("fopen");
        return;
    }

    if (fseek(fPtr, 0, SEEK_END) != 0)
    {
        perror("fseek");
        goto cleanup;
    }

    const long fileSize = ftell(fPtr);
    if (fileSize < 0)
    {
        perror("ftell");
        goto cleanup;
    }

    fileContentBufPtr = malloc(fileSize + 1);

    rewind(fPtr);
    if (fread(fileContentBufPtr, 1, fileSize, fPtr) != fileSize)
    {
        perror("fread");
        goto cleanup;
    }

    fileContentBufPtr[fileSize] = '\0';

    printf("%s\n", fileContentBufPtr);

cleanup:
    fclose(fPtr);
    if (fileContentBufPtr != nullptr)
    {
        free(fileContentBufPtr);
    }
}

int main(int argc, char** argv)
{
    print_the_flag();
    return 0;
}
