#include "sysapi.h"

int main(void *arg)
{
        (void)arg;

//        pcreate(10);

        char str[10];

        cons_write("Salut\0",strlen("Salut\0"));

        //"Hello\0";
        strncpy(str,"Hello\0",5);
        cons_write(str,strlen(str));

        strncpy(str,".\0",2);
        cons_write(str,strlen(str));



        //printf("S: %s\n", str);
        //printf(str);



        if(str==NULL)
          return 32832;
        else
          return 32832;
}
