/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-06 20:16:43
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-10 00:36:35
 * @FilePath: /Linux-kernel-start/src/GUN-C-Extend.c
 * @Description: GUN C 扩展
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

// 系统库文件
#include <stdio.h>
#include <stdlib.h>

/**
 * 1. 语句表达式
 */

#define max(a, b)                                                              \
  ({                                                                           \
    printf("a: %d, b: %d\n", (a), (b));                                        \
    (a) > (b) ? (a) : (b);                                                     \
  })

// max宏存在安全问题，a和b有可能会计算两次，
// 问题原理：宏展开的结果会使得 i++ 和 j++ 在比较 (a) > (b)
// 时各执行一次，然后在决定返回 (a) 或 (b) 时，再执行一次。
// 比如，向a传入i++，向b传入b++
// maxint就是为了解决此问题，通过重新定义变量解决此问题
#define maxint(a, b)                                                           \
  ({                                                                           \
    printf("a: %d, b: %d\n", (a), (b));                                        \
    int _a = (a), _b = (b);                                                    \
    printf("_a: %d, _b: %d\n", (_a), (_b));                                    \
    _a > _b ? _a : _b;                                                         \
  })

// maxint 缺陷是只能接收int类型的参数，下列函数只要保证两个参数是可比较类型即可
#define min(x, y)                                                              \
  ({                                                                           \
    typeof(x) _min1 = (x);                                                     \
    typeof(y) _min2 = (y);                                                     \
    (void)(&_min1 == &_min2);                                                  \
    \    
    _min1 < _min2                                                              \
        ? _min1                                                                \
        : _min2;                                                               \
  })
// (void)(&_min1 == &_min2);作用是类型检查，检查_min1和_min2类型是否相同

// 构造新类型
// typeof (*x) y;   // 声明y是x指针指向的类型
// typeof (*x) z[4];    // 声明z是数组，其中数组类型是x指针指向的类型
// typeof (typeof (char *)[4]) m;   //
// 声明m是指针数组，这和char*m[4]声明的效果是一样的

/**
 * 2.变长数组
 */

struct line {
  int length;
  char contents[0]
};

typedef char *(*_get_name)();
char *get_student_name() { return "0xSchnappi"; }

typedef int (*_get_count)();
int get_student_count() { return 10; }

struct Student {
  _get_name get_name;
  _get_count get_count;
};

/**
 * 5.可变参数宏
 * 主要运用在输出函数中
 */

#define debug_info(fmt, ...) printf(fmt, ##__VA_ARGS__)

/**
 * 6.函数属性
 * 格式: __attribute__((attribute-list))
 */

void __attribute__((noreturn)) die(void);

/**
 * 7.变量属性和类型属性
 * 常见的有：alignment、packed和sections
 * packed 对变量以字节对齐，对域以位对齐
 */
struct example {
  char a;             // 普通变量，字节对齐
  int b;              // 普通变量，字节对齐
  unsigned int c : 3; // 位域，位对齐
  unsigned int d : 5; // 位域，位对齐
  unsigned int e : 8; // 位域，位对齐
} __attribute__((packed));

/**
 * 8.内建函数
 * 由GNU C提供了一系列内建函数以进行优化，内建函数以"_builtin_"作为前缀
 * __builtin_expect用于分支预测，加快执行效率的
 */

#define LIKELY(x) __builtin_expect(!!(x), 1)   // x很可能为真
#define UNLIKELY(x) __builtin_expect(!!(x), 0) // x很可能为假

void main() {

  /**
   * 1. 语句表达式
   */
  printf("Compare 1 and 2, max: %d\n", max(1, 2));

  int a = 1, b = 2;
  printf("Compare 1++ and 2++, max: %d\n", max(a++, b++));
  a = 1;
  b = 2;
  printf("Compare 1++ and 2++, maxint: %d\n", maxint(a++, b++));

  /**
   * 2.变长数组
   */

  int thislength = 10;
  printf("struct line size: %ld, int size:%ld\n", sizeof(struct line),
         sizeof(int));

  struct line *thisline = malloc(sizeof(struct line) + thislength);
  thisline->length = thislength;
  printf("struct line size: %ld, int size:%ld\n", sizeof(struct line),
         sizeof(int));
  for (size_t i = 0; i < thisline->length; i++) {
    thisline->contents[i] = i;
  }

  for (size_t i = 0; i < thisline->length; i++) {
    printf("index=%ld, contents[%ld] = %d\n", i, i, thisline->contents[i]);
  }

  free(thisline);

  /**
   * 3. case 的范围
   */

  int val = 0;
  const char name[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  const char *tmp = NULL;
  tmp = name;

  for (;; tmp++) {
    switch (*tmp) {
    case '0' ... '8':
      val = 10 * val + (*tmp - '0');
      printf("*tmp: %c, val: %d\n", *tmp, val);
      break;

    default:
      printf("*tmp: %c\n", *tmp);
      break;
    }

    if (*tmp < '0' || *tmp > '8') {
      break; // 退出 while 循环
    }
  }

  /**
   * 4.标号元素
   * 初始化不需要按照结构体定义的顺序进行初始化
   * 未初始化的成员的值为0或NULL
   * 这种经常出现在Linux内核处理不同平台的问题上
   */

  static const struct Student middleSchool = {
      .get_count = get_student_count,
  };

  debug_info("Hello world - %s", "0xSchnappi");

  for (size_t i = 0; i < 50; i++) {
    // 很明显50以内，能被5整除的数字占少数，就可以使用UNLIKELY进行分支预测进行优化
    if (UNLIKELY(i % 5 == 0)) {
      ;
    } else {
      ;
    }
  }
}