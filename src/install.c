#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "package.h"
#include "wrappers/curl.h"
#include "parser/parser.h"
#include "parser/lexer.h"

void install_pkg(package_t* package, int verbose_mode)
{
    if (getuid())
    {
        printf("FATAL: You must be a superuser to install a package.\n");
        exit(1);
    }

    if (verbose_mode)
        printf("Downloading crate file from '%s'...\n", package->url);

    char* crate = curl(package->url);

    if (is_404(crate))
    {
        printf("FATAL: Package '%s' not found\n.", package->name);
        exit(1);
    }

    if (verbose_mode)
        printf("Package instruction contents:\n%s\n", crate);
    
    if (verbose_mode)
        printf("Parsing crate...\n\n");

    parser_t* parser = init_parser(init_package(), init_lexer(crate));
    
    if (verbose_mode)
        printf("Package initiated\n");

    if (verbose_mode)
        printf("Size of package is %lu\n", sizeof(parser->package));

    if (verbose_mode)
        printf("Begin parsing.\n");

    parser_parse(parser);

    if (verbose_mode)
        printf("Done parsing.\n");

    printf(":: Package to install:          %s-%s\n", parser->package->name, parser->package->version);
    printf(":: Source code is located at:   %s (git repository)\n\n", parser->package->base);

    printf(":: Continue with installation? (y/n): ");

    char choice = getchar();

    if (choice == 'y' || choice == 'Y')
    {
        time_t start_time = time(NULL);

        printf("\n[1/4] Checking dependecies...\n");

        for (int i = 0; i < parser->package->make_deps_size; i++)
        {
            char exec[sizeof(parser->package->make_deps[i]) + 10];

            sprintf(exec, "/bin/%s", parser->package->make_deps[i]);

            FILE* file = fopen(exec, "r");

            if (verbose_mode)
                printf("Opened file %s\n", exec);

            if (file == NULL)
            {
                printf("FATAL: Package '%s' does not appear to be installed!\n", parser->package->make_deps[i]);
                exit(1);
            }

            printf("[✓] %s\n", parser->package->make_deps[i]);
        }

        char* git_clone = malloc(sizeof(parser->package->base) + 20);

        sprintf(git_clone,"git clone %s >/dev/null 2>&1 ", parser->package->base);
        
        printf("\n[2/4] Cloning repository...\n");

        if (verbose_mode)
            printf("Executing command 'git clone %s'\n", parser->package->base);

        system(git_clone);

        printf("[3/4] Running install script...\n");

        char* install_code = malloc(sizeof(parser->package->install_code) + 125);

        sprintf(install_code,"cd %s\n(%s) >/dev/null 2>&1 ", parser->package->name, parser->package->install_code);
        
        if (verbose_mode)
            printf("Install script = %s", parser->package->install_code);

        system(install_code);

        printf("[4/4] Cleaning up...\n");

        char* cleanup = malloc(200);

        sprintf(cleanup,"rm -rf ./%s", parser->package->name);

        if (verbose_mode)
            printf("Code to clean up = %s", cleanup);

        system(cleanup);

        time_t end_time = time(NULL);

        printf("\n:: Time elapsed: %li seconds.\n", end_time - start_time);

        printf(":: All done, exiting now...\n");
        exit(0);
    }
    else 
    {
        printf("Operation canceled by the user.\n");
        exit(0);
    }
}