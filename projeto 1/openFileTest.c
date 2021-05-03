// openFileTest.c

#include <stdio.h>
#include <string.h>
#define MAXCHAR 1000
int main() {
    FILE *fp;
    char str[MAXCHAR];
    char* filename = "data/index.txt";
    char example[2000];
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    while (fgets(str, MAXCHAR, fp) != NULL)
        strcat(example, str);
        // printf("%s", str);
    fclose(fp);
    printf("%s", example);
    return 0;
}