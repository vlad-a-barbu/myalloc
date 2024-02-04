#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void *
malloc(size_t size) 
{
    void *p;
    p = sbrk(0);
    if (sbrk(size) == (void*)-1)
        return NULL;
    return p;
}

int main() 
{
    size_t size = 10;
    int* arr = malloc(sizeof(int) * size);
    for (size_t i = 0; i < size; ++i)
        *(arr + i) = i;
    return 0;
}