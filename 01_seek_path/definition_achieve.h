#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <time.h>
#include <WinCon.h>
#include "definition.h"

void flush()
{
	char c;
	while((c=getchar())!='\n'&&c!=EOF);
}

Stack *empty_stack(Stack *stack)
{
	Stack *p = stack;
	while(stack != NULL)
	{
		stack = stack->next;
		free(p);
		p = stack;
	}
	stack = NULL;

	return stack;
}

int is_empty(Stack *stack)
{
	if(stack == NULL)
		return 1;
	else
		return 0;
}

Stack *push(Stack *stack, int x, int y, int dir) 
{
	stack_elem *temp;
	
	temp = (stack_elem *)malloc(sizeof(stack_elem));
	
	if(temp)
	{
		temp->x = x;
		temp->y = y;
		temp->dir = dir;
		temp->next = stack;
		temp->front = NULL;

		if(stack != NULL)
			stack->front = temp;	//���ջ��Ϊ�գ���ջ��Ԫ�ص�frontָ��ָ���µ�ջ��

		stack = temp;
	}
	
	return stack;
}

stack_elem get_top_elem(Stack *stack)
{
	return *stack;
}

Stack *pop(Stack *stack)
{
	Stack *p = stack;
	stack = stack->next;	//ջ��ָ�����
	if(stack != NULL)
		stack->front = NULL;	//�µ�ջ��ָ���frontָ���ÿ�

	free(p);
	
	return stack;
}

int is_way(Stack *stack, int x, int y)
{
	Stack *p = stack;
	char i = 0;
	while(p != NULL)
	{		
		if(p->x == x && p->y == y)
			break;
		else
			p = p->next;
	}
	
	return p == NULL ? 0 : p->dir;		//����ҵ������ظ�λ������һ����ǰ�����򣬷��򷵻�0
}

void save_path(Stack *stack, FILE *p)
{
	Stack *pr = stack;

	while(pr->next != NULL)
		pr = pr->next;		//prָ��ջ��

	fprintf(p, "��%d��ͨ·\n", way_count);
	while(pr != NULL)
	{
		fprintf(p, "(%d, %d, %d)\n", pr->x, pr->y, pr->dir);
		pr = pr->front;
	}

	fprintf(p, "**********\n");
}

void creat_maze_random(int maze[SIZE_X_MAX][SIZE_Y_MAX])
{
	int i, j;
	FILE *f_maze = fopen("maze.txt", "w");		
	if(f_maze == NULL)
	{
		printf("file not open!\n");
		exit(0);
	}

	enter_arguments();

	srand((unsigned)time(NULL));
	
	while(way_count == 0)
	{
		for(i=0;i<size_x;i++)			//���Թ������ֵ
		{
			for(j=0;j<size_y;j++)
				maze[i][j] = rand() % 2;
		}
	
		for(i=0;i<size_x;i++)
		{
			maze[i][0] = 1;				//��ǽ
			maze[i][size_y-1] = 1;		//��ǽ
		}
		for(i=0;i<size_y;i++)
		{
			maze[0][i] = 1;				//��ǽ
			maze[size_x-1][i] = 1;		//��ǽ
		}
	
		maze[entrance_x][entrance_y] = 0;		//���
		maze[out_x][out_y] = 0;			//����
	
		system("cls");
		printf("Maze creating......\n\n");
		for(i=0;i<size_x;i++)
		{
			for(j=0;j<size_y;j++)
			{
				if(maze[i][j] == 1)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
															BACKGROUND_RED);	//��ɫ���ש��
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
															BACKGROUND_GREEN);	//��ɫ���ͨ·

				printf("  ");
				Sleep(5);	//��ʱ50����
			}

			printf("\n");
		}
		printf("\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
															FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		seek_path_count(maze, entrance_x, entrance_y);		//����ͨ·����
	}

	for(i=0;i<size_x;++i)
	{
		for(j=0;j<size_y;++j)
		{
			fprintf(f_maze, "%-2d", maze[i][j]);		//���Թ����浽�ļ���
		}
		fprintf(f_maze, "\n");
	}

	fclose(f_maze);
}

void print_stack(Stack *stack)
{
	if(stack == NULL)
		return;
		
	print_stack(stack->next);
	
	printf("%d, %d, %d\n", stack->x, stack->y, stack->dir);
	
	return;
}

void print_maze_dynamic(int maze[SIZE_X_MAX][SIZE_Y_MAX], Stack *stack, Stack *step)
{	
	int i, j, dir;
	int start_line;
	COORD target_pos;			//������Ŀ��λ��
	CONSOLE_SCREEN_BUFFER_INFO current_pos;		//�����굱ǰλ��
	Stack *p = step;		//���������߹���λ��
	
	system("cls");		//����
	printf("��%d��ͨ·\n\n", way_count);

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), 
																&current_pos);		//��ȡ��ǰ����λ��;
	start_line = current_pos.dwCursorPosition.Y;		//��¼�Թ��ĵ�һ������λ��

	for(i=0;i<size_x;++i)	//����Թ�
	{
		for(j=0;j<size_y;++j)
		{
			if((dir = is_way(stack, i, j)) != 0)		//λ��ͨ·�ϵ�0����1�������ʱ���0
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										BACKGROUND_GREEN);
			}
			else if(maze[i][j] == 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										BACKGROUND_GREEN);
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
										BACKGROUND_RED);
			}

			printf("  ");

		}
		printf("\n");
	}

	while(p->next != NULL)
		p = p->next;		//pָ��ջ��,���Թ������

	while(p != NULL)//��̬����߳��Թ��Ĺ���
	{
		target_pos.X = (p->y)*2; 
		target_pos.Y = start_line + p->x;		//����ͨ·�ϵĵ����Թ��е�λ�ã����ù��
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
							   target_pos);			//�������Ŀ��λ��

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
							  FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|BACKGROUND_GREEN);		//��ɫ���ͨ·
		switch(p->dir)//�Լ�ͷ��ʾ�ƶ�����
		{
			case 1:
				printf("%-2c", Right);
				break;
			case 2:
				printf("%-2c", Down);
				break;
			case 3:
				printf("%-2c", Left);
				break;
			case 4:
				printf("%-2c", Up);
				break;
			case 5:					//��dir>4,�����з����ѳ��ԣ���ͨ·������һ��
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
									  BACKGROUND_GREEN);	
				printf("  ");
				break;
			default:
				break;
		} 

		Sleep(500);		//��ʱ500����
		p = p->front;	//ָ���Ƶ���һ��
	}

	target_pos.X = 0;
	target_pos.Y = start_line + size_x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
						   target_pos);			//�����������Թ���ĩβ
	printf("\n");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
					      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);//�ָ�����ɫ����
}

void seek_path(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y)
{
	Stack last_location;		//��¼��һ����λ��
	Stack *stack = NULL;		//��¼ͨ·
	Stack *step = NULL;		//��¼�����߹���λ��

	int dir = 1;		//��ʼ����Ϊ����
	int next_x, next_y;

	FILE *p = fopen("path.txt", "w");	//�����ļ�,��������ͨ·
	if(p == NULL)
	{
		printf("File not open\n");
		getchar();
		exit(0);
	}

	while(!(dir > 4 && x == entrance_x && y == entrance_y))	//����ǰλ��λ���Թ����ڣ����ҷ���dir>4��ʾû��ͨ·
	{
		step = push(step, x, y, dir);	//�����߹���ÿһ��

		if(dir > 4)		//��dir>4����ʾ��ǰλ���߲�ͨ���򷵻ص���һ����ͨ��λ�ã�����һλ�û���������
		{
			if(!is_empty(stack))	
			{
				last_location = get_top_elem(stack);	//��һ����ͨ��λ�ô���ջ��
				stack = pop(stack);
				x = last_location.x;
				y = last_location.y;	//����һ��
				maze[x][y] = 0;

				dir = last_location.dir + 1;	//������һ����
			}//if
		}//if
		else
		{
			switch(dir)		//����dir��ѡ����һ��λ��
			{
				case 1:
					next_x = x;
					next_y = y+1;
					break;
				case 2:
					next_x = x+1;
					next_y = y;
					break;
				case 3:
					next_x = x;
					next_y = y-1;
					break;
				case 4:
					next_x = x-1;
					next_y = y;
					break;
				default:
					break; 
			}//switch

			if(next_x>=size_x || next_x<0 || next_y>=size_y || next_y<0 || maze[next_x][next_y] == 1)
			{
				++dir;	//����߽����һ������ͨ·��������һ����
			}//if
			else
			{
				stack = push(stack, x, y, dir);		//����һ��������ͨ���򱣴浱ǰλ��

				if(next_x == out_x && next_y == out_y)
				{
					way_count++;	//���������ڣ����ͨ·������һ
					step = push(step, next_x, next_y, dir);
					stack = push(stack, next_x, next_y, dir);

					print_maze_dynamic(maze, stack, step);	//���ͼ��ͨ·
					print_stack(stack);		//����ַ�ͨ·
					printf("\nPress any key to continue......\n");
					getchar();

					save_path(stack, p);	//����·�����ļ���

					stack = pop(stack);		//�����ڳ�ջ
					stack = pop(stack);		//������ǰһλ�ó�ջ
					step =pop(step);		//�����ڳ�ջ
					dir++;
				}//if
				else
				{
					maze[x][y] = 1;		//����ǰλ�ñ�ʶΪ���߹�

					x = next_x;
					y = next_y;
					dir = 1;
				}//else
			}//else		
		}//else
	}//while

	fclose(p);	//�ر��ļ�

	printf("\nAll path has been found\n");
}

void seek_path_count(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y)
{	
	if(x >= size_x || x < 0 || y >= size_y || y < 0)	//��������߽磬����
		return;
		
	if(maze[x][y] == 1)	//����߲�ͨ������
		return;	

	if(x == out_x && y == out_y)	//���������ڣ���ͨ·������1��Ȼ�󷵻�
	{
		way_count++;
		return;
	}
	
	maze[x][y] = 1;	//�������ͨ�򽫵�ǰλ�õ�ֵ��1����־���߹�

	seek_path_count(maze, x, y+1);	//right
	seek_path_count(maze, x+1, y);	//down
	seek_path_count(maze, x, y-1);	//left
	seek_path_count(maze, x-1, y);	//up
		
		
	maze[x][y] = 0;	
		
	return;
}

int is_size_illegal(int x, int y)	//�ж�������Թ���С�Ƿ���ȷ
{
	if(x < 3 || x > SIZE_X_MAX-1)
		return 1;
	else if(y < 3 || y > SIZE_Y_MAX-1)
		return 1;
	else
		return 0;

}

int is_entrance_illegal(int x, int y)	//�ж����������Ƿ���ȷ
{
	if((x >= 0 && x < size_x) && (y >= 0 && y < size_y))
	{
		if(x > 0 && x < size_x-1)
		{
			if(y == 0 || y == size_y-1)	//�������������
			{
				return 0;
			}
			else
				return 1;
		}
		else if(y > 0 && y < size_y-1)	//�������������
		{
			return 0;
		}
		else
			return 1;
	}
	else
		return 1;
}

int is_exit_illegal(int x, int y)		//�ж�����ĳ����Ƿ���ȷ
{
	if(x >= 0 && x < size_x && y >= 0 && y < size_y)
	{
		if(x == entrance_x && y == entrance_y)		//������Ƿ���ͬ
			return 1;

		if(x > 0 && x < size_x-1)
		{
			if(y == 0 || y == size_y-1)	//��������������
			{
				return 0;
			}
			else
				return 1;
		}
		else if(y > 0 && y < size_y-1)	//��������������
		{
			return 0;
		}
		else
			return 1;
	}
	else
		return 1;
}

void enter_arguments()
{
	printf("Enter maze row and column. eg:(10  10)\n");
	scanf("%d %d", &size_x, &size_y);					//�����Թ��Ĺ�ģ
	while(is_size_illegal(size_x, size_y))
	{
		printf("Enter error! Try again\n");
		flush();		//���������
		scanf("%d %d", &size_x, &size_y);
	}
		
	printf("Enter maze entrance. eg:(0  1)\n");
	scanf("%d %d", &entrance_x, &entrance_y);			//�����Թ������
	while(is_entrance_illegal(entrance_x, entrance_y))
	{
		printf("Enter error! Try again\n");
		flush();
		scanf("%d %d", &entrance_x, &entrance_y);
	}

	printf("Enter maze exit. eg:(8  9)\n");
	scanf("%d %d", &out_x, &out_y);					//�����Թ��ĳ���
	while(is_exit_illegal(out_x, out_y))			//����ͨ·��ͳ�����ͬ����������
	{
		printf("Enter error! Try again\n");
		flush();
		scanf("%d %d", &out_x, &out_y);
	}

	printf("\n");
	printf("size: %d * %d\n", size_x, size_y);				//����Թ��Ĵ�С����ڡ�����
	printf("entrance: %d, %d\n", entrance_x, entrance_y);
	printf("exit: %d, %d\n", out_x, out_y);
	printf("\nPress any key to start creating maze......\n");

	flush();
	getchar();
}