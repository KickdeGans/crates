#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package.h"
#include "install.h"

void help()
{
    printf("Crates help menu:\n\n");
    printf("Usage: crates <options> <arguments>");

    printf("install:\nInstall a package.\n    Usage: ... install <package> ...\n\n");
    printf("--verbose-mode:\nVerbose mode for installing a package.\n    Usage: ... --verbose-mode ...\n\n");
    printf("--help:\nOpen help menu.\n");
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        help();
    }

    int verbose_mode = 0;
    int help_menu = 0;
    int install = 0;
    char* pkg = "";

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "install") == 0)
        {
            install = 1;

            i++;

            if (i > argc)
            {
                printf("install: No input given.\n");
                exit(1);
            }

            pkg = argv[i];
        }
        if (strcmp(argv[i], "--verbose-mode") == 0)
        {
            verbose_mode = 1;
        }
        if (strcmp(argv[i], "--help") == 0)
        {
            help_menu = 1;
        }
    }

    if (help_menu)
        help();
    if (install)
    {
        package_t* package = init_package();

        package->name = pkg;
        package->url = malloc(sizeof(package->name) + 35);
            
        sprintf(package->url, "https://www.kickdegans.nl/crates/%s.crate", package->name);

        install_pkg(package, verbose_mode);
    }
    
    return 0;
}