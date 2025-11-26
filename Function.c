#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "Function.h"

// 全局变量
static int student_counter = 0;
static int teacher_counter = 0;
VarArray tcr_table, stu_table;
database teacher_base, student_base;

// ==================== 系统核心函数 ====================

// 系统初始化
int init_system(void) {
    // 初始化数据库
    tcr_table = ini_VarArray(1024, sizeof(long long));
    if (init_dataBase(&teacher_base, teacherdef, &tcr_table) == 1) {
        printf("\033[31m教师数据库初始化失败！\033[0m\n");
        return 1;
    }
    
    stu_table = ini_VarArray(1024, sizeof(long long));
    if (init_dataBase(&student_base, studentdef, &stu_table) == 1) {
        printf("\033[31m学生数据库初始化失败！\033[0m\n");
        return 1;
    }
    
    // 初始化雨伞系统
    init_umbrella_system();
    
    return 0;
}

// 系统清理
void cleanup_system(void) {
    if(tcr_table.data) free(tcr_table.data);
    if(stu_table.data) free(stu_table.data);
    if(teacher_base.data) fclose(teacher_base.data);
    if(student_base.data) fclose(student_base.data);
}

// ==================== 数据库函数 ====================

// 数据库初始化
int init_dataBase(database* dataBase, int st, VarArray* table) {
    if (st == 1) {
        dataBase->data = fopen(stufile_elif, "rb+");
        if(dataBase->data == NULL) {
            dataBase->data = fopen(stufile_elif, "wb+");
        }
        if (dataBase->data != NULL) {
            strcpy(dataBase->name, stufile_elif);
            dataBase->capacity = 0;
            return 0;
        }
    } else if (st == 0) {
        dataBase->data = fopen(tecfile_elif, "rb+");
        if(dataBase->data == NULL) {
            dataBase->data = fopen(tecfile_elif, "wb+");
        }
        if (dataBase->data != NULL) {
            strcpy(dataBase->name, tecfile_elif);
            dataBase->capacity = 0;
            return 0;
        }
    }
    return 1;
}

// 数据库追加功能
database* add_dataBaseS(database* dataBase, const void* new, int st) {
    if (dataBase) {
        FILE* file = fopen(dataBase->name, "ab");
        if(file != NULL) {
            fwrite(new, st == 1 ? sizeof(student) : sizeof(teacher), 1, file);
            fclose(file);
            dataBase->capacity++;
            return dataBase;
        }
    }
    return NULL;
}

// 数据库保护写入
int bak_dataBase(database* dataBase, const void* new, int st) {
    // 简化实现
    return add_dataBaseS(dataBase, new, st) != NULL ? 1 : -1;
}

// 内核强制写入保护
int sync(FILE* tmpFile) {
    if (fflush(tmpFile) != 0) {
        return -1;
    }
    int file = fileno(tmpFile);
    if (fsync(file) != 0) {
        return -1;
    }
    return 1;
}

// ==================== 用户管理函数 ====================

// 搜索函数
int search(VarArray* table, long long targit) {
    for (int i = 0; i < table->capacity; i++) {
        if (((long long*)table->data)[i] == targit) {
            return i;
        }
    }
    return -1;
}

// 生成学生ID
long long generate_student_id(int class_num) {
    int current_year = 2023;
    long long id = (long long)current_year * 1000000 + (long long)class_num * 1000 + (student_counter % 1000);
    student_counter++;
    return id;
}

// 生成教师ID
long long generate_teacher_id(void) {
    long long id = 1000000000LL + (teacher_counter % 100000000LL);
    teacher_counter++;
    return id;
}

// 检查用户是否存在
int user_exists(int user_type, long long id, VarArray* table) {
    return search(table, id) != -1;
}

// ==================== 验证函数 ====================

// 验证密码格式
int validate_password(char* password) {
    int len = strlen(password);
    if(len < 6 || len > 63) {
        return 0;
    }
    
    int has_digit = 0;
    int has_letter = 0;
    
    for(int i = 0; i < len; i++) {
        char c = password[i];
        if(c >= '0' && c <= '9') {
            has_digit = 1;
        } else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            has_letter = 1;
        } else {
            return 0;
        }
    }
    
    return has_digit && has_letter;
}

// 验证用户名
int validate_username(char* username) {
    int len = strlen(username);
    if(len == 0) return 0;
    
    for(int i = 0; i < len; i++) {
        char c = username[i];
        if(!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')) {
            return 0;
        }
    }
    return 1;
}

// 验证姓名
int validate_name(char* name) {
    int len = strlen(name);
    if(len == 0) return 0;
    
    for(int i = 0; i < len; i++) {
        unsigned char c = name[i];
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            continue;
        }
        if(c >= 0x80) {
            continue;
        }
        return 0;
    }
    return 1;
}

// 验证学号
int validate_student_number(long long stu_number) {
    return (stu_number >= 1000000000LL && stu_number <= 99999999999LL);
}

// 验证年龄
int validate_age(int age) {
    return (age > 0 && age < 150);
}

// 验证成绩
int validate_mark(double mark) {
    return (mark >= 0 && mark <= 100);
}

// 验证输入格式
int validate_input_format(int field_type, void* value) {
    switch(field_type) {
        case 0: return validate_student_number(*(long long*)value);
        case 1: return validate_name((char*)value);
        case 2: return validate_age(*(int*)value);
        case 3: return validate_mark(*(double*)value);
        case 4: return validate_password((char*)value);
        default: return 0;
    }
}

// ==================== 登录注册函数 ====================

// 选择用户类型
int select_user_type(void) {
    int choice;
    while(1) {
        printf("\033[33m请选择用户类型：\033[0m\n");
        printf("\033[36m1. 学生\033[0m\n");
        printf("\033[36m0. 教师\033[0m\n");
        printf("\033[33m请输入选择 (0或1): \033[0m");
        
        if(scanf("%d", &choice) == 1 && (choice == 0 || choice == 1)) {
            clear_input_buffer();
            return choice;
        } else {
            printf("\033[31m输入错误！请输入0（教师）或1（学生）\033[0m\n");
            clear_input_buffer();
        }
    }
}

// 重新设计的注册函数
int redesigned_regist(int user_type, database* user_base, VarArray* table) {
    if(user_type == teacherdef) {
        teacher tcrNew;
        
        printf("\033[33m请输入用户名（仅支持字母、数字和下划线）：\033[0m");
        supGets(tcrNew.nickname, 64, 3);
        
        printf("\033[33m请输入姓名（仅支持中文、英文字母）：\033[0m");
        supGets(tcrNew.name, 64, 3);
        
        tcrNew.job_number = generate_teacher_id();
        printf("\033[33m系统为您分配的工号是：%lld\033[0m\n", tcrNew.job_number);
        
        printf("\033[33m请输入年龄（1-149之间的正整数）：\033[0m");
        supScanf(1, &tcrNew.age);
        
        // 密码输入和验证
        while(1) {
            printf("\033[33m请输入密码（6-63位，必须包含数字和字母）：\033[0m");
            char password[64];
            supGets(password, 64, 3);
            
            if(validate_password(password)) {
                strcpy(tcrNew.password, password);
                break;
            } else {
                printf("\033[31m密码格式错误！密码必须为6-63位，且包含数字和字母\033[0m\n");
            }
        }
        
        if(search(table, tcrNew.job_number) != -1) {
            printf("\033[31m工号已存在，注册失败！\033[0m\n");
            return 0;
        }
        
        add_dataBaseS(user_base, &tcrNew, teacherdef);
        table->data = realloc(table->data, (table->capacity + 1) * sizeof(long long));
        ((long long*)table->data)[table->capacity] = tcrNew.job_number;
        table->capacity++;
        printf("\033[32m教师注册成功！\033[0m\n");
        return 1;
        
    } else if(user_type == studentdef) {
        student stuNew;
        
        printf("\033[33m请输入用户名（仅支持字母、数字和下划线）：\033[0m");
        supGets(stuNew.username, 64, 3);
        
        printf("\033[33m请输入姓名（仅支持中文、英文字母）：\033[0m");
        supGets(stuNew.name, 64, 3);
        
        printf("\033[33m请输入班级号（正整数）：\033[0m");
        supScanf(1, &stuNew.class);
        
        stuNew.stu_number = generate_student_id(stuNew.class);
        printf("\033[33m系统为您分配的学号是：%lld\033[0m\n", stuNew.stu_number);
        
        printf("\033[33m请输入年龄（1-99之间的正整数）：\033[0m");
        supScanf(1, &stuNew.age);
        
        printf("\033[33m请输入成绩（0-100之间的数字）：\033[0m");
        supScanf(2, &stuNew.mark);
        
        // 密码输入和验证
        while(1) {
            printf("\033[33m请输入密码（6-63位，必须包含数字和字母）：\033[0m");
            char password[64];
            supGets(password, 64, 3);
            
            if(validate_password(password)) {
                strcpy(stuNew.password, password);
                break;
            } else {
                printf("\033[31m密码格式错误！密码必须为6-63位，且包含数字和字母\033[0m\n");
            }
        }
        
        if(search(table, stuNew.stu_number) != -1) {
            printf("\033[31m学号已存在，注册失败！\033[0m\n");
            return 0;
        }
        
        add_dataBaseS(user_base, &stuNew, studentdef);
        table->data = realloc(table->data, (table->capacity + 1) * sizeof(long long));
        ((long long*)table->data)[table->capacity] = stuNew.stu_number;
        table->capacity++;
        printf("\033[32m学生注册成功！\033[0m\n");
        return 1;
    }
    return 0;
}

// 显示用户信息
void display_user_info(int user_type, void* user_data) {
    if(user_type == teacherdef) {
        teacher* tcr = (teacher*)user_data;
        printf("\n\033[34m=== 教师信息 ===\033[0m\n");
        printf("\033[36m姓名: %s\033[0m\n", tcr->name);
        printf("\033[36m工号: %lld\033[0m\n", tcr->job_number);
        printf("\033[36m年龄: %d\033[0m\n", tcr->age);
        printf("\033[36m昵称: %s\033[0m\n", tcr->nickname);
        printf("\033[34m================\033[0m\n");
    } else {
        student* stu = (student*)user_data;
        printf("\n\033[34m=== 学生信息 ===\033[0m\n");
        printf("\033[36m姓名: %s\033[0m\n", stu->name);
        printf("\033[36m学号: %lld\033[0m\n", stu->stu_number);
        printf("\033[36m班级: %d\033[0m\n", stu->class);
        printf("\033[36m用户名: %s\033[0m\n", stu->username);
        printf("\033[36m年龄: %d\033[0m\n", stu->age);
        printf("\033[36m成绩: %.2f\033[0m\n", stu->mark);
        printf("\033[34m================\033[0m\n");
    }
}

// ==================== 菜单和界面函数 ====================

// 打印主菜单
void print_main_menu(void) {
    printf("\n\033[36m═══════════════════════════════════════════\033[0m\n");
    printf("\033[33m           教务管理系统 v2.0\033[0m\n");
    printf("\033[36m═══════════════════════════════════════════\033[0m\n");
    printf("\033[32m1. 登录系统 (login)\033[0m\n");
    printf("\033[32m2. 用户注册 (regist)\033[0m\n");
    printf("\033[32m3. 系统统计 (stats)\033[0m\n");
    printf("\033[32m4. 系统备份 (backup)\033[0m\n");
    printf("\033[32m5. 帮助信息 (help)\033[0m\n");
    printf("\033[31m0. 退出系统 (quit)\033[0m\n");
    printf("\033[36m═══════════════════════════════════════════\033[0m\n");
    printf("\033[33m请选择操作: \033[0m");
}

// 打印教师菜单
void print_teacher_menu(void) {
    printf("\n\033[34m═══════════════════════════════════════════\033[0m\n");
    printf("\033[33m               教师工作台\033[0m\n");
    printf("\033[34m═══════════════════════════════════════════\033[0m\n");
    printf("\033[32m1. 学生管理\033[0m\n");
    printf("\033[32m2. 成绩管理\033[0m\n");
    printf("\033[32m3. 邮件系统\033[0m\n");
    printf("\033[32m4. 雨伞管理\033[0m\n");
    printf("\033[32m5. 个人信息\033[0m\n");
    printf("\033[31m0. 退出登录\033[0m\n");
    printf("\033[34m═══════════════════════════════════════════\033[0m\n");
    printf("\033[33m请选择操作: \033[0m");
}

// 打印学生菜单
void print_student_menu(void) {
    printf("\n\033[35m═══════════════════════════════════════════\033[0m\n");
    printf("\033[33m               学生工作台\033[0m\n");
    printf("\033[35m═══════════════════════════════════════════\033[0m\n");
    printf("\033[32m1. 查看成绩\033[0m\n");
    printf("\033[32m2. 查看邮件\033[0m\n");
    printf("\033[32m3. 雨伞借还\033[0m\n");
    printf("\033[32m4. 个人信息\033[0m\n");
    printf("\033[31m0. 退出登录\033[0m\n");
    printf("\033[35m═══════════════════════════════════════════\033[0m\n");
    printf("\033[33m请选择操作: \033[0m");
}

// 教师界面
void teacher_interface(teacher* tcr, database* stu_db, database* tcr_db) {
    char command[64];
    
    // 检查新邮件和雨伞状态
    check_umbrella_status(tcr->job_number);
    
    while(1) {
        print_teacher_menu();
        supGets(command, 64, 3);
        
        if(strcmp(command, "1") == 0) {
            student_management_menu(tcr, stu_db);
        } 
        else if(strcmp(command, "2") == 0) {
            grade_management_menu(tcr, stu_db);
        }
        else if(strcmp(command, "3") == 0) {
            email_management_menu(tcr, stu_db);
        }
        else if(strcmp(command, "4") == 0) {
            umbrella_module(tcr->job_number, teacherdef);
        }
        else if(strcmp(command, "5") == 0) {
            display_user_info(teacherdef, tcr);
        }
        else if(strcmp(command, "0") == 0) {
            printf("\033[35m退出教师界面\033[0m\n");
            break;
        }
        else {
            printf("\033[31m无效命令，请重试\033[0m\n");
        }
    }
}

// 学生界面
void student_interface(student* stu, database* stu_db, database* tcr_db) {
    char command[64];
    
    // 检查新邮件和雨伞状态
    check_emails(stu->stu_number, studentdef);
    check_umbrella_status(stu->stu_number);
    
    while(1) {
        print_student_menu();
        supGets(command, 64, 3);
        
        if(strcmp(command, "1") == 0) {
            view_grades(stu->stu_number);
        }
        else if(strcmp(command, "2") == 0) {
            check_emails(stu->stu_number, studentdef);
        }
        else if(strcmp(command, "3") == 0) {
            umbrella_module(stu->stu_number, studentdef);
        }
        else if(strcmp(command, "4") == 0) {
            display_user_info(studentdef, stu);
        }
        else if(strcmp(command, "0") == 0) {
            printf("\033[35m退出学生界面\033[0m\n");
            break;
        }
        else {
            printf("\033[31m无效命令，请重试\033[0m\n");
        }
    }
}

// 学生管理菜单
void student_management_menu(teacher* tcr, database* stu_db) {
    char choice[64];
    
    while(1) {
        printf("\n\033[34m────── 学生管理 ──────\033[0m\n");
        printf("\033[32m1. 列出所有学生\033[0m\n");
        printf("\033[32m2. 查询学生信息\033[0m\n");
        printf("\033[32m3. 按班级查看\033[0m\n");
        printf("\033[31m0. 返回上级\033[0m\n");
        printf("\033[33m请选择: \033[0m");
        
        supGets(choice, 64, 3);
        
        if(strcmp(choice, "1") == 0) {
            list_users(studentdef);
        }
        else if(strcmp(choice, "2") == 0) {
            student_query_module(stu_db);
        }
        else if(strcmp(choice, "3") == 0) {
            int class_num;
            printf("\033[33m请输入班级号: \033[0m");
            supScanf(1, &class_num);
            view_class_students(class_num);
        }
        else if(strcmp(choice, "0") == 0) {
            break;
        }
        else {
            printf("\033[31m无效选择\033[0m\n");
        }
    }
}

// 成绩管理菜单
void grade_management_menu(teacher* tcr, database* stu_db) {
    char choice[64];
    
    while(1) {
        printf("\n\033[34m────── 成绩管理 ──────\033[0m\n");
        printf("\033[32m1. 录入成绩\033[0m\n");
        printf("\033[32m2. 修改成绩\033[0m\n");
        printf("\033[32m3. 查看班级成绩\033[0m\n");
        printf("\033[32m4. 查看学生成绩\033[0m\n");
        printf("\033[31m0. 返回上级\033[0m\n");
        printf("\033[33m请选择: \033[0m");
        
        supGets(choice, 64, 3);
        
        if(strcmp(choice, "1") == 0) {
            grade_module(tcr, stu_db);
        }
        else if(strcmp(choice, "2") == 0) {
            modify_grade_menu(tcr);
        }
        else if(strcmp(choice, "3") == 0) {
            int class_num;
            printf("\033[33m请输入班级号: \033[0m");
            supScanf(1, &class_num);
            view_class_grades(tcr, class_num);
        }
        else if(strcmp(choice, "4") == 0) {
            long long student_id;
            printf("\033[33m请输入学号: \033[0m");
            supScanf(1, &student_id);
            view_grades(student_id);
        }
        else if(strcmp(choice, "0") == 0) {
            break;
        }
        else {
            printf("\033[31m无效选择\033[0m\n");
        }
    }
}

// 邮件管理菜单
void email_management_menu(teacher* tcr, database* stu_db) {
    char choice[64];
    
    while(1) {
        printf("\n\033[34m────── 邮件系统 ──────\033[0m\n");
        printf("\033[32m1. 发送班级邮件\033[0m\n");
        printf("\033[32m2. 发送个人邮件\033[0m\n");
        printf("\033[32m3. 查看已发送邮件\033[0m\n");
        printf("\033[31m0. 返回上级\033[0m\n");
        printf("\033[33m请选择: \033[0m");
        
        supGets(choice, 64, 3);
        
        if(strcmp(choice, "1") == 0) {
            send_class_email_menu(tcr);
        }
        else if(strcmp(choice, "2") == 0) {
            send_individual_email_menu(tcr);
        }
        else if(strcmp(choice, "3") == 0) {
            view_sent_emails(tcr->job_number);
        }
        else if(strcmp(choice, "0") == 0) {
            break;
        }
        else {
            printf("\033[31m无效选择\033[0m\n");
        }
    }
}

// 发送班级邮件菜单
void send_class_email_menu(teacher* tcr) {
    long long class_num;
    char content[MAX_CONTENT_LEN];
    
    printf("\033[33m请输入目标班级: \033[0m");
    supScanf(1, &class_num);
    
    printf("\033[33m请输入邮件内容: \033[0m");
    supGets(content, MAX_CONTENT_LEN, 3);
    
    send_email_to_class(tcr, class_num, content);
}

// 发送个人邮件菜单
void send_individual_email_menu(teacher* tcr) {
    long long student_id;
    char content[MAX_CONTENT_LEN];
    
    printf("\033[33m请输入学生学号: \033[0m");
    supScanf(1, &student_id);
    
    printf("\033[33m请输入邮件内容: \033[0m");
    supGets(content, MAX_CONTENT_LEN, 3);
    
    send_email_to_student(tcr, student_id, content);
}

// 修改成绩菜单
void modify_grade_menu(teacher* tcr) {
    long long student_id;
    char exam_name[64];
    double new_score;
    
    printf("\033[33m请输入学生学号: \033[0m");
    supScanf(1, &student_id);
    
    printf("\033[33m请输入考试名称: \033[0m");
    supGets(exam_name, 64, 3);
    
    printf("\033[33m请输入新分数: \033[0m");
    supScanf(2, &new_score);
    
    modify_grade_record(tcr, student_id, exam_name, new_score);
}

// 查看班级学生
void view_class_students(int class_num) {
    FILE* file = fopen(stufile_elif, "rb");
    if(file == NULL) {
        printf("\033[31m无法打开学生数据库\033[0m\n");
        return;
    }
    
    student stu;
    int found = 0;
    
    printf("\n\033[34m班级 %d 的学生列表：\033[0m\n", class_num);
    printf("\033[36m学号\t\t姓名\t\t年龄\033[0m\n");
    
    while(fread(&stu, sizeof(student), 1, file) == 1) {
        if(stu.class == class_num) {
            printf("\033[37m%lld\t%s\t%d\033[0m\n", stu.stu_number, stu.name, stu.age);
            found = 1;
        }
    }
    
    if(!found) {
        printf("\033[31m未找到该班级的学生\033[0m\n");
    }
    
    fclose(file);
}

// ==================== 邮件系统函数 ====================

void email_module(teacher* tcr, database* stu_db) {
    // 调用邮件管理菜单
    email_management_menu(tcr, stu_db);
}

void check_emails(long long user_id, int user_type) {
    // 简化实现 - 检查邮件
    printf("\033[33m检查邮件功能（待实现）\033[0m\n");
}

void send_email_to_class(teacher* tcr, long long target_class, char* content) {
    // 简化实现 - 发送班级邮件
    printf("\033[32m已向班级 %lld 发送邮件\033[0m\n", target_class);
}

void send_email_to_student(teacher* tcr, long long student_id, char* content) {
    // 简化实现 - 发送个人邮件
    printf("\033[32m已向学生 %lld 发送邮件\033[0m\n", student_id);
}

void view_sent_emails(long long teacher_id) {
    // 简化实现 - 查看已发送邮件
    printf("\033[33m查看已发送邮件功能（待实现）\033[0m\n");
}

// ==================== 成绩系统函数 ====================

void grade_module(teacher* tcr, database* stu_db) {
    // 简化实现 - 成绩模块
    printf("\033[33m成绩管理功能（待实现）\033[0m\n");
}

void view_grades(long long student_id) {
    // 简化实现 - 查看成绩
    printf("\033[33m查看成绩功能（待实现）\033[0m\n");
}

void add_grade_record(teacher* tcr, long long student_id, char* exam_name, double score) {
    // 简化实现 - 添加成绩记录
}

void modify_grade_record(teacher* tcr, long long student_id, char* exam_name, double new_score) {
    // 简化实现 - 修改成绩记录
    printf("\033[32m已修改学生 %lld 的成绩\033[0m\n", student_id);
}

void view_class_grades(teacher* tcr, int class_num) {
    // 简化实现 - 查看班级成绩
    printf("\033[33m查看班级 %d 成绩功能（待实现）\033[0m\n", class_num);
}

// ==================== 雨伞系统函数 ====================

void umbrella_module(long long user_id, int user_type) {
    int choice;
    printf("\033[33m1. 借雨伞\n2. 还雨伞\n请选择: \033[0m");
    supScanf(1, &choice);
    
    if(choice == 1) {
        borrow_umbrella(user_id);
    } else if(choice == 2) {
        return_umbrella(user_id);
    } else {
        printf("\033[31m无效选择\033[0m\n");
    }
}

void init_umbrella_system(void) {
    FILE* file = fopen(umbrellafile, "rb");
    if(file == NULL) {
        file = fopen(umbrellafile, "wb");
        if(file != NULL) {
            umbrella umb;
            for(int i = 1; i <= 20; i++) {
                umb.id = i;
                umb.borrowed = 0;
                umb.borrower_id = 0;
                fwrite(&umb, sizeof(umbrella), 1, file);
            }
            fclose(file);
        }
    } else {
        fclose(file);
    }
}

void check_umbrella_status(long long user_id) {
    // 简化实现 - 检查雨伞状态
}

void borrow_umbrella(long long user_id) {
    // 简化实现 - 借雨伞
    printf("\033[32m借雨伞功能（待实现）\033[0m\n");
}

void return_umbrella(long long user_id) {
    // 简化实现 - 还雨伞
    printf("\033[32m还雨伞功能（待实现）\033[0m\n");
}

void view_umbrella_status(void) {
    // 简化实现 - 查看雨伞状态
}

// ==================== 工具函数 ====================

// 超级读取
int supGets(char inputs[], int size, int mode) {
    if(fgets(inputs, size, stdin) == NULL) {
        return -1;
    }
    
    int len = strlen(inputs);
    if(len > 0 && inputs[len-1] == '\n') {
        inputs[len-1] = '\0';
    }
    return 0;
}

// 增强scanf
int supScanf(int mode, void* targit) {
    if(mode == 1) {
        if(scanf("%d", (int*)targit) == 1) {
            clear_input_buffer();
            return 0;
        }
    } else if(mode == 2) {
        if(scanf("%lf", (double*)targit) == 1) {
            clear_input_buffer();
            return 0;
        }
    }
    clear_input_buffer();
    return -1;
}

// 清除输入缓冲区
void clear_input_buffer(void) {
    while(getchar() != '\n');
}

// 去除换行符
void len(char inputs[]) {
    // 已在上面的supGets中处理
}

// 可变数组初始化
VarArray ini_VarArray(int ini_length, int size) {
    VarArray array;
    array.data = malloc(ini_length * size);
    if(array.data != NULL) {
        array.capacity = ini_length;
        array.size = size;
        array.index = 0;
    } else {
        array.capacity = 0;
        array.size = 0;
        array.index = 0;
    }
    return array;
}

// 可变数组扩容
VarArray* re_VarArray(VarArray* Array, int length) {
    if(Array->capacity < length) {
        void* tmp = realloc(Array->data, Array->size * length);
        if(tmp != NULL) {
            Array->data = tmp;
            Array->capacity = length;
        }
    }
    return Array;
}

// ==================== 验证码函数 ====================

//第一层验证码
int verification1(void) {
    const int length = 10;
    char varcode[length+1];
    char answer[11];
    int block = 0;
    
    while (1) {
        if (block == 3) {
            return -1;
        }
        
        printf("\n\033[36m=============================================\033[0m\n");
        init_verification1(varcode, answer, length);
        printf("\033[36m验证码：--%s\033[0m\n", varcode);
        printf("\033[36m=============================================\033[0m\n");
        
        printf("\033[33m请输入验证码中非字符部分: \033[0m");
        char input[64];
        if(scanf("%s", input) != 1) {
            printf("\033[31m输入错误！\033[0m\n");
            continue;
        }
        clear_input_buffer();
        
        if(strcmp(input, answer) == 0) {
            printf("\033[32m第一层验证码验证成功！\033[0m\n");
            return 0;
        } else {
            printf("\033[31m第一层验证码验证失败！请重试！\033[0m\n");
            block++;
        }
    }
}

void init_verification1(char varcode[], char answer[], const int length) {
    char useless[] = {'!','@','#','$','%','&','*','(',')','_','+','=','[',']','{','}','|','\\',':',';','"','\'','<','>','?','/','~','`'};
    srand((unsigned int)time(NULL));
    
    int blueprint[length];
    for(int i = 0; i < length; i++) {
        blueprint[i] = rand() % 3;
    }
    
    int j = 0;
    for(int i = 0; i < length; i++) {
        if(blueprint[i] == 0) {
            varcode[i] = useless[rand() % (sizeof(useless))];
        } else if(blueprint[i] == 1) {
            varcode[i] = '0' + rand() % 10;
            answer[j] = varcode[i];
            j++;
        } else if(blueprint[i] == 2) {
            varcode[i] = 'a' + rand() % 26;
            answer[j] = varcode[i];
            j++;
        }
    }
    varcode[length] = '\0';
    answer[j] = '\0';
}

//第二层验证码
int verification2(void) {
    const int length = 13;
    char varcode[length+1];
    int block = 0;
    
    while(1) {
        if(block == 3) {
            return -1;
        }
        
        printf("\n\033[36m=============================================\033[0m\n");
        int answer = init_verification2(varcode, length);
        printf("\033[36m%s\033[0m\n", varcode);
        printf("\033[36m=============================================\033[0m\n");
        
        printf("\033[33m请输入验证码的结果: \033[0m");
        int input;
        if(scanf("%d", &input) != 1) {
            printf("\033[31m输入错误！\033[0m\n");
            continue;
        }
        clear_input_buffer();
        
        if(input == answer) {
            printf("\033[32m第二层验证码验证成功！\033[0m\n");
            return 0;
        } else {
            printf("\033[31m第二层验证码验证失败！请重试！\033[0m\n");
            block++;
        }
    }
}

int init_verification2(char varcode[], int length) {
    srand((unsigned int)time(NULL));
    char symbol[] = {'+','-','*'};
    
    varcode[1] = ' ';
    varcode[3] = ' ';
    varcode[5] = ' ';
    varcode[7] = ' ';
    varcode[9] = ' ';
    varcode[11] = ' ';
    varcode[10] = '=';
    varcode[12] = '\0';
    
    int time[2];
    int i = rand() % 3;
    time[0] = i;
    varcode[6] = symbol[i];
    
    int j = rand() % 3;
    time[1] = j;
    varcode[2] = symbol[j];
    
    int a = rand() % 10;
    int b = rand() % 10;
    int c = rand() % 10;
    varcode[0] = '0' + a;
    varcode[4] = '0' + b;
    varcode[8] = '0' + c;
    
    if(time[0] == 2) {
        if(time[1] == 0) return (a + b * c);
        else if(time[1] == 1) return (a - b * c);
        else return (a * b * c);
    } else if(time[1] == 2) {
        if(time[0] == 0) return (a * b + c);
        else if(time[0] == 1) return (a * b - c);
        else return (a * b * c);
    } else {
        if(time[0] == 0 && time[1] == 0) return (a + b + c);
        else if(time[0] == 0 && time[1] == 1) return (a - b + c);
        else if(time[0] == 1 && time[1] == 0) return (a + b - c);
        else return (a - b - c);
    }
}

// ==================== 系统统计函数 ====================

// 系统统计
void system_statistics(void) {
    printf("\n\033[34m════════════ 系统统计 ════════════\033[0m\n");
    
    // 统计教师数量
    FILE* tcr_file = fopen(tecfile_elif, "rb");
    int teacher_count = 0;
    if(tcr_file != NULL) {
        teacher tcr;
        while(fread(&tcr, sizeof(teacher), 1, tcr_file) == 1) {
            teacher_count++;
        }
        fclose(tcr_file);
    }
    
    // 统计学生数量
    FILE* stu_file = fopen(stufile_elif, "rb");
    int student_count = 0;
    if(stu_file != NULL) {
        student stu;
        while(fread(&stu, sizeof(student), 1, stu_file) == 1) {
            student_count++;
        }
        fclose(stu_file);
    }
    
    printf("\033[36m教师数量: %d\033[0m\n", teacher_count);
    printf("\033[36m学生数量: %d\033[0m\n", student_count);
    printf("\033[34m════════════════════════════════════\033[0m\n");
}

// 系统备份
void backup_system(void) {
    printf("\n\033[33m开始系统备份...\033[0m\n");
    printf("\033[32m系统备份功能（待实现）\033[0m\n");
}

// 系统恢复
void restore_system(void) {
    // 简化实现
}

// ==================== 其他功能函数 ====================

// 列出用户
void list_users(int user_type) {
    if(user_type == studentdef) {
        printf("\033[33m列出所有学生功能（待实现）\033[0m\n");
    } else {
        printf("\033[33m列出所有教师功能（待实现）\033[0m\n");
    }
}

// 学生查询模块
void student_query_module(database* stu_db) {
    printf("\033[33m学生查询模块功能（待实现）\033[0m\n");
}

// 注册函数
int regist(int mode, database* user_base, VarArray* table) {
    return redesigned_regist(mode, user_base, table);
}