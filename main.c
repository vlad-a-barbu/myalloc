#include <unistd.h>

#define mul4(x) (((((x) - 1) / 4) * 4) + 4)
#define BLOCK_SIZE 18

typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

typedef struct s_block *t_block;

struct s_block 
{
    u_long  size;
    t_block next;
    u_short free;
    char    end[1];
};

void *base = NULL;

t_block
find(t_block *last, u_long size)
{
    t_block b = base;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return (b);
}

t_block
extend(t_block last, u_long size)
{
    t_block b;
    b = sbrk(0);
    if (sbrk(BLOCK_SIZE + size) == (void*)-1)
        return (NULL);
    b->size = size;
    b->next = NULL;
    if (last)
        last->next = b;
    b->free = 0;
    return (b);
}

void
split(t_block b, u_long size)
{
    t_block new;
    new         = b->end + size;
    new->size   = b->size - size - BLOCK_SIZE;
    new->next   = b->next;
    new->free   = 1;
    b->size     = size;
    b->next     = new;
}

void *
malloc(u_long size) 
{
    t_block b, last;
    u_long s;
    s = mul4(size);
    if (base) {
        last = base;
        b = find(&last, s);
        if (b) {
            if((b->size - s) >= (BLOCK_SIZE + 4))
                split(b, s);
            b->free = 0;
        } else {
            b = extend(last, s);
            if (!b)
                return (NULL);
        }
    } else {
        b = extend(NULL, s);
        if (!b)
            return (NULL);
        base = b;
    }
    return (b->end);
}

int
main() 
{
    u_short size = 10;
    int* arr = malloc(sizeof(int) * size);
    for (u_short i = 0; i < size; ++i)
        *(arr + i) = i;
    return (0);
}