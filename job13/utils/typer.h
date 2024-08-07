#ifndef _TYPER_H
#define _TYPER_H

extern void typer_indent();
extern void typer_push();
extern void typer_pop();
extern void typer_dump(char *format, ...);
extern void typer_test();

#endif
