#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* 头文件引用 */
#include "main.h"
#include "Function.h"

/* 
登录模块：		y打印提示信息	
				y读取————》封装
				检测————————检测类型（分流），检测存在
				验证码平台————》封装		（检测输入格式，检测是否正确）
				次数限制
				//防御模块————》封装
				匹配分流
				遍历数据库开始登录
 */
 
 //注册
 int regist (database){
 
 }
 
 //登录
 int login(database student,database teacher){
	
 }
 
 
 
 //类型检测
 int apart ( void ){
	int inputs;
	while (1){
		printf ("你的登入类型为（1学生，2老师） | >  ");
		supScanf(1,&inputs);
		if (inputs == 1){
			return studentdef;	//学生
		} else if (inputs == 0){
			return teacherdef;	//老师
		} else {
			printf ("Invaid input!\n");
		}
	}
 }
 
 //第一层验证码
 int verification1 (void){		//本体
	//验证码数组定义
	const length = 10;
	char varcode[length];		//验证码
	char answer[length];
	int block = 0;
	//打印验证码
	while (1){
			if (block == 3){
				return -1;
			}
			printf ("\n=============================================\n");
			init_verification1 (char varcode[length],char answer[length],length);
			for (i = 0;i < length;i++ ){
			printf ("%s",varcode[i]);
			}
			printf ("\n=============================================\n");
			//用户输入
			printf ("请输入验证码中非字符部分,输入 re 可以重新生成 ");
			printf ("＞　");
			char input[64];
			supgets(input,sizeof(input),2);
			if (strcmp (input,'re')){
				continue;
			} else {
				if (strcmp (input,answer)){
					printf ("第一层验证码验证成功！\n");
					break;
				} else {
					printf ("第一层验证码验证失败！请重试！\n");
					block++;
					continue;
				}
			}
	}
	return 0;
 }
 
void init_verification1 (char varcode[],char answer[],const length){	//生成
	char useless[] = {'!','@','#','$','%','&','*'};		//无用字符
	srand (time());
	//const length = 10;
	int blueprint[length];		//记录每个位置的内容类型
	//char varcode[length];		//验证码
	//char answer[length];
	for (int i = 0; i < length ; i++){
		blueprint[i] = rand()%3;
	}
	int j = 0;
	for (i = 0;i < length;i++){
		if (blueprint[i] == 0){		//useless
			varcode[i] = useless[rand()%7];
		} else if (blueprint[i] == 1){		//digit
			varcode[i] = 48 + rand()%10;
			answer[j] = varcode[i];
			j++;
		} else if ( blueprint[i] == 2 ){
			varcode[i] = 97 + rand()%27;
			answer[j] = varcode[i];
			j++;
		}
	}
}


//第二层验证码
int verification2 (void){		//本体
//验证码数组定义
	const length = 13;
	char varcode[length];		//验证码
	int block = 0;
	//打印验证码
	while (1){
			if (block == 3){
				return -1;
			}
			printf ("\n=============================================\n");
			int answer = init_verification2 (char varcode[length] , length);
			for (i = 0;i < length;i++ ){
			printf ("%s",varcode[i]);
			}
			printf ("\n=============================================\n");
			//用户输入
			printf ("请输入验证码的结果,输入 10086 可以重新生成 ");
			printf ("＞　");
			int input;
			supScanf(1,&input)
			if (input == 10086){
				continue;
			} else {
				if (input == answer){
					printf ("第二层验证码验证成功！\n");
					break;
				} else {
					printf ("第二层验证码验证失败！请重试！\n");
					continue;
				}
			}
	}
	return 0;
 }

int init_verification2 (char varcode[length],length){ 		//生成验证码：1 + 1 + 1 = 2
	srand (time());
	char symbol[] = {'+','-','*'};
	varcode[1] = ' '，varcode[3] = ' '，varcode[5] = ' '，varcode[7] = ' '，varcode[9] = ' '，varcode[11] = ' ';
	varcode[10] = '=';
	//符号位
	int time[2];
	int i = rand()%3;
	time[0] = i;		//第二位符号
	if ( i == 2){
		varcode[6] = '*';
	} else {
		varcode[6] = symbol[i];
	}
	int i = rand()%3;
	time[1] = i;			//第一位符号
	varcode[2] = symbol[i];
	//数字
	int a = rand()%10;
	int b = rand()%10;
	int c = rand()%10;
	varcode[0] = 48 + a;
	varcode[4] = 48 + b;
	varcode[8] = 48 + c;
	if (time[0] == 3){
		if (time[1] = 0){
			return (b * c + a);
		} else if (time[1] = 1){
			return (b * c - a);
		}if (time[1] = 0){
			return (b * c * a);
		}
	} else if (time[1] == 3){
		if (time[0] = 0){
			return (a * b + c);
		} else if (time[0] = 0){
			return (a * b - c);
		} else if (time[0] = 0){
			return (a * b * c);
		}
	} else {
		return (a + b + c)
	}
}