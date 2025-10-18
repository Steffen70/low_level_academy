#pragma once
#include <stdio.h>

typedef struct
{
    bool wasSuccessful;
    FILE* filePtr;
    char* exceptionMessage;
} DbFileResponse;

void destroy_db_file_response(const DbFileResponse* responsePtr);

DbFileResponse create_db_file(const char* filename);
DbFileResponse open_db_file(const char* filename);