#include "shell.h"

// cd, exit, export, unset, pwd, NULL

static const char *BUILTINS = {
    "cd", "exit", "export", "unset", "pwd", "NULL"
};

int is_builtin(const char *cmd) {
    for(int i = 0; BUILTINS[i]!=NULL, i++) {
        if(strcmp(cmd, BUILTINS[i]) == 0) return 1;
    }
    return 0;
}

//individual implementations

static int builtin_cd(Command *cmd) {
    const char *dir;

    if (cmd->argc < 2) { // cd with no args go to home
        dir = getenv("HOME");
        if (dir == NULL) {
            fprintf(stderr, "mysh: cd: HOME not set\n");
            return 1;
        }
    } else {
        dir = cmd->argv[1];
    }

    if (chdir(dir) == -1) {
        fprintf(stderr, "SHELL: cd: %s: %s\n", dir, strerror(errno));
        return 1;
    }
    
    return 0;
}


static int builtin_exitt(Command *cmd) {
    int code = 0;
    if (cmd->argc >= 2) {
        code = atoi(cmd->argv[1]);
    }
    exit(code)
}

// complete this
static int builtin_export(Command *cmd) {
    // todo- implement with env table

    if (cmd->argc < 2) {
        // todo: print all exported vars
        return 0;
    }

    child *arg = cmd->argv[1];
    char *eq = strchr()
}

static int builtin_pwd(Command *cmd) {
    (void)cmd; //unused;
    char buf[4096];
    if (getcwd(buf, sizeof(buf)) == NULL) {
        perror("getcwd");
        return 1;
    }
    printf("%s\n", buf);
    return 0;
}

int execute_builtin(Command *cmd) {
    if (strcmp(cmd->argv[0], "cd")     == 0) return builtin_cd(cmd);
    if (strcmp(cmd->argv[0], "exit")   == 0) return builtin_exit(cmd);
    if (strcmp(cmd->argv[0], "export") == 0) return builtin_export(cmd);
    if (strcmp(cmd->argv[0], "pwd")    == 0) return builtin_pwd(cmd);

    /* should never reach here if is_builtin() is in sync */
    fprintf(stderr, "mysh: unknown builtin: %s\n", cmd->argv[0]);
    return 1;
}
