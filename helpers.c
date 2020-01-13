#include "helpers.h"

void msleep(int miliseconds) {
    usleep(miliseconds * 1000);
}

char* inttostr(int num, int len) {
    char *str = malloc((size_t) len);
    char *format = malloc((size_t) 3);
    sprintf(format, "%%0%dd", len);
    sprintf(str, format, num);
    return str;
}

int strtoint(char* str) {
    long num;
    char*ptr;
    num = strtol(str, &ptr, 10);
    return (int)num;
}