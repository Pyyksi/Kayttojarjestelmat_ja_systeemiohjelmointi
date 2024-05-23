//Viikko 9 tehtävä 1. main

#include <stdio.h>
#include <stdlib.h>
#include "header_file.h"

int main(int argc, char *argv[]) {
    //Check for the correct amount of arguments
    if (argc != 4) {
        printf("usage: %s <number> <number> <number>\n", argv[0]);
        return 1;
    }
    
    Numbers numbers;
    numbers.a = atoi(argv[1]);
    numbers.b = atoi(argv[2]);
    numbers.c = atoi(argv[3]);
    
    int summed = sum(numbers);
    int multiplied = multiply(numbers);
    
    printf("%d\n", summed);
    printf("%d\n", multiplied);
    
    return 0;
}

