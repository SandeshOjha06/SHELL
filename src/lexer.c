#include "shell.h"

int lexer(const char *input, Token *tokens, int max_tokens) {
    int i = 0; // index into input string
    int t = 0;  // token array
    int w = 0; // current word
    char word[MAX_INPUT];

    while (input[i] != '\0' && t < max_tokens - 1 ) {
        if (input[i] == ' ' || input[i] == '\t') {
            if (w > 0) {
                word[w] = '\0';
                tokens[t].type = TOKEN_WORD;
                strncpy(tokens[t].value, word, MAX_INPUT - 1);
                t++;
                w = 0;
            }
            i++ ;
            continue;
        }
        // pipe
        if (input[i] == '|') {
            // finsih any word in progress
            if (w > 0) {
                word[w] = '\0';
                tokens[t].type = TOKEN_WORD;
                strncpy(tokens[t].value, word, MAX_INPUT - 1);
                t++;
                w = 0;
            }

            tokens[t].type = TOKEN_PIPE;
            tokens[t].value[0] = '\0';
            t++;
            i++;
            continue;
        }

        // redirect - > or >>
         if (input[i] == '>') {
            // finsih any word in progress
            if (w > 0) {
                word[w] = '\0';
                tokens[t].type = TOKEN_WORD;
                strncpy(tokens[t].value, word, MAX_INPUT - 1);
                t++;
                w = 0;
            }

            // peek at next character
            if (input[i +1] == '>') {
                tokens[t].type = TOKEN_REDIR_APPEND;
                i += 2;
            } else {
                tokens[t].type = TOKEN_REDIR_OUT;
                i++;
            }
            tokens[t].value[0] = '\0';
            t++;
            continue;
        }

        // redirect <
        if (input[i] == '<') {
            if (w > 0) {
                word[w] = '\0';
                tokens[t].type = TOKEN_WORD;
                strncpy(tokens[t].value, word, MAX_INPUT - 1);
                t++;
                w = 0;
            }
            tokens[t].type = TOKEN_REDIR_IN;
            tokens[t].value[0] = '\0';
            t++;
            i++;
            continue;
        }

        // regular character -> word
        word[w++] = input[i++];

        //finsih remaining word if any
    }
        if (w > 0) {
            word[w] = '\0';
            tokens[t].type = TOKEN_WORD;
            strncpy(tokens[t].value, word, MAX_INPUT - 1);
            t ++;
    }

    // end EOF Token
    tokens[t].type = TOKEN_EOF;
    tokens[t].value[0] = '\0';
    t++;
    
    return t;
}


void print_tokens(Token *tokens, int n) {
    const char *type_names[] = {
        "WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "REDIR_APPEND", "EOF"
    };

    fprintf(stderr, "[lexer] %d tokens:\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(stderr, "  [%d] type=%-14s value=\"%s\"\n",
                i,
                type_names[tokens[i].type],
                tokens[i].value);
    }
}