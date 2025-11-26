CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = ts
SOURCES = main.c login.c Function.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = main.h Function.h

# 默认目标
all: $(TARGET)

# 链接目标程序
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# 编译源文件
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o

login.o: login.c $(HEADERS)
	$(CC) $(CFLAGS) -c login.c -o login.o

Function.o: Function.c $(HEADERS)
	$(CC) $(CFLAGS) -c Function.c -o Function.o

# 清理编译产物
clean:
	rm -f $(OBJECTS) $(TARGET)

# 重新编译
rebuild: clean all

# 显示帮助信息
help:
	@echo "  clean    - 清理编译产物"
	@echo "  rebuild  - 重新编译"
	@echo "  help     - 显示此帮助信息"

.PHONY: all clean rebuild install help




