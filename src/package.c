#include <stdio.h>
#include <stdlib.h>
#include "package.h"

package_t* init_package()
{
    package_t* package = calloc(1, sizeof(package_t));

    package->base = (void*) 0;
    package->install_code = (void*) 0;
    package->name = (void*) 0;
    package->url = (void*) 0;
    package->version = (void*) 0;
    package->make_deps_size = 0;

    return package;
}