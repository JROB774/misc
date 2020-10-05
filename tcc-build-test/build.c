#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libtcc.h>

int main (int argc, char** argv)
{
    TCCState* tcc;
    int i;

    printf("Compiling program... ");

    tcc = tcc_new();
    if (!tcc)
    {
        printf("\nFailed to create TCC state!\n");
        exit(1);
    }

    for (i=1; i<argc; ++i)
    {
        char* a = argv[i];
        if (a[0] == '-')
        {
            if      (a[1] == 'B') tcc_set_lib_path    (tcc, a+2);
            else if (a[1] == 'I') tcc_add_include_path(tcc, a+2);
            else if (a[1] == 'L') tcc_add_library_path(tcc, a+2);
        }
    }

    // Must be called before any compilation.
    tcc_set_output_type(tcc, TCC_OUTPUT_EXE);

    if (tcc_add_file(tcc, "main.c") == -1)
    {
        printf("\nFailed to compile main!\n");
        exit(1);
    }

    tcc_output_file(tcc, "main.exe");

    tcc_delete(tcc);

    printf("complete!\n");
    return 0;
}
