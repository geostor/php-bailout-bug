#include <stdio.h>
#include <string.h>
#include <setjmp.h>

static unsigned char buf[256]; 
static long used;
static jmp_buf out;

static char *alloc(int at, long size)
{
    used += size;
    if (used > 40)
        longjmp(out, 1);
    return (char *)&buf[at];
}

static char *store(int at, const char *s)
{
    char *p = alloc(at, strlen(s) + 1); /* may jump out before returning */
    strcpy(p, s);
    return p;
}

typedef struct {
    char *user;
    char *group;
} Session;

int main(void)
{
    /* first use */
    Session *a = (Session *)&buf[0];
    a->user  = store(100, "user1");
    a->group = store(120, "admin");
    printf("user=%s group=%s\n", a->user, a->group);

    /* second use */
    Session *b = (Session *)&buf[0];
    if (setjmp(out) == 0) {
        used = 32;
        b->user  = store(150, "user2");
        alloc(170, 10); /* over the limit -> jump out */
        b->group = store(190, "guest");
    }
    printf("user=%s group=%s\n", b->user, b->group);
    
    return 0;
}
