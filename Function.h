#ifndef FUNCTION_H
#define FUNCTION_H

#include "main.h"

// 系统初始化函数
int init_system(void);
void cleanup_system(void);

// 数据库管理函数
int init_dataBase(database* dataBase, int st, VarArray* table);
database* add_dataBaseS(database* dataBase, const void* new, int st);
int bak_dataBase(database* dataBase, const void* new, int st);
int sync(FILE* tmpFile);

// 用户管理函数
int search(VarArray* table, long long targit);
long long generate_student_id(int class_num);
long long generate_teacher_id(void);
int user_exists(int user_type, long long id, VarArray* table);

// 验证函数
int validate_username(char* username);
int validate_name(char* name);
int validate_student_number(long long stu_number);
int validate_age(int age);
int validate_mark(double mark);
int validate_password(char* password);
int validate_input_format(int field_type, void* value);

// 登录注册函数
int login(int user_type, database* user_base, VarArray* table);
int redesigned_login(database* teacher_db, database* student_db, 
                    VarArray* teacher_table, VarArray* student_table);
int redesigned_regist(int user_type, database* user_base, VarArray* table);
int regist(int mode, database* user_base, VarArray* table);
int select_user_type(void);
void display_user_info(int user_type, void* user_data);
int verify_password(void* user, int user_type);

// 用户界面函数
void teacher_interface(teacher* tcr, database* stu_db, database* tcr_db);
void student_interface(student* stu, database* stu_db, database* tcr_db);
void list_users(int user_type);
void student_query_module(database* stu_db);

// 菜单函数
void student_management_menu(teacher* tcr, database* stu_db);
void grade_management_menu(teacher* tcr, database* stu_db);
void email_management_menu(teacher* tcr, database* stu_db);
void send_class_email_menu(teacher* tcr);
void send_individual_email_menu(teacher* tcr);
void modify_grade_menu(teacher* tcr);
void view_class_students(int class_num);

// 邮件系统函数
void email_module(teacher* tcr, database* stu_db);
void check_emails(long long user_id, int user_type);
void send_email_to_class(teacher* tcr, long long target_class, char* content);
void send_email_to_student(teacher* tcr, long long student_id, char* content);
void view_sent_emails(long long teacher_id);

// 成绩系统函数
void grade_module(teacher* tcr, database* stu_db);
void view_grades(long long student_id);
void add_grade_record(teacher* tcr, long long student_id, char* exam_name, double score);
void modify_grade_record(teacher* tcr, long long student_id, char* exam_name, double new_score);
void view_class_grades(teacher* tcr, int class_num);

// 雨伞系统函数
void umbrella_module(long long user_id, int user_type);
void init_umbrella_system(void);
void check_umbrella_status(long long user_id);
void borrow_umbrella(long long user_id);
void return_umbrella(long long user_id);
void view_umbrella_status(void);

// 工具函数
int supScanf(int mode, void* targit);
int supGets(char inputs[], int size, int mode);
void len(char inputs[]);
void clear_input_buffer(void);

// 验证码函数
int verification1(void);
void init_verification1(char varcode[], char answer[], const int length);
int verification2(void);
int init_verification2(char varcode[], int length);

// 系统统计函数
void system_statistics(void);
void backup_system(void);
void restore_system(void);

#endif