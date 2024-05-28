//Viikko 10 tehtävä 3.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;
    
    switch (pid = fork()) {
    
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
        
    case 0:
        printf("Child %d was born, running child...\n", getpid());
        if (execvp(argv[1], argv+1) == -1) {
            perror("execvp");
            exit(1);
        }
        break;
        
    default:
        printf("Child %d was born, waiting in parent...\n", pid);
        if (wait(&status) == -1) {
            perror("wait");
            exit(1);
        }
        printf("Child done, status: %d\n", status);
        break;
    }
    
    return 0;
}
