#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "../package.h"

parser_t* init_parser(package_t* package, lexer_T* lexer)
{
    parser_t* parser = calloc(1, sizeof(parser_t));

    parser->lexer = lexer;
    parser->package = package;
    parser->current_token = lexer_get_next_token(parser->lexer);

    return parser;
}

parser_t* parser_eat(parser_t* parser, int token)
{
    if (parser->current_token->type == token)
    {
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf("Unexpected token '%s'\n", parser->current_token->value);
        exit(1);
    }

    return (void*) 0;
}

parser_t* parser_parse(parser_t* parser)
{
    while (parser->current_token->type != TOKEN_EOF)
    {
        if (parser->current_token->type == TOKEN_EOF)
            return parser;

        if (strcmp(parser->current_token->value, "pkgName") == 0)
        {
            parser_eat(parser, TOKEN_ID);
            parser_eat(parser, TOKEN_EQUALS);

            parser->package->name = parser->current_token->value;

            parser_eat(parser, TOKEN_STRING);

            continue;
        }
        if (strcmp(parser->current_token->value, "pkgVersion") == 0)
        {
            parser_eat(parser, TOKEN_ID);
            parser_eat(parser, TOKEN_EQUALS);

            parser->package->version = parser->current_token->value;

            parser_eat(parser, TOKEN_STRING);

            continue;
        }
        if (strcmp(parser->current_token->value, "pkgUrl") == 0)
        {
            parser_eat(parser, TOKEN_ID);
            parser_eat(parser, TOKEN_EQUALS);

            parser->package->base = parser->current_token->value;

            parser_eat(parser, TOKEN_STRING);

            continue;
        }
        if (strcmp(parser->current_token->value, "install") == 0)
        {
            parser_eat(parser, TOKEN_ID);
            parser_eat(parser, TOKEN_EQUALS);

            parser->package->install_code = parser->current_token->value;

            parser_eat(parser, TOKEN_STRING);
            
            continue;
        }
        if (strcmp(parser->current_token->value, "deps") == 0)
        {
            parser_eat(parser, TOKEN_ID);
            parser_eat(parser, TOKEN_EQUALS);
            parser_eat(parser, TOKEN_LPAREN);

            int i = 0;
        
            while (parser->current_token->type == TOKEN_STRING)
            {
                parser->package->make_deps[i] = malloc(sizeof(parser->current_token->value));

                parser->package->make_deps[i] = parser->current_token->value;

                parser_eat(parser, TOKEN_STRING);
                i++;
            }

            parser->package->make_deps_size = i;

            parser_eat(parser, TOKEN_RPAREN);

            continue;
        }
    }

    return parser;
}