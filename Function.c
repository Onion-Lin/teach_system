#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>   
#include <errno.h> 
#include <fcntl.h>
#include <sys/stat.h>


/* 头文件引用 */
#include "main.h"
#include "Function.h"

//打印函数
void printhead (void){
	printf ("\n=============================================\n");
	printf ("Welcome! Please type in comanneds!\n");
	printf ("type in add or A to add\n");
	printf ("type search or S to search\n");
	printf ("type logIn or l to Logging in\n");
	printf ("type help to print this again\n");
	printf ("type quit to leave\n");
	printf ("=============================================\n\n");
}

// 超级读取
int supGets(char inputs[] , int size, int mode){	//功能
	while(1){
		fgets(inputs, size , stdin);
		int len = strlen(inputs);
		if ((len < size || inputs[size-1] == '\n') && len != 0 ) {
			inputs[len] = '\0';		//去除回车
			
			if (mode == 1){				//功能1——输出为数字
				for ( int i = 0; isdigit (inputs[i]) ; i++ ){
					if (i+1 == len){
						return 0;
					}
				}
				printf ("大傻春你在干什么!\n你小汁，再乱输有你好果汁吃！\n");
				while (getchar() != '\n');
			}  else if (mode == 2){		//功能2——输出为数字和字母
				for ( int i = 0; isalnum (inputs[i]) ; i++ ){
					if (i+1 == len){
						return 0;
					}
				}
				printf ("大傻春你在干什么!\n你小汁，再乱输有你好果汁吃！\n");
				while (getchar() != '\n');
			}else if (mode == 3){		//功能2——输出字母
				for ( int i = 0; isalpha (inputs[i]) ; i++ ){
					if (i+1 == len){
						return 0;
					}
				}
				printf ("大傻春你在干什么!\n你小汁，再乱输有你好果汁吃！\n");
				while (getchar() != '\n');
			}
		} else {
			printf ("大傻春你在干什么!\n你小汁，再乱输有你好果汁吃！\n");
			while (getchar() != '\n');
		}
	}
}


//增强scanf
int supScanf(int mode,void* targit){
	while (1){
		if (mode == 1){		//1为整数
			scanf ("%d",targit);
			if (getchar() == '\n'){
				return 0;
			}else{
				printf ("大傻春你在干什么!\n你小汁，再乱输有你好果汁吃！\n");
				while (getchar() != '\n');
			}
		}
		else if (mode == 2){		//2为小数
			scanf ("%lf",targit);
			if (getchar() == '\n'){
				return 0;
			}else{
				printf ("大傻春你在干什么!\n你小汁，再乱输有你好果汁吃！\n");
				while (getchar() != '\n');
			}
		}
	}
}

//可变数组初始化
VarArray ini_VarArray (int ini_length,int size){
	void* temp = NULL;
	VarArray Array;
	temp = malloc(size * ini_length);
	if (temp != NULL){
		Array.data = temp;
		Array.capacity = ini_length;
		Array.size = size;
		Array.index = 0;
		return array;
	}
}

//可变数组判断，扩容
VarArray* re_VarArray (VarArray*  Array , int length){
	if ( Array->capacity < length ){
		void* tmp = NULL;
		tmp = realloc (Array,Array->size * length);
		if (tmp != NULL){
			Array = tmp;
			return Array;
		} else {
			printf ("扩容失败，请检查！\n");
			return Array;
		}
	}
}

//数据库初始化
database* init_dataBase(char name[],FILE* dataBase){
	database* tmp = NULL;
	tmp = malloc( sizeof(database) );
	if (tmp != NULL){
		//FILE* dataBase = fopen ( file_elif ,"ab+");
		if ( dataBase =  NULL){
			printf ("数据库读取失败！请检查后重试！\n");
			return NULL;
		} else {
			strcpy (tmp->name,name);
			tmp->data = dataBase;
			tmp->capacity = 0;
			return tmp;
		}
	}
}


//数据库追加功能
database* add_dataBaseS( database* dataBase , const void* new , int st){		//st:1-->stu,0-->tcr
	if (dataBase){
		printf ("正在向%s中添加数据……\n",dataBase->name);
				int tmp = bak_dataBase( dataBase , new ,st);
				if (tmp == 1){
					printf ("写入成功！\n");
					return dataBase;
				} else {
					printf ("写入失败，数据库已还原！\n请检查后重试！\n");
					return dataBase;
				}
				
			} else {
				return NULL;
			}	
}



//数据库保护写入函数
int bak_dataBase ( database* dataBase ,const void* new , int st){		//st:1-->stu,0-->tcr
	if (dataBase->capacity != 0){
		//备份文件
		FILE* backup = fopen (filebak,"ab+");
		if (st == 1){ 
			size_t ST = sizeof(student);
			student stu;
			while (fread (stu , ST , 1 , dataBase->data) == 1){
				fwrite (stu,ST,1,backup);
			}
		}else if (st == 0){
			size_t ST = sizeof(teacher);
			teacher tcr;
			while (fread (tcr , ST , 1 , dataBase->data) == 1){
				fwrite (tcr,ST,1,backup);
			}
		}
		//写入
		size_t temp = fwrite (new,ST,1,dataBase->data);
		//检测写入是否成功
		if (temp == 1){
			dataBase->capacity++;
			sync(dataBase->data);
			fclose (backup);
			remove(filebak); // 删除旧备份
			return 1;
		} else {
			//失败回滚
			remove (file_elif);
			rename (filebak,file_elif);
			return -1;
		}
	}

}



//内核强制写入保护
int sync(FILE* tmpFile){
	if ( fflush(tmpFile) != 0 ){	//清空 stdio 缓冲区,送入操作系统内核
		printf ("操作系统内核同步失败！%s!\n请稍后重试！\n",strerror(errno));
		return -1;
	}
	int file = fileno(tmpFile);
	if (fsync(file) != 0){			//强制内核将数据刷到磁盘
		printf ("写入失败！%s!\n请稍后重试！\n",strerror(errno));
		return -1;
	}
	return 1;
}


//搜索函数
int search(database* dataBase , ){
	
}
