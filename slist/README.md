# slist

## 简介

这是一个单链表模块，这个模块将单链表操作的一些共同方法分离出来。在需要使用单链表的时候，就不需要每次都重复实现单链表的共同部分的方法了。

## 用法

这个单链表库只实现了对单链表的简单操作，具体使用时需要实现数据相关的一些方法：

```c
typedef void *(*slist_fn_alloc) (void);             // 申请节点内存的方法
typedef void (*slist_fn_free) (slist_t node);       // 释放节点内存的方法
typedef int (*slist_fn_fill) (slist_t node, void *param);       // 填充节点的方法
typedef int (*slist_fn_node_do) (slist_t node, void *param);    // 操作节点的方法
typedef int (*slist_fn_check) (slist_t node, void *param);      // 检查节点的方法
```

### 1. 创建新单链表

定义新的单链表，这个新的单链表是真正包含业务数据的链表，定义方法如下：

```c
struct list_user { 
    struct slist parent;
    // .....
    int foo;
    char bar;
    // .....
};

```

上面的操作相当于面向对象语言中的“继承”，`struct list_user` “继承” 了`struct slist`

### 2. 定义申请内存和释放内存的方法

```c
typedef struct list_user list_user_t;
static void *list_node_malloc()
{
    list_user_t node = (void*)malloc(sizeof(struct list_user));
    if (!node) return NULL;

    node->foo = 123;    // 初始化成员变量
    node-bar = 12;      // 初始化成员变量

    return node;
}

static void list_node_free(slist_t nodeIn)
{
    free(nodeIn);
    // 如果有其他通过malloc申请的内存，需要释放
}

```

### 3. 实现一些常用方法

以创建节点为例，实现方法为：

```c
static list_user_t list_node_create()
{
    return (list_user_t) slist_node_create(list_node_malloc, list_node_free, NULL, NULL);
}

```

其他的方法也与这类似，具体请参照头文件
