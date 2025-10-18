#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <low_level_academy/file.h>

static void print_help(char* argv[])
{
    printf("Usage:\t%s <database_file> [options]...\n", argv[0]);
    printf("Options:\n");

    const char* format = "\t%s%s\n";
    printf(format, "--help\t\t", "show help");
    printf(format, "--version\t", "show version");
    printf(format, "--new\t\t", "create new db file");
    printf(format, "--file\t\t", "open existing db file");
}

static void print_create_db_file(char* filename)
{
    printf("Creating new db file: '%s'...\n", filename);
    const auto response = create_db_file(filename);
    if (!response.wasSuccessful)
    {
        printf("%s\n", response.exceptionMessage);
        destroy_db_file_response(&response);
        return;
    }
    // TODO: Return mainCtx - remove destroy_db_file_response call
    destroy_db_file_response(&response);
}

static void print_open_db_file(char* filename)
{
    printf("Using existing db file: '%s'...\n", filename);
    const auto response = open_db_file(filename);
    if (!response.wasSuccessful)
    {
        printf("%s\n", response.exceptionMessage);
        destroy_db_file_response(&response);
        return;
    }
    destroy_db_file_response(&response);
}

typedef void (*action)(void* actionCtxPtr);

typedef struct ActionListAction ActionListAction;
struct ActionListAction{
    ActionListAction* nextActionPtr;
    action actionPtr;
    void* actionCtxPtr;
};

static void run_action(ActionListAction* actionListActionPtr)
{
    actionListActionPtr->actionPtr(actionListActionPtr->actionCtxPtr);

    if (actionListActionPtr->nextActionPtr != nullptr)
    {
        run_action(actionListActionPtr->nextActionPtr);
    }
    free(actionListActionPtr);
}

typedef struct option option;

int main(const int argc, char* argv[])
{
    int longOptIndex = 0;

    static option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"version", no_argument, nullptr, 'v'},
        {"new", no_argument, nullptr, 'n'},
        {"file", no_argument, nullptr, 'f'},
    };

    ActionListAction noAction;
    ActionListAction* lastActionPtr = &noAction;

    int opt;
    if ((opt = getopt_long(argc, argv, "hvnf", longOptions, &longOptIndex)) != -1)
    {
        do
        {
            // ReSharper disable once CppDFAMemoryLeak
            ActionListAction* currentActionPtr = malloc(sizeof(ActionListAction));
            lastActionPtr->nextActionPtr = currentActionPtr;
            switch (opt)
            {
            case 'h':
                currentActionPtr->actionPtr = (action)&print_help;
                currentActionPtr->actionCtxPtr = argv;
                break;
            case 'v':
                currentActionPtr->actionPtr = (action)&printf;
                currentActionPtr->actionCtxPtr = &"1.0.0.0\n";
                break;
            case 'n':
                currentActionPtr->actionPtr = (action)&print_create_db_file;
                currentActionPtr->actionCtxPtr = argv[1];
                break;
            case 'f':
                currentActionPtr->actionPtr = (action)&print_open_db_file;
                currentActionPtr->actionCtxPtr = argv[1];
                break;
            default:
            case '?':
                return 1;
            }
            lastActionPtr = currentActionPtr;
        }
        while ((opt = getopt_long(argc, argv, "hvnf", longOptions, &longOptIndex)) != -1);
    }
    else
    {
        ActionListAction* currentActionPtr = malloc(sizeof(ActionListAction));
        noAction.nextActionPtr = currentActionPtr;
        currentActionPtr->actionPtr = (action)&print_open_db_file;
        currentActionPtr->actionCtxPtr = argv[optind];
    }

    run_action(noAction.nextActionPtr);

    return 0;
}
