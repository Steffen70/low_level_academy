#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <low_level_academy/file.h>

static DbFileResponse open(const char* filename, const char* openMode)
{
    FILE* fPtr = fopen(filename, openMode);
    if (fPtr == nullptr)
    {
        char* errorMessage = strdup(strerror(errno));
        return (DbFileResponse){false, nullptr, errorMessage};
    }
    return (DbFileResponse){true, fPtr, nullptr};
}

DbFileResponse create_db_file(const char* filename)
{
    return open(filename, "w+");
}

DbFileResponse open_db_file(const char* filename)
{
    return open(filename, "r+");
}

void destroy_db_file_response(const DbFileResponse* responsePtr)
{
    if (responsePtr->wasSuccessful)
    {
        fclose(responsePtr->filePtr);
        return;
    }
    free(responsePtr->exceptionMessage);
}
