#include "std.h"
#include "typer.h"

static int typer_level;

void typer_indent()
{
    for (int i = 0; i < typer_level; i++)
        printf("  ");
}

void typer_push()
{
    typer_level++;
}

void typer_pop()
{
    typer_level--;
}

void typer_dump(char *format, ...)
{
    va_list ap;
    typer_indent();

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

#if 0
class
   student
     name mike
     age 10
   student
     name john
     age 12 
#endif
void typer_test()
{
    typer_dump("class\n");
    typer_push();

    typer_dump("student\n");
    typer_push();
    typer_dump("name %d\n", "mike");
    typer_dump("age %d\n", 10);
    typer_pop();

    typer_dump("student\n");
    typer_push();
    typer_dump("name %d\n", "john");
    typer_dump("age %d\n", 12);
    typer_pop();

    typer_pop();
}
