#include <stdio.h>
#include <time.h>

void write_log(char* filename, char* message){
    FILE* file;
     time_t now;

    now = time(NULL);
    file = fopen(filename, "a");
    if(file == NULL) return;
    fprintf(file,"[%ld] %s\n",now,message);
    fclose(file);
}