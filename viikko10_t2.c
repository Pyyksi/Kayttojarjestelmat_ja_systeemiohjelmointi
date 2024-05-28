//Viikko 10 tehtävä 2.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    //Turning arguments into a string for system()
    char command[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        strcat(command, " ");
    }
    
    //System call
    system (command);
    
    //Ecex call
    execvp(argv[1], argv+1);
    
    //Print to confirm that execvp works   
    printf("This line will not be printed if execvp() is succesfull.\n");
    
    printf("Execvp did not work.\n");
    exit(EXIT_FAILURE);
}
