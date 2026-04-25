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
        
    }


}