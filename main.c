#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "main.h"
#include "Function.h"

/* 全局变量定义 */
int attempts = 0;
const int max_attempts = 3;

// 全局数据库变量
VarArray tcr_table, stu_table;
database teacher_base, student_base;

/* 主函数部分 */
int main() {
    char ipt[64];
    
    // 系统初始化
    if (init_system() != 0) {
        printf("\033[31m系统初始化失败！程序退出。\033[0m\n");
        return 1;
    }
    
    printf("\033[32m系统初始化成功！\033[0m\n");
    
    // 主循环
    while (1) {
        print_main_menu();
        supGets(ipt, 64, 3);
        
        if (strcmp(ipt, "1") == 0 || strcmp(ipt, "login") == 0) {
            // 登录
            int login_result = redesigned_login(&teacher_base, &student_base, &tcr_table, &stu_table);
            if(login_result == -1) {
                printf("\033[31m用户不存在！请先注册！\033[0m\n");
            }
        }
        else if (strcmp(ipt, "2") == 0 || strcmp(ipt, "regist") == 0) {
            // 注册
            int reg_type = select_user_type();
            
            if (verification1() == 0 && verification2() == 0) {
                if (reg_type == teacherdef) {
                    redesigned_regist(reg_type, &teacher_base, &tcr_table);
                } else if (reg_type == studentdef) {
                    redesigned_regist(reg_type, &student_base, &stu_table);
                }
            } else {
                printf("\033[31m验证码验证失败，注册失败！\033[0m\n");
                if(attempts >= max_attempts) {
                    printf("\033[31m尝试次数过多，程序退出！\033[0m\n");
                    break;
                }
            }
        }
        else if (strcmp(ipt, "3") == 0 || strcmp(ipt, "stats") == 0) {
            // 系统统计
            system_statistics();
        }
        else if (strcmp(ipt, "4") == 0 || strcmp(ipt, "backup") == 0) {
            // 系统备份
            backup_system();
        }
        else if (strcmp(ipt, "5") == 0 || strcmp(ipt, "help") == 0) {
            // 帮助信息
            print_main_menu();
        }
        else if (strcmp(ipt, "0") == 0 || strcmp(ipt, "quit") == 0) {
            // 退出系统
            printf("\033[35m感谢使用教务管理系统！再见！\033[0m\n");
            break;
        }
        else {
            printf("\033[31m无效输入！请重试。\033[0m\n");
        }
    }
    
    // 系统清理
    cleanup_system();
    return 0;
}