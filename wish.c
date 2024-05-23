#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_DIRS 100
#define MAX_DIR_LENGTH 100

//Set the original path
char shell_path[MAX_DIRS * MAX_DIR_LENGTH + MAX_DIRS] = "/bin";


//Initialize the utility programs
void execute_command(char *command);
int execute_builtin_commands(char *command);
void error_handling();

void interactive_mode(void);
void batch_mode(char* batch_file_name);

//The main function runs the first loop of the shell. It takes in zero to one arguments.
int main() {
    while(1) {
        char* user_input = NULL;
        size_t input_size = 0;
        
	//Print the promt
        printf("$> ");
        ssize_t input_length = getline(&user_input, &input_size, stdin);
        
        //Check for empty command and print error
        if (input_length <= 1) {
            error_handling();
            free(user_input);
            continue;
        }
        
        user_input[strcspn(user_input, "\n")] = 0;

        int n_arguments = 0;
        char* arguments[2];
        
        //Seperate the arguments to check for incorrect input
        arguments[n_arguments++] = strtok(user_input, " ");
        while ((arguments[n_arguments++] = strtok(NULL, " ")) != NULL);
        n_arguments--;

	//Print error for too many arguments
        if (n_arguments > 2) {
            error_handling();
        
        //Check for the correct input ./wish and choose the mode based on the number of arguments
        } else if (strcmp(arguments[0], "./wish") == 0) {
            if (n_arguments == 2) {
                batch_mode(arguments[1]);
                break;
            } else if (n_arguments == 1) {
                interactive_mode();
                break;
            }
            
        //Check for an exit call
        } else if (strcmp(user_input, "exit") == 0) {
            execute_builtin_commands(user_input);
        }

        free(user_input);
    }
    return 0;
}

//Execute command runs commands excluding the built-in commands. It takes the command argument from interactive_mode or batch_mode depending on whether the user gave a batch file or not. 
void execute_command(char *command) {
    char *args[100];
    char *output_file = NULL;
    int i = 0;
    
    //Check for need of redirection and tokenize the command
    char *token = strtok(command, " ");
    while (token != NULL && i < 99) {
        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            output_file = token;
            break;
        }
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    
    //Fork and check if the fork was successful
    pid_t pid = fork();
    if (pid == 0) {
    
    	//Start the child process
        if (output_file != NULL) {
            freopen(output_file, "w", stdout);
            freopen(output_file, "w", stderr);
        }
        
        //Make a path to the executable
        char *full_path = malloc(strlen(shell_path) + strlen(args[0]) + 2);
        sprintf(full_path, "%s/%s", shell_path, args[0]);
        
        //Check for access to the executable (if the executable exists)
        if (access(full_path, X_OK) == -1) {
          free(full_path);
          free(command);
          exit(EXIT_FAILURE);
        }
        
        //Execute with the given arguments
        if (execv(full_path, args) == -1) {
          error_handling();
          free(full_path);
          free(command);
          exit(EXIT_FAILURE);
        }
    } else {
    	
    	//Parent process waits for the child process to end
        int status;
        wait(&status);
    }
}

//The interactive mode allows the user to give commands one at a time. 
void interactive_mode(void) {
    while (1) {
        char* command = NULL;
        size_t command_size = 0;
        
	//Print the prompt
        printf("wish> ");
        
        //This is needed for the first time that the user enters interactive mode. 
        ssize_t command_length = getline(&command, &command_size, stdin);
        if (command_length == -1) {
            free(command);
            exit(EXIT_FAILURE);
        }

        command[strcspn(command, "\n")] = 0;
        
        //Check for parallel commands and tokenize the command
        char *parallel_commands[100];
        int num_parallel_commands = 0;
        char *token = strtok(command, "&");
        while (token != NULL && num_parallel_commands < 99) {
            parallel_commands[num_parallel_commands++] = token;
            token = strtok(NULL, "&");
        }
        
        //Check if the command given is a built-in command. Returns 1 if true and does not try to run the command again with execute command. If execute_builtin_command returns 0, the command is then passed on to execute_command. 
        for (int i = 0; i < num_parallel_commands; i++) {
            if (!execute_builtin_commands(parallel_commands[i])) {
        
                execute_command(parallel_commands[i]);
            }
        }
        free(command);
    }
}

//The batch mode reads input from a file and executes the commands in the said file. 
void batch_mode(char* batch_file_name) {
    
    //Open the file based on its name in read mode. If it fails, print out error message and call for exit. 
    FILE* file = fopen(batch_file_name, "r");
    if (file == NULL) {
        error_handling();
        exit(1);
    }

    char* line = NULL;
    size_t line_size = 0;
    
    //Get the text from a line one line at a time
    while (getline(&line, &line_size, file) != -1) {
        line[strcspn(line, "\n")] = 0;
        
        //Check for parallel commands and tokenize the command
        char *parallel_commands[100];
        int num_parallel_commands = 0;
        char *token = strtok(line, "&");
        while (token != NULL && num_parallel_commands < 99) {
            parallel_commands[num_parallel_commands++] = token;
            token = strtok(NULL, "&");
        }
        
        //Check if the command given is a built-in command. Returns 1 if true and does not try to run the command again with execute command. If execute_builtin_command returns 0, the command is then passed on to execute_command. 
        for (int i = 0; i < num_parallel_commands; i++) {
            if (!execute_builtin_commands(parallel_commands[i])) {
                execute_command(parallel_commands[i]);
            }
        }
    }
    
    fclose(file);
    free(line);
}


//Built-in commands:
int execute_builtin_commands(char *command) {

    //Get the command from interactive_mode or batch_mode and check if it is one of the built-in commands.
    
    //If the command is "exit", close the shell by calling exit with 0.
    if (strcmp(command, "exit") == 0) {
        exit(0);
        
    //If the command is "cd", check that it has exactly one argument. If not, print the error message. Return 1 if "cd" was found and executed succesfully. 
    } else if (strncmp(command, "cd", 2) == 0) {
        char *arg = strtok(command, " ");
        arg = strtok(NULL, " ");
        if (arg == NULL) {
            error_handling();
        } else {
            char *next = strtok(NULL, " ");
            if (next != NULL) {
               error_handling();
            } else if (chdir(arg) != 0) {
                perror("cd");    
            } else {
                return 1;
            }
        }
        
    //Check if the command starts with "path"
    } else if (strncmp(command, "path", 4) == 0) {
        
        //Tokenize the command
        char *arg = strtok(command, " ");
        arg = strtok(NULL, " ");
        
        //If the user gives no arguments, set the shell path to empty. While the path is empty, the shell can only run built-in commands. 
        if (arg == NULL) {
            shell_path[0] = '\0';
            return 1;
        } else {
            strcpy(shell_path, arg);
            arg = strtok(NULL, " ");
            while (arg != NULL) {
                //Loop through all tokens and add them to the path
                strcat(shell_path, " ");
                strcat(shell_path, arg);
                arg = strtok(NULL, " ");
            }
            
            //If "path" was found and executed, return 1.
            return 1;
        }
    }
    
    //If the command was not a built-in command, return 0.
    return 0;   
}

//Program errors
//Print this message when encountering errors that the shell cathes. 
void error_handling() {
    char error_message[30] = "An error has occured\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

