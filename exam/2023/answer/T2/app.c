#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utest.h"

int add(int a, int b)
{
    return a + b;
}

// Logical error
int sub(int a, int b)
{
    return a * b;
}

int mul(int a, int b)
{
    return a * b;
}

int mod(int a, int b)
{
    return a % b;
}

void test_add()
{
    assert(add(1, 1) == 2);
}

void test_sub()
{
    assert(sub(1, 1) == 0);
}

void test_mul()
{
    assert(mul(1, 1) == 1);
}

void test_mod()
{
    assert(mod(5, 2) == 1);
}

int main(int argc, char *argv[])
{
    utest_add("test_add", test_add);
    utest_add("test_sub", test_sub);
    utest_add("test_mul", test_mul);
    utest_add("test_mod", test_mod);
    utest_run();
    return 0;
}
