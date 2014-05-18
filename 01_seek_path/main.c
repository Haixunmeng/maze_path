#include "definition_achieve.h"

int main()
{
	int maze[SIZE_X_MAX][SIZE_Y_MAX];

	creat_maze_random(maze);

	printf("\nMaze creating finish\n%d path\n", way_count);
	way_count = 0;

	printf("\nPress any key to continue......\n");
	getchar();

	seek_path(maze, entrance_x, entrance_y);

	printf("Press any key to exit......\n");
	getchar();
	return 0;
}
