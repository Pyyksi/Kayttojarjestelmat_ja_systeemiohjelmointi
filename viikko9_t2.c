//Viikko 9 tehtävä
//Prints out command line parameters and environment variables

#include<stdio.h>
int main(int argc, char *argv[], char *envp[]) {
    printf("Command line parameters:\n");
    for (int i = 1; i < argc; i++) {
        printf("%d: %s\n", i, argv[i]);
    }
    printf("\n");
    printf("Environment variables:\n");
    for (int i = 0; envp[i] != NULL; i++) {
        printf("%s\n", envp[i]);
    }
    printf("\n");
}
