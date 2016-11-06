#include <stdio.h>
#include <diff.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

void print_help()
{
    wprintf(L"Usage: diff.exe <filename1> <filename2>");
}

int args_parse(int argc, char* argv[], char** file1_path, char** file2_path)
{
    if (argc < 3)
    {
        print_help();
        return 0;
    }
    *file1_path = argv[1];
    *file2_path = argv[2];
    return 1;
}

void file_exception(char *file)
{
    fprintf(stderr, "Can't open %s: ", file);
    perror("");
    exit(1);
}

void print_results_and_free(Results *res)
{
    if (res->res == false)
    {
        wprintf(L"This files are equal!\n");
    } else {
        wprintf(L"Files differs in line %d:\n>%ls\n>%ls\n",
                res->line_number, res->line1, res->line2);
        free(res->line1);
        free(res->line2);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    char *file1_path;
    char *file2_path;
    if (!args_parse(argc, argv, &file1_path, &file2_path))
        return 1;
    FILE *file1 = fopen(file1_path, "rt");
    if (!file1)
        file_exception(file1_path);
    FILE *file2 = fopen(file2_path, "rt");
    if (!file2)
        file_exception(file2_path);

    Results results = {0};
    diff(file1, file2, &results);
    print_results_and_free(&results);
    fclose(file1);
    fclose(file2);
    return 0;
}
