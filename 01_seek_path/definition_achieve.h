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
			stack->front = temp;	//如果栈不为空，令栈顶元素的front指针指向新的栈顶

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
	stack = stack->next;	//栈顶指针后移
	if(stack != NULL)
		stack->front = NULL;	//新的栈顶指针的front指针置空

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
	
	return p == NULL ? 0 : p->dir;		//如果找到，返回该位置向下一步的前进方向，否则返回0
}

void save_path(Stack *stack, FILE *p)
{
	Stack *pr = stack;

	while(pr->next != NULL)
		pr = pr->next;		//pr指向栈底

	fprintf(p, "第%d条通路\n", way_count);
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
		for(i=0;i<size_x;i++)			//对迷宫随机赋值
		{
			for(j=0;j<size_y;j++)
				maze[i][j] = rand() % 2;
		}
	
		for(i=0;i<size_x;i++)
		{
			maze[i][0] = 1;				//左墙
			maze[i][size_y-1] = 1;		//右墙
		}
		for(i=0;i<size_y;i++)
		{
			maze[0][i] = 1;				//上墙
			maze[size_x-1][i] = 1;		//下墙
		}
	
		maze[entrance_x][entrance_y] = 0;		//入口
		maze[out_x][out_y] = 0;			//出口
	
		system("cls");
		printf("Maze creating......\n\n");
		for(i=0;i<size_x;i++)
		{
			for(j=0;j<size_y;j++)
			{
				if(maze[i][j] == 1)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
															BACKGROUND_RED);	//红色输出砖块
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
															BACKGROUND_GREEN);	//绿色输出通路

				printf("  ");
				Sleep(5);	//延时50毫秒
			}

			printf("\n");
		}
		printf("\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
															FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		seek_path_count(maze, entrance_x, entrance_y);		//搜索通路数量
	}

	for(i=0;i<size_x;++i)
	{
		for(j=0;j<size_y;++j)
		{
			fprintf(f_maze, "%-2d", maze[i][j]);		//将迷宫保存到文件中
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
	COORD target_pos;			//保存光标目标位置
	CONSOLE_SCREEN_BUFFER_INFO current_pos;		//保存光标当前位置
	Stack *p = step;		//保存所有走过的位置
	
	system("cls");		//清屏
	printf("第%d条通路\n\n", way_count);

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), 
																&current_pos);		//获取当前光标的位置;
	start_line = current_pos.dwCursorPosition.Y;		//记录迷宫的第一行所在位置

	for(i=0;i<size_x;++i)	//输出迷宫
	{
		for(j=0;j<size_y;++j)
		{
			if((dir = is_way(stack, i, j)) != 0)		//位于通路上的0被置1，则输出时输出0
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
		p = p->next;		//p指向栈底,即迷宫的入口

	while(p != NULL)//动态输出走出迷宫的过程
	{
		target_pos.X = (p->y)*2; 
		target_pos.Y = start_line + p->x;		//根据通路上的点在迷宫中的位置，设置光标
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
							   target_pos);			//光标跳到目标位置

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
							  FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|BACKGROUND_GREEN);		//绿色输出通路
		switch(p->dir)//以箭头表示移动方向
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
			case 5:					//当dir>4,即所有方向都已尝试，无通路，后退一步
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
									  BACKGROUND_GREEN);	
				printf("  ");
				break;
			default:
				break;
		} 

		Sleep(500);		//延时500毫秒
		p = p->front;	//指针移到下一步
	}

	target_pos.X = 0;
	target_pos.Y = start_line + size_x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
						   target_pos);			//光标跳到输出迷宫的末尾
	printf("\n");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
					      FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);//恢复到白色字体
}

void seek_path(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y)
{
	Stack last_location;		//记录上一步的位置
	Stack *stack = NULL;		//记录通路
	Stack *step = NULL;		//记录所有走过的位置

	int dir = 1;		//初始方向为向右
	int next_x, next_y;

	FILE *p = fopen("path.txt", "w");	//创建文件,保存所有通路
	if(p == NULL)
	{
		printf("File not open\n");
		getchar();
		exit(0);
	}

	while(!(dir > 4 && x == entrance_x && y == entrance_y))	//当当前位置位于迷宫出口，并且方向dir>4表示没有通路
	{
		step = push(step, x, y, dir);	//保存走过的每一步

		if(dir > 4)		//当dir>4，表示当前位置走不通，则返回到上一能走通的位置，在上一位置换个方向尝试
		{
			if(!is_empty(stack))	
			{
				last_location = get_top_elem(stack);	//上一能走通的位置存在栈顶
				stack = pop(stack);
				x = last_location.x;
				y = last_location.y;	//后退一步
				maze[x][y] = 0;

				dir = last_location.dir + 1;	//尝试下一方向
			}//if
		}//if
		else
		{
			switch(dir)		//根据dir来选择下一个位置
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
				++dir;	//到达边界或下一步不是通路，则尝试另一方向
			}//if
			else
			{
				stack = push(stack, x, y, dir);		//若下一步可以走通，则保存当前位置

				if(next_x == out_x && next_y == out_y)
				{
					way_count++;	//如果到达出口，则给通路数量加一
					step = push(step, next_x, next_y, dir);
					stack = push(stack, next_x, next_y, dir);

					print_maze_dynamic(maze, stack, step);	//输出图形通路
					print_stack(stack);		//输出字符通路
					printf("\nPress any key to continue......\n");
					getchar();

					save_path(stack, p);	//保存路径到文件中

					stack = pop(stack);		//将出口出栈
					stack = pop(stack);		//将出口前一位置出栈
					step =pop(step);		//将出口出栈
					dir++;
				}//if
				else
				{
					maze[x][y] = 1;		//将当前位置标识为已走过

					x = next_x;
					y = next_y;
					dir = 1;
				}//else
			}//else		
		}//else
	}//while

	fclose(p);	//关闭文件

	printf("\nAll path has been found\n");
}

void seek_path_count(int maze[SIZE_X_MAX][SIZE_Y_MAX], int x, int y)
{	
	if(x >= size_x || x < 0 || y >= size_y || y < 0)	//若果到达边界，返回
		return;
		
	if(maze[x][y] == 1)	//如果走不通，返回
		return;	

	if(x == out_x && y == out_y)	//如果到达出口，给通路数量加1，然后返回
	{
		way_count++;
		return;
	}
	
	maze[x][y] = 1;	//如果能走通则将当前位置的值置1，标志以走过

	seek_path_count(maze, x, y+1);	//right
	seek_path_count(maze, x+1, y);	//down
	seek_path_count(maze, x, y-1);	//left
	seek_path_count(maze, x-1, y);	//up
		
		
	maze[x][y] = 0;	
		
	return;
}

int is_size_illegal(int x, int y)	//判断输入的迷宫大小是否正确
{
	if(x < 3 || x > SIZE_X_MAX-1)
		return 1;
	else if(y < 3 || y > SIZE_Y_MAX-1)
		return 1;
	else
		return 0;

}

int is_entrance_illegal(int x, int y)	//判断输入的入口是否正确
{
	if((x >= 0 && x < size_x) && (y >= 0 && y < size_y))
	{
		if(x > 0 && x < size_x-1)
		{
			if(y == 0 || y == size_y-1)	//入口在左右两边
			{
				return 0;
			}
			else
				return 1;
		}
		else if(y > 0 && y < size_y-1)	//入口在上下两边
		{
			return 0;
		}
		else
			return 1;
	}
	else
		return 1;
}

int is_exit_illegal(int x, int y)		//判断输入的出口是否正确
{
	if(x >= 0 && x < size_x && y >= 0 && y < size_y)
	{
		if(x == entrance_x && y == entrance_y)		//与入口是否相同
			return 1;

		if(x > 0 && x < size_x-1)
		{
			if(y == 0 || y == size_y-1)	//出口在左右两边
			{
				return 0;
			}
			else
				return 1;
		}
		else if(y > 0 && y < size_y-1)	//出口在上下两边
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
	scanf("%d %d", &size_x, &size_y);					//输入迷宫的规模
	while(is_size_illegal(size_x, size_y))
	{
		printf("Enter error! Try again\n");
		flush();		//清空输入流
		scanf("%d %d", &size_x, &size_y);
	}
		
	printf("Enter maze entrance. eg:(0  1)\n");
	scanf("%d %d", &entrance_x, &entrance_y);			//输入迷宫的入口
	while(is_entrance_illegal(entrance_x, entrance_y))
	{
		printf("Enter error! Try again\n");
		flush();
		scanf("%d %d", &entrance_x, &entrance_y);
	}

	printf("Enter maze exit. eg:(8  9)\n");
	scanf("%d %d", &out_x, &out_y);					//输入迷宫的出口
	while(is_exit_illegal(out_x, out_y))			//若无通路或和出口相同则重新输入
	{
		printf("Enter error! Try again\n");
		flush();
		scanf("%d %d", &out_x, &out_y);
	}

	printf("\n");
	printf("size: %d * %d\n", size_x, size_y);				//输出迷宫的大小、入口、出口
	printf("entrance: %d, %d\n", entrance_x, entrance_y);
	printf("exit: %d, %d\n", out_x, out_y);
	printf("\nPress any key to start creating maze......\n");

	flush();
	getchar();
}