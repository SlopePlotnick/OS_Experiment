#include <stdio.h>
#include <stdlib.h>

enum {
    SHAPE_RECT,
    SHAPE_CIRCLE,
};

struct shape {
    int type;
};

struct rect {
    int type;
    int width;
    int height;
};

struct circle {
    int type;
    int radius;
};

struct shape *shapes[2];

void shape_dump(struct shape *shape) 
{
    struct rect *rect;
    struct circle *circle;

    switch (shape->type) {
        case SHAPE_RECT:
            rect = (struct rect *)shape;
            printf("RECT widht=%d height=%d\n", rect->width, rect->height);
            break;

        case SHAPE_CIRCLE:
            circle = (struct circle *)shape;
            printf("CIRCLE radius=%d\n", circle->radius);
            break;
    }
}

struct rect *build_rect(int width, int height)
{
    struct rect *rect = malloc(sizeof(struct rect));
    rect->type = SHAPE_RECT;
    rect->width = width;
    rect->height = height;
    return rect; 
}

struct circle *build_circle(int radius)
{
    struct circle *circle = malloc(sizeof(struct circle));
    circle->type = SHAPE_CIRCLE;
    circle->radius = radius;
    return circle; 
}

int main()
{
    struct rect *rect = build_rect(3, 4);
    struct circle *circle = build_circle(5);

    shapes[0] = (struct shape *) rect;
    shapes[1] = (struct shape *) circle;

    int i;
    for (i = 0; i < 2; i++) {
        struct shape *shape = shapes[i];
        shape_dump(shape);
    }
    return 0;
}
