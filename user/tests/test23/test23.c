#include "sysapi.h"

int main(void *arg)
{
        unsigned long quartz;
        unsigned long ticks;

        (void)arg;

        wait_clock(200);
        clock_settings(&quartz, &ticks);
        printf("Quartz: %u, Ticks: %u", quartz, ticks);
        wait_clock(100);
        printf("oi: %u", current_clock());

//        pcreate(10);

        char str[10];
        // //"Hello\0";
        // strncpy(str,"Hello\0",5);
        // cons_write(str,strlen(str));
        //
        // cons_write("Salut\0",6);

        //printf("S: %s\n", str);
        //printf(str);

        if(str==NULL)
          return 32832;
        else
          return 32832;
}
