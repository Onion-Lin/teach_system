#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 头文件引用 */
#include "main.h"
#include "Function.h"

/*教务系统：
	登录模块：	y打印提示信息	
				y读取————》封装
				检测————————检测类型（分流），检测存在
				验证码平台————》封装		（检测输入格式，检测是否正确）
				次数限制
				//防御模块————》封装
				匹配分流
				遍历数据库开始登录
	
	注册模块：	创建数据库文件：学生，老师
				打印提示信息
				读取————》封装
				检测：	检测是否注册过，
						分流
				写入
				校验
	
	检测模块：	指针传参
				检测输入格式
				检测输入内容是否正确
				检测重复
				检测用户分流————》返回值0老师，1学生，-1失败
	
	验证码模块：	创建字符数组存放数字，字母与字符
				生成随机数抽取
				确保不都是非打印字符
				调用检测检测是否正确
				
			
*/

/* 全局变量定义，声明 */


/* 主函数部分 */
int main (){
		//读取字符串定义
		char ipt[64];
		char name[64];
		int TorS;
		//循环主体
		while (1){
			//打印提示信息
			printhelp();
			//读取输入，判断执行那个功能
			fgets (ipt,64,stdin);
			if (strcmp (ipt,"login") == 0){
				//检测分流，返回值存进变量
				TorS = apart();			//（1学生，2老师）
			
				//验证码平台
				if ( verification1() == 0 && verification2() == 0){	//验证成功
					//防御模块 （待定）
				
					//读取分流结果，输出提示信息，读取输入功能，失败自动循环
					//调用功能函数
					if (TorS == teacherdef){
						// 这里是老师的功能

						
						
					}
					else if(TorS == studentdef){
						//这里是学生的功能
						
						
						
					}
				}
			}
			//注册
			else if (strcmp (ipt,"regist") == 0){
				
			
			}
			//安全退出————》保存，检验，（写入）
			else if (strcmp (ipt,"quit")){
				jianyanjianyanjianyan()
				if(return == ok){
				printf ("Thanks for using!See you!\n");
				break;
			}else{
				cuowuchuli()
			}
				//break;
			}
			//错误处理
			else{
				printf ("Invalid input!Please try again!\n");
			}
		}
		
	//free内存，防止泄露
	
	return 0;
}