#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "main.h"
#include "Function.h"

extern int attempts;
extern const int max_attempts;

// 声明在 Function.c 中定义的全局变量
extern VarArray tcr_table, stu_table;
extern database teacher_base, student_base;

// 密码验证函数
int verify_password(void* user, int user_type) {
    int attempts = 0;
    const int max_attempts = 3;
    
    while(attempts < max_attempts) {
        printf("请输入密码: ");
        char password[MAX_PASSWORD_LEN];
        supGets(password, MAX_PASSWORD_LEN, 3);
        
        int password_correct = 0;
        if(user_type == teacherdef) {
            teacher* tcr = (teacher*)user;
            password_correct = (strcmp(password, tcr->password) == 0);
        } else {
            student* stu = (student*)user;
            password_correct = (strcmp(password, stu->password) == 0);
        }
        
        if(password_correct) {
            if(user_type == teacherdef) {
                teacher* tcr = (teacher*)user;
                printf("\033[32m登录成功！欢迎教师 %s (工号: %lld)\033[0m\n", tcr->name, tcr->job_number);
                teacher_interface(tcr, &student_base, &teacher_base);  // 修复这里
            } else {
                student* stu = (student*)user;
                printf("\033[32m登录成功！欢迎学生 %s (学号: %lld)\033[0m\n", stu->name, stu->stu_number);
                student_interface(stu, &student_base, &teacher_base);  // 修复这里
            }
            return 1;
        } else {
            attempts++;
            if(attempts < max_attempts) {
                printf("\033[31m密码错误！还有 %d 次机会\033[0m\n", max_attempts - attempts);
            }
        }
    }
    
    printf("\033[31m密码错误次数过多，登录失败！\033[0m\n");
    return 0;
}

// 登录函数
int login(int user_type, database* user_base, VarArray* table) {
    if(verification1() != 0 || verification2() != 0) {
        printf("\033[31m验证码验证失败，登录失败！\033[0m\n");
        return 0;
    }
    
    long long user_id;
    printf("请输入您的工号或学号: ");
    if(scanf("%lld", &user_id) != 1) {
        printf("\033[31m输入错误！\033[0m\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    
    int index = search(table, user_id);
    if(index == -1) {
        printf("\033[31m用户不存在！请先注册！\033[0m\n");
        return -1;
    }
    
    FILE* file = fopen(user_base->name, "rb");
    if(file == NULL) {
        printf("\033[31m无法打开用户数据库！\033[0m\n");
        return 0;
    }
    
    int found = 0;
    int login_result = 0;
    
    if(user_type == teacherdef) {
        teacher tcr;
        while(fread(&tcr, sizeof(teacher), 1, file) == 1) {
            if(tcr.job_number == user_id) {
                found = 1;
                login_result = verify_password(&tcr, user_type);
                break;
            }
        }
    } else {
        student stu;
        while(fread(&stu, sizeof(student), 1, file) == 1) {
            if(stu.stu_number == user_id) {
                found = 1;
                login_result = verify_password(&stu, user_type);
                break;
            }
        }
    }
    
    fclose(file);
    
    if(!found) {
        printf("\033[31m读取用户数据失败！\033[0m\n");
        return 0;
    }
    
    return login_result;
}

// 重新设计的登录函数
int redesigned_login(database* teacher_db, database* student_db, 
                    VarArray* teacher_table, VarArray* student_table) {
    if(verification1() != 0 || verification2() != 0) {
        printf("\033[31m验证码验证失败，登录失败！\033[0m\n");
        return 0;
    }
    
    long long user_id;
    printf("请输入您的工号或学号: ");
    if(scanf("%lld", &user_id) != 1) {
        printf("\033[31m输入错误！\033[0m\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    
    int teacher_index = search(teacher_table, user_id);
    int student_index = search(student_table, user_id);
    
    if(teacher_index != -1) {
        return login(teacherdef, teacher_db, teacher_table);
    } else if(student_index != -1) {
        return login(studentdef, student_db, student_table);
    } else {
        printf("\033[31m用户不存在！请先注册！\033[0m\n");
        return -1;
    }
}