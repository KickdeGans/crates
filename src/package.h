#ifndef PACKAGE_H
#define PACKAGE_H

typedef struct PACKAGE_STRUCT
{
    char* name;
    char* url;
    char* base;
    char* version;
    char* install_code;
    char* make_deps[75];
    int make_deps_size;
} package_t;

package_t* init_package();

#endif