#ifndef SHELL_H
#define SHELL_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 128
#define MAX_ARGS 64
#define MAX_PIPE_CMDS 16

typedef enum {
    TOKEN_WORD, //command ir args
    TOKEN_PIPE, // |
    TOKEN_REDIR_IN, // <
    TOKEN_REDIR_OUT, // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_INPUT];
} Token;

/* command structure */
typedef struct {
    char *argv[MAX_ARGS];  /* null-terminated argument array      */
    int   argc;            /* number of arguments                 */
    char *input_file;      /* NULL = read from stdin / prev pipe  */
    char *output_file;     /* NULL = write to stdout / next pipe  */
    int   append;          /* 1 = O_APPEND, 0 = O_TRUNC          */
} Command;

/* pipeline structure */
typedef struct {
    Command cmds[MAX_PIPE_CMDS];
    int     n;             /* number of commands in pipeline      */
} Pipeline;

/* lexer.c */
int  lexer(const char *input, Token *tokens, int max_tokens);
void print_tokens(Token *tokens, int n);  // for debgging
 
/* parser.c */
Pipeline *parse(Token *tokens, int n);
void      free_pipeline(Pipeline *p);
void      print_pipeline(Pipeline *p);    // for debugging
 
/* executor.c */
void execute(Pipeline *p);
 
/* builtins.c */
int  is_builtin(const char *cmd);
int  execute_builtin(Command *cmd);
 
/* signals.c */
void setup_signals(void);
 
/* utils.c */
void  die(const char *msg);               /* perror + exit(1)     */
char *safe_strdup(const char *s);         /* strdup with error check */
 
#endif /* SHELL_H */