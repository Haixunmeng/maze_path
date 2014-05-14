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

	creat_maze_random(maze);

	printf("\nMaze creating finish\n%d path\n", way_count);
	way_count = 0;

	printf("\nPress any key to continue......\n");
	getchar();

	seek_path(maze, entrance_x, entrance_y);

	getchar();
	return 0;
}
