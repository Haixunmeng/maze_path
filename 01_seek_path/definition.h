#define Up 24
#define Down 25
#define Left 27
#define Right 26
/*********************************定义栈**********************************/
/***********定义栈元素类型***********/
typedef struct stack	
{
	int x;				    //当前的坐标
	int y;
	int dir;				//当前位置的前进方向 
	struct stack *next;		//指向下一栈元素
	struct stack *front;	//指向上一节点
}Stack, stack_elem;
 
 
/**********定义对栈的操作*************/ 
int isEmpty(Stack *stack);								// 判断栈是否为空 
Stack *pop(Stack *stack);								// 出栈函数,返回栈顶指针		 
Stack *push(Stack *stack, int x, int y, int dir);		// 进栈函数，返回栈顶指针		 
stack_elem get_top_elem(Stack *stack);					// 获取栈顶元素，返回栈顶元素	
int isWay(Stack *stack, int x, int y);					// 查找栈中是否包含该位置 
void print_stack(Stack *stack);						    //从栈底到栈顶输出全部栈元素
Stack *empty_stack(Stack *stack);						//清空链表
/**************************************************************************/

/*************定义迷宫***************/

#define SIZE_X_MAX  100						//定义迷宫的最大规模
#define SIZE_Y_MAX  100

int size_x;						//迷宫的规模
int size_y;

int entrance_x;					//定义迷宫的入口 
int entrance_y;

int out_x;						//定义迷宫的出口 
int out_y; 

int way_count = 0;															//记录通路的数量 
void creat_maze_random(int maze[SIZE_X_MAX][SIZE_Y_MAX]);								//随机创建迷宫
void seek_path(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y);							//非递归方式找出路 
void seek_path_count(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y);
void print_maze_dynamic(int maze[SIZE_X_MAX][SIZE_Y_MAX], Stack *stack, Stack *step);				    //动态打印迷宫通路
void print_way(Stack *stack);													//输出迷宫的字符通路 
void delay(int n);															//延时
void save_path(Stack *stack, FILE *p);													//将路径保存到文件中
int is_size_illegal(int x, int y);
int is_entrance_illegal(int x, int y);
int is_exit_illegal(int x, int y);