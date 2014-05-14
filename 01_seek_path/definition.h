#define Up 24
#define Down 25
#define Left 27
#define Right 26
/*********************************����ջ**********************************/
/***********����ջԪ������***********/
typedef struct stack	
{
	int x;				    //��ǰ������
	int y;
	int dir;				//��ǰλ�õ�ǰ������ 
	struct stack *next;		//ָ����һջԪ��
	struct stack *front;	//ָ����һ�ڵ�
}Stack, stack_elem;
 
 
/**********�����ջ�Ĳ���*************/ 
int isEmpty(Stack *stack);								// �ж�ջ�Ƿ�Ϊ�� 
Stack *pop(Stack *stack);								// ��ջ����,����ջ��ָ��		 
Stack *push(Stack *stack, int x, int y, int dir);		// ��ջ����������ջ��ָ��		 
stack_elem get_top_elem(Stack *stack);					// ��ȡջ��Ԫ�أ�����ջ��Ԫ��	
int isWay(Stack *stack, int x, int y);					// ����ջ���Ƿ������λ�� 
void print_stack(Stack *stack);						    //��ջ�׵�ջ�����ȫ��ջԪ��
Stack *empty_stack(Stack *stack);						//�������
/**************************************************************************/

/*************�����Թ�***************/

#define SIZE_X_MAX  100						//�����Թ�������ģ
#define SIZE_Y_MAX  100

int size_x;						//�Թ��Ĺ�ģ
int size_y;

int entrance_x;					//�����Թ������ 
int entrance_y;

int out_x;						//�����Թ��ĳ��� 
int out_y; 

int way_count = 0;															//��¼ͨ·������ 
void creat_maze_random(int maze[SIZE_X_MAX][SIZE_Y_MAX]);								//��������Թ�
void seek_path(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y);							//�ǵݹ鷽ʽ�ҳ�· 
void seek_path_count(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y);
void print_maze_dynamic(int maze[SIZE_X_MAX][SIZE_Y_MAX], Stack *stack, Stack *step);				    //��̬��ӡ�Թ�ͨ·
void print_way(Stack *stack);													//����Թ����ַ�ͨ· 
void delay(int n);															//��ʱ
void save_path(Stack *stack, FILE *p);													//��·�����浽�ļ���
int is_size_illegal(int x, int y);
int is_entrance_illegal(int x, int y);
int is_exit_illegal(int x, int y);