## 源代码说明

+ 包含了答案的源代码
+ 不要给学生
+ 这次课要读代码，完成一小部分代码

## mmap 的使用

+ 在当前目录下，存在 mmap 目录，有 2 个例子
+ 需要向学生讲解一下

## 测试用目录 city

当前目录下，存在一个目录 city，用于测试

```
city
├── china
│   ├── beijing.py
│   ├── jiangsu
│   │   ├── nanjing.py
│   │   └── wuxi.py
│   └── shanghai.py
├── hello.c
└── world.c
```

## 镜像文件

+ disk/city.bin
  - 磁盘镜像
  - 保存了 city 目录下的所有文件

+ disk/empty.bin
  - 磁盘镜像
  - 保存了一个空目录

## 编译运行

$ make
$ ./dump.uxfs disk/empty.bin
$ ./dump.uxfs disk/city.bin

## 最终任务

+ 完成 dump.c 中 TODO 的部分
+ 能够显示出 city 的目录结构

