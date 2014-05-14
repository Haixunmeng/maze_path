#include "definition_achieve.h"

int main()
{
	int maze[SIZE_X_MAX][SIZE_Y_MAX];/*={
	{1,0,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,1,1,0,0,1,0,1},
	{1,0,0,0,0,1,1,0,0,1},
	{1,0,1,1,1,0,0,0,0,1},
	{1,0,0,0,1,1,0,0,0,1},
	{1,0,1,0,0,0,1,0,0,1},
	{1,0,1,1,1,0,1,1,0,1},
	{1,1,0,0,0,0,0,0,0,0},
	{1,1,1,1,1,1,1,1,1,1}};*/

	printf("Enter maze row and column. fg:(10, 10)\n");
	scanf("%d %d", &size_x, &size_y);					//输入迷宫的规模
	while(is_size_illegal(size_x, size_y))
	{
		printf("Enter erroe! Try again\n");
		scanf("%d %d", &size_x, &size_y);
	}
		
	printf("Enter maze entrance. fg:(0, 1)\n");
	scanf("%d %d", &entrance_x, &entrance_y);			//输入迷宫的入口
	while(is_entrance_illegal(entrance_x, entrance_y))
	{
		printf("Enter erroe! Try again\n");
		scanf("%d %d", &entrance_x, &entrance_y);
	}

	printf("Enter maze exit. fg:(8, 9)\n");
	scanf("%d %d", &out_x, &out_y);					//输入迷宫的出口
	while(is_exit_illegal(out_x, out_y))
	{
		printf("Enter erroe! Try again\n");
		scanf("%d %d", &out_x, &out_y);
	}

	printf("size: %d * %d\n", size_x, size_y);
	printf("entrance: %d, %d\n", entrance_x, entrance_y);
	printf("exit: %d, %d\n", out_x, out_y);

	getchar();
	getchar();

	creat_maze_random(maze);		//随机创建迷宫
	seek_path_count(maze, 0, 1);	//搜索迷宫通路的数量

	while(way_count == 0)			//若迷宫没有通路，则再次随机创建一个迷宫
	{
		printf("No way out!\n\n");
		//delay(50000000);

		system("cls");
		creat_maze_random(maze);

		seek_path_count(maze, entrance_x, entrance_y);
	}
	printf("Maze creating finish\n%d path\n", way_count);
	way_count = 0;

	printf("\nPress any key to continue......\n");
	getchar();

	seek_path(maze, entrance_x, entrance_y);

	getchar();
	return 0;
}
