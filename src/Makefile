# 定义编译器
CC = gcc

# 定义编译器标志，这里不使用任何优化
CFLAGS = -O0

# 通过wildcard函数获取当前目录下的所有的.c文件
SRC = $(wildcard *c)

# 根据 .c 文件生成对应的可执行文件名字
EXEC = $(SRC:.c=)

# 默认目标
all: $(EXEC)

# 生成每个可知执行文件的规则
# 使用%进行模式匹配
%: %.c
	$(CC) $(CFLAGS) -o $@ $<


# 清理生成的文件
clean:
	rm -f $(EXEC)