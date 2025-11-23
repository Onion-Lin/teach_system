
#define main.h

/* 结构体定义 */
typedef struct students{
	long long stu_number;
	char name[64];
	int class;
	char username[64];
	int age;
	double mark;
}student;

typedef struct teachers{
	char name[64];
	long long job_number;
	int age;
	char nickname[64];
} teacher;

typedef struct array{
    void *data;      // 存储数据的指针数组
    int index;         // 当前元素索引
    int capacity;     // 当前容量
    int size;    // 每个元素的大小（字节）
} VarArray;

typedef struct databases {
	FILE* data;			//数据库文件流指针
	char name[256];		//数据库名称
	int capacity;		//数据库当前容量
}database;

//define宏定义
#define teacherdef 0
#define studentdef 1



//函数声明
void printhead (void);//打印函数
void len(char inputs[]);// 去除换行符
VarArray ini_VarArray (int ini_length,int size);//可变数组初始化
VarArray* re_VarArray (VarArray*  Array , int length);//可变数组判断，扩容