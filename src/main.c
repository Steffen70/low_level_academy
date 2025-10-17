#include <getopt.h>
#include <stdio.h>

static void print_help(char* argv[])
{
    printf("Usage:\t%s <database_file> [options]...\n", argv[0]);
    printf("Options:\n");

    const char* format = "\t%s\t\t%s\n";
    printf(format, "-h|--help", "show help");
    printf(format, "-v|--version", "show version");
    printf(format, "-n|--new", "create new db file");
    printf(format, "-f|--file", "open existing db file");
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

    int opt;
    if ((opt = getopt_long(argc, argv, "hvnf", longOptions, &longOptIndex)) != -1)
    {
        do
        {
            switch (opt)
            {
            case 'h':
                print_help(argv);
                break;
            case 'v':
                printf("1.0.0.0\n");
                break;
            case 'n':
                printf("Creating new db file: '%s'...\n", argv[1]);
                break;
            case 'f':
                printf("Using existing db file: '%s'...\n", argv[1]);
                break;
            default:
            case '?':
                return 1;
            }
        }
        while ((opt = getopt_long(argc, argv, "hvnf", longOptions, &longOptIndex)) != -1);
    }
    else
    {
        printf("Using existing db file: '%s'...\n", argv[optind]);
    }

    return 0;
}
