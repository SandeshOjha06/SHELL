#include "shell.h"

/*
setup_redirects()

called in child process. allocates stdin/stdout to the files specifies in the command
use dup2() to make fd 0 or 1 and closes the original fd
*/

static void setup_redirects(Command *cmd) {
    if(cmd->input_file) {
        int fd = open(cmd->input_file, O_RDONLY);

        if (fd == -1) {
            perror(cmd -> input_file);
            exit(1);
        }

        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if(cmd->output_file) {
        int flags = O_WRONLY | O_CREAT;
        int fd = open(cmd->output_file, flags, 0644);

        if (fd == -1) {
            perror(cmd -> output_file);
            exit(1);
        } 

        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

static void execute_single(Command *cmd) {
    // buitins - runs in the shell process(no fork)
    if (is_builtin(cmd->argv[0])) {
        execute_builtin(cmd);
        return;
    }

    pid_t pid = fork()

    if (pid == -1){
        perror("fork");
        return;
    }

    if (pid == 0) {
        signal(SIGINT, SIG_DIF);
        signal(SIGQUIT, SIG_DIF);
        signal(SIGTSTP, SIG_DIF);

        setup_redirects(cmd);

        //replacing process with command
        execvp(cmd->argv[0], cmd->argv);

        // only return on execvp failure
        fprintf(stderr, "mysh: %s: %s\n", cmd->argv[0], strerror(errno));
        exit(127); 

    }

    int status;

    waitpid(pid, &status, 0);

    // work left - save exit status for $? */
}

