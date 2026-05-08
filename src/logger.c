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
void memory_log(char* msg)
{
    FILE* file;

    file = fopen("logs/memory_log.txt",
                 "a");

    if(file == NULL)
    {
        return;
    }

    fprintf(file,
            "%s\n",
            msg);

    fclose(file);
}