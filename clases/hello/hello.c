#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Hello World\n");
    return;

}

void* greet(void*)
{
    printf("Hello World\n");
    return NULL;
}