#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 结构体定义 */
typedef struct students{
	long long stu_number;
	char name[64];
	int class;
	char username[64];
	int age;
	double mark;
	char password[64];
}student;

typedef struct teachers{
	char name[64];
	long long job_number;
	int age;
	char nickname[64];
	char password[64];
} teacher;

typedef struct array{
    void *data;
    int index;
    int capacity;
    int size;
} VarArray;

typedef struct databases {
	FILE* data;
	char name[256];
	int capacity;
}database;

// 邮件结构体
typedef struct email {
    long long sender_id;
    long long target_class;
    char content[256];
    int read_status;
    time_t send_time;
} email;

// 成绩结构体
typedef struct grade {
    long long student_id;
    char exam_name[64];
    double score;
    time_t exam_time;
} grade;

// 雨伞结构体
typedef struct umbrella {
    int id;
    int borrowed;
    long long borrower_id;
    time_t borrow_time;
} umbrella;

// 系统配置
#define teacherdef 0
#define studentdef 1
#define stufile_elif "student.dat"
#define tecfile_elif "teacher.dat"
#define emailfile "email.dat"
#define gradefile "grade.dat"
#define umbrellafile "umbrella.dat"
#define filebak "backup.dat"
#define MAX_PASSWORD_LEN 64
#define MAX_NAME_LEN 64
#define MAX_CONTENT_LEN 256

// 函数声明
void printhead(void);
void print_main_menu(void);
void print_teacher_menu(void);
void print_student_menu(void);
VarArray ini_VarArray(int ini_length, int size);
VarArray* re_VarArray(VarArray* Array, int length);

#endif