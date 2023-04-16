#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"
#include "../package.h"

typedef struct PARSER_STRUCT
{
    token_T* current_token;
    lexer_T* lexer;
    package_t* package;
} parser_t;

parser_t* init_parser(package_t* package, lexer_T* lexer);
parser_t* parser_eat(parser_t* parser, int token);
parser_t* parser_parse(parser_t* parser);

#endif