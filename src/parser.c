#include "shell.h"

Pipeline *parse(Token *tokens, int n) {

    Pipeline *p = calloc(1, sizeof(Pipeline));
    if (!p) {
        die("calloc pipeline");
    }

    int t = 0; //current position in token array
    int c = 0; //current ommand index in pipeline

    // Initializing the first commnad
    p->cmds[c].argc = 0;
    p->cmds[c].input_file = NULL;
    p->cmds[c].output_file = NULL;
    p->cmds[c].append = 0;

    while (t < n && tokens[t].type != TOKEN_EOF) {
        
        switch (tokens[t].type) {

            case TOKEN_WORD:
            //adding word to the next argument in the current command
            // argv must be null terminated for execvp
            if(p->cmds[c].argc >= MAX_ARGS - 1) {
                fprintf(stderr, "SHELL: too many arguments\n");
                p->n = c + 1; 
                free_pipeline(p);
                return NULL;
            }
            p->cmds[c].argv[p->cmds[c].argc++] = safe_strdup(tokens[t].value);
            p->cmds[c].argv[p->cmds[c].argc] = NULL;
            t++;
            break;

            case TOKEN_PIPE:
            // pipe token - finish current command and start a new one
            if (p->cmds[c].argc == 0) {
                fprintf(stderr, "SHELL: syntax error near '|'\n");
                p->n = c; 
                free_pipeline(p);
                return NULL;
            }
            c++;
            if (c >= MAX_PIPE_CMDS) {
                fprintf(stderr, "SHELL: too many pipe commands\n");
                p->n = c; 
                free_pipeline(p);
                return NULL;
            }

            p->cmds[c].argc        = 0;
            p->cmds[c].input_file  = NULL;
            p->cmds[c].output_file = NULL;
            p->cmds[c].append      = 0;
            t++;
            break;

            case TOKEN_REDIR_IN:
            // < must be followed by a filename
            t++;
            if (t >= n || tokens[t].type != TOKEN_WORD) {
                fprintf(stderr, "SHELL: expected file name after '<'\n");
                p->n = c + 1; 
                free_pipeline(p);
                return NULL;
            }
            p->cmds[c].input_file = safe_strdup(tokens[t].value);
            t++;
            break;

            case TOKEN_REDIR_OUT:
            t++;
            if (t >= n || tokens[t].type != TOKEN_WORD) {
                fprintf(stderr, "SHELL: expected file name after '>'\n");
                p->n = c + 1; 
                free_pipeline(p);
                return NULL;
            }
            p->cmds[c].output_file = safe_strdup(tokens[t].value);
            p->cmds[c].append = 0;
            t++;
            break;

            case TOKEN_REDIR_APPEND:
            t++;
            if (t >= n || tokens[t].type != TOKEN_WORD) {
                fprintf(stderr, "SHELL: expected file name after '>>'\n");
                p->n = c + 1;
                free_pipeline(p);
                return NULL;
            }
            p->cmds[c].output_file = safe_strdup(tokens[t].value);
            p->cmds[c].append = 1;
            t++;
            break;
            
            case TOKEN_EOF:
            t++;
            break;

            default:
            t++;
            break;
        }
    }

    // trailing pipe: "ls |" — last command has no words
    if (p->cmds[c].argc == 0 && c > 0) {
        fprintf(stderr, "SHELL: syntax error: incomplete pipeline\n");
        p->n = c;
        free_pipeline(p);
        return NULL;
    }

    // empty input
    if (c == 0 && p->cmds[0].argc == 0) {
        free(p);
        return NULL;
    }

    p->n = c + 1;
    return p;
}

void free_pipeline(Pipeline *p) { // call after execute is done with the popeline
    if (!p) return;

    for (int i = 0; i < p->n; i++) {
        for (int j = 0; j < p->cmds[i].argc; j++) {
            free(p->cmds[i].argv[j]);
        }
        if (p->cmds[i].input_file) free(p->cmds[i].input_file);
        if (p->cmds[i].output_file) free(p->cmds[i].output_file);
    }
    free(p);
}

void print_pipeline(Pipeline *p) { // callinfg this after parse to verify the structure before executing
    fprintf(stderr, "[parser] pipeline with %d command(s):\n", p->n);
    for (int i = 0; i < p->n; i++) {
        fprintf(stderr, "  cmd[%d]:\n", i);
        fprintf(stderr, "    argv: ");
        for (int j = 0; j < p->cmds[i].argc; j++)
            fprintf(stderr, "\"%s\" ", p->cmds[i].argv[j]);
        fprintf(stderr, "\n");
        if (p->cmds[i].input_file)
            fprintf(stderr, "    input_file:  %s\n", p->cmds[i].input_file);
        if (p->cmds[i].output_file)
            fprintf(stderr, "    output_file: %s (append=%d)\n",
                    p->cmds[i].output_file, p->cmds[i].append);
    }
}