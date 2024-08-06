/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-06 20:16:43
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-06 23:00:16
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
      return 0;
    }
  }
}