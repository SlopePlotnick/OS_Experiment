// + 实现函数 tree_visit
//   - 主程序遍历树的每个节点
//   - 对每个节点创建一个子线程，在这道题目中，总共创建 7 个子线程
//   - 在每个子线程中，打印 tree->data
//
// + 实现函数 wait_sub_threads
//   - 等待所有的子线程执行完毕
//   - 在 list_visit 时，需要记录下所有线程的 ID，记录在全局变量中
//   - 在 wait_sub_threads 中使用 pthread_join
//
// + 程序可能的输出结果
//   - 字符 a b c d e f g 每个出现一次，出现的顺序可能是随机的  
//   - END 一定是最后出现
//   a
//   d
//   b
//   e
//   c
//   f
//   g
//   END
#include <stdio.h>
#include <stdlib.h>

struct tree {
    char data;
    struct tree *left;
    struct tree *right;
};

struct tree *tree_create(char data, struct tree *left, struct tree *right)
{
    struct tree *tree = malloc(sizeof(struct tree));
    tree->data = data;
    tree->left = left;
    tree->right = right;
    return tree;
}

void tree_visit(struct tree *tree)
{
}

// 先序遍历算法，供参考
void tree_pre_order(struct tree *tree)
{
    if (tree == NULL)
        return;
    printf("%c\n", tree->data);
    tree_pre_order(tree->left);
    tree_pre_order(tree->right);
}

void wait_sub_threads()
{
}

int main()
{
    //     a
    //  b     e
    // c d   f g

    struct tree *c = tree_create('c', NULL, NULL);
    struct tree *d = tree_create('d', NULL, NULL);
    struct tree *b = tree_create('b', c, d);

    struct tree *f = tree_create('f', NULL, NULL);
    struct tree *g = tree_create('g', NULL, NULL);
    struct tree *e = tree_create('e', f, g);

    struct tree *a = tree_create('a', b, e);

    tree_visit(a);
    wait_sub_threads();

    puts("END");
    return 0;
}
