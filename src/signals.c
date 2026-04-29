#include "shell.h"

static void sigchild_handler(int sig){
    // save and restore errno
    // waitpid sets errno on failure if not saved, the interrupted code might see a different errno

    int saved_errno = errno;

    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        //child pid reaped
        // todo: update job table implementing job control
        (void)pid;
        (void)status;
    }
    errno = saved_errno;
}

void setup_signals(void) {
    struct sigaction sa;

    //ignore sigint in the shell
     sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        die("sigaction SIGINT");

    //ignore shigquit in hte shell
    if(sigaction(SIGQUIT, &sa, NULL) == -1)
        die("sigaction SIGQUIT");

    //ignore sigstp in the shell
    if(sigaction(SIGTSTP, &sa, NULL) == -1)
        die("sigactino SIGSTP");

    //handle SIGCHILD to reap zombie childre
    sa.sa_handler = sigchild_handler;
    sigemptyset(&sa.sa_mask);

     /*
     SA_RESTART  - restart syscalls interrupted by this signal
                   so our read() in main.c doesn't return EINTR
     SA_NOCLDSTOP - only get SIGCHLD on child exit, not stop/continue
*/
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        die("sigaction SIGCHLD");


}