//Viikko 9 tehtävä 1. header file

#ifndef HEADER_FILE_H
#define HEADER_FILE_H

typedef struct {
    int a;
    int b;
    int c;
} Numbers;

int sum(Numbers numbers);
int multiply(Numbers numbers);

#endif

