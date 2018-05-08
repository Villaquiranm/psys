#include "sysapi.h"

int main(void *arg)
{
        (void)arg;

//        pcreate(10);

        //char str[10] = "Hello\0";
        cons_write("Hello\0",sizeof("Hello\0"));

        return 32832;
}
