#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char maze[16][16]; // 2D array for
char path_maze[16][16]; // copy of maze for pathfinding algorithm
int player_pos[] = {0, 0}; // position of player
int monster_pos[] = {0, 0}; // position of monster
int escape_pos[] = {7, 0}; // position of escape point
int hard = 0; // difficulty level

void main()
{
    while (1)
    {
        printf("Please choose the difficulty [0: easy, 1: hard]: ");
        scanf("%d", &hard);

        int won = 0;
        clear_maze(); // configures maze for start (sets position of walls and escape point)
        spawn_characters(); // Spawns player and monster
        show_maze(); // Prints maze

        while (1)
        {
            int move_num;
            printf("\n8: up 2: down 4: left 6: right\nPlease enter move: "); // Numbers choosed for numpad
            scanf("%d", &move_num);
            if (player_move(move_num)) // player_move function returns 1 if move is valid 
            {
                if (player_pos[0] == escape_pos[0] && player_pos[1] == escape_pos[1]) // Checks winning
                {
                    won = 1;
                    break;
                }
                else if (player_pos[0] == monster_pos[0] && player_pos[1] == monster_pos[1]) // Checks losing
                {
                    won = 0;
                    break;
                }
                else
                {
                    monster_move(); // Moves monster
                    show_maze();
                    if (monster_pos[0] == player_pos[0] && monster_pos[1] == player_pos[1])
                    {
                        won = 0;
                        break;
                    }
                    continue;
                }
            }
            else
            {
                printf("Could not move!\nPlease enter valid move number!"); //
            }
        }

        if (won)
        {
            printf("\n---YOU WON!---");
        }
        else
        {
            printf("\n---YOU LOSE!---");
        }

        printf("\nDo you want to play again? [0, 1]: ");
        int flag;
        scanf("%d", &flag);
        if (!flag)
        {
            break;
        }
    }
}
void monster_move()
{
    srand((unsigned)time(NULL)); // It sets seed for random number generating
    if (hard) // If hard == 1, monster moves most efficient move
    {
        int move_num = find_best_way(); // find_best_way function is finds shortest way and returns move_num for this
        if (move_num == 0)
        {

            maze[monster_pos[0]][monster_pos[1]] = ' ';
            monster_pos[0] -= 1;
        }
        else if (move_num == 1)
        {

            maze[monster_pos[0]][monster_pos[1]] = ' ';
            monster_pos[0] += 1;
        }
        else if (move_num == 2)
        {

            maze[monster_pos[0]][monster_pos[1]] = ' ';
            monster_pos[1] -= 1;
        }
        else if (move_num == 3)
        {
            maze[monster_pos[0]][monster_pos[1]] = ' ';
            monster_pos[1] += 1;
        }
    }
    else // if hard == 0, monster moves randomly
    {
        int moved = 0;
        while (!moved)
        {
            int move_num = rand() % 4;
            if (move_num == 0)
            {
                if (maze[monster_pos[0] - 1][monster_pos[1]] == ' ' && (monster_pos[0] - 1 < 16 && monster_pos[0] - 1 >= 0))
                {
                    maze[monster_pos[0]][monster_pos[1]] = ' ';
                    monster_pos[0] -= 1;
                    moved = 1;
                }
            }
            else if (move_num == 1)
            {
                if (maze[monster_pos[0] + 1][monster_pos[1]] == ' ' && (monster_pos[0] + 1 < 16 && monster_pos[0] + 1 >= 0))
                {
                    maze[monster_pos[0]][monster_pos[1]] = ' ';
                    monster_pos[0] += 1;
                    moved = 1;
                }
            }
            else if (move_num == 2)
            {
                if (maze[monster_pos[0]][monster_pos[1] - 1] == ' ' && (monster_pos[1] - 1 < 16 && monster_pos[1] - 1 >= 0))
                {
                    maze[monster_pos[0]][monster_pos[1]] = ' ';
                    monster_pos[1] -= 1;
                    moved = 1;
                }
            }
            else if (move_num == 3)
            {
                if (maze[monster_pos[0]][monster_pos[1] + 1] == ' ' && (monster_pos[1] + 1 < 16 && monster_pos[1] + 1 >= 0))
                {
                    maze[monster_pos[0]][monster_pos[1]] = ' ';
                    monster_pos[1] += 1;
                    moved = 1;
                }
            }
        }
    }
}

int find_best_way()
{
    int i, x, row, column;
    int next_positions[30][3];
    int next_positions2[30][3];
    int best_move = 4;
    for (i = 0; i < 30; i++)
    {
        next_positions2[i][2] = 0;
    }
    for (i = 0; i < 30; i++)
    {
        next_positions[i][2] = 0;
    }
    for (i = 0; i < 16; i++)
    {
        for (x = 0; x < 16; x++)
        {
            path_maze[i][x] = maze[i][x];
        }
    }
    path_maze[player_pos[0]][player_pos[1]] = 'P';
    next_positions[0][0] = player_pos[0];
    next_positions[0][1] = player_pos[1];
    next_positions[0][2] = 1;

    path_maze[monster_pos[0]][monster_pos[1]] = 'M';
    int founded = 0;

    while (!founded)
    {
        for (i = 0; i < 30; i++)
        {
            if (next_positions[i][2])
            {
                row = next_positions[i][0];
                column = next_positions[i][1];
                if (path_maze[row - 1][column] == ' ' && (row - 1 < 16 && row - 1 >= 0))
                {
                    path_maze[row - 1][column] = 'B';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions2[x][2] != 1)
                        {
                            next_positions2[x][2] = 1;
                            next_positions2[x][0] = row - 1;
                            next_positions2[x][1] = column;
                            break;
                        }
                    }
                }
                else if (path_maze[row - 1][column] == 'M' && (row - 1 < 16 && row - 1 >= 0))
                {
                    founded = 1;
                    best_move = 1;
                    break;
                }

                if (path_maze[row + 1][column] == ' ' && (row + 1 < 16 && row + 1 >= 0))
                {
                    path_maze[row + 1][column] = 'T';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions2[x][2] != 1)
                        {
                            next_positions2[x][2] = 1;
                            next_positions2[x][0] = row + 1;
                            next_positions2[x][1] = column;
                            break;
                        }
                    }
                }
                else if (path_maze[row + 1][column] == 'M' && (row + 1 < 16 && row + 1 >= 0))
                {
                    founded = 1;
                    best_move = 0;
                    break;
                }

                if (path_maze[row][column - 1] == ' ' && (column - 1 < 16 && column - 1 >= 0))
                {
                    path_maze[row][column - 1] = 'R';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions2[x][2] != 1)
                        {
                            next_positions2[x][2] = 1;
                            next_positions2[x][0] = row;
                            next_positions2[x][1] = column - 1;
                            break;
                        }
                    }
                }
                else if (path_maze[row][column - 1] == 'M' && (column - 1 < 16 && column - 1 >= 0))
                {
                    founded = 1;
                    best_move = 3;
                    break;
                }

                if (path_maze[row][column + 1] == ' ' && (column + 1 < 16 && column + 1 >= 0))
                {
                    path_maze[row][column + 1] = 'L';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions2[x][2] != 1)
                        {
                            next_positions2[x][2] = 1;
                            next_positions2[x][0] = row;
                            next_positions2[x][1] = column + 1;
                            break;
                        }
                    }
                }
                else if (path_maze[row][column + 1] == 'M' && (column + 1 < 16 && column + 1 >= 0))
                {
                    founded = 1;
                    best_move = 2;
                    break;
                }
            }
        }
        for (i = 0; i < 30; i++)
        {
            next_positions[i][2] = 0;
        }

        for (i = 0; i < 30; i++)
        {
            if (next_positions2[i][2])
            {
                row = next_positions2[i][0];
                column = next_positions2[i][1];
                if (path_maze[row - 1][column] == ' ' && (row - 1 < 16 && row - 1 >= 0))
                {
                    path_maze[row - 1][column] = 'B';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions[x][2] != 1)
                        {
                            next_positions[x][2] = 1;
                            next_positions[x][0] = row - 1;
                            next_positions[x][1] = column;
                            break;
                        }
                    }
                }
                else if (path_maze[row - 1][column] == 'M' && (row - 1 < 16 && row - 1 >= 0))
                {
                    founded = 1;
                    break;
                }
                if (path_maze[row + 1][column] == ' ' && (row + 1 < 16 && row + 1 >= 0))
                {
                    path_maze[row + 1][column] = 'T';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions[x][2] != 1)
                        {
                            next_positions[x][2] = 1;
                            next_positions[x][0] = row + 1;
                            next_positions[x][1] = column;
                            break;
                        }
                    }
                }
                else if (path_maze[row + 1][column] == 'M' && (row + 1 < 16 && row + 1 >= 0))
                {
                    founded = 1;
                    break;
                }
                if (path_maze[row][column - 1] == ' ' && (column - 1 < 16 && column - 1 >= 0))
                {
                    path_maze[row][column - 1] = 'R';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions[x][2] != 1)
                        {
                            next_positions[x][2] = 1;
                            next_positions[x][0] = row;
                            next_positions[x][1] = column - 1;
                            break;
                        }
                    }
                }
                else if (path_maze[row][column - 1] == 'M' && (column - 1 < 16 && column - 1 >= 0))
                {
                    founded = 1;
                    break;
                }
                if (path_maze[row][column + 1] == ' ' && (column + 1 < 16 && column + 1 >= 0))
                {
                    path_maze[row][column + 1] = 'L';
                    for (x = 0; x < 30; x++)
                    {
                        if (next_positions[x][2] != 1)
                        {
                            next_positions[x][2] = 1;
                            next_positions[x][0] = row;
                            next_positions[x][1] = column + 1;
                            break;
                        }
                    }
                }
                else if (path_maze[row][column + 1] == 'M' && (column + 1 < 16 && column + 1 >= 0))
                {
                    founded = 1;
                    break;
                }
            }
        }
        for (i = 0; i < 30; i++)
        {
            next_positions2[i][2] = 0;
        }
    }
    return best_move;
}

int player_move(int move_num)
{
    int moved = 0;

    if (move_num == 8)
    {
        if (maze[player_pos[0] - 1][player_pos[1]] != 'W' && (player_pos[0] - 1 < 16 && player_pos[0] - 1 >= 0))
        {
            maze[player_pos[0]][player_pos[1]] = ' ';
            player_pos[0] -= 1;
            moved = 1;
        }
    }
    else if (move_num == 2)
    {
        if (maze[player_pos[0] + 1][player_pos[1]] != 'W' && (player_pos[0] + 1 < 16 && player_pos[0] + 1 >= 0))
        {
            maze[player_pos[0]][player_pos[1]] = ' ';
            player_pos[0] += 1;
            moved = 1;
        }
    }
    else if (move_num == 4)
    {
        if (maze[player_pos[0]][player_pos[1] - 1] != 'W' && (player_pos[1] - 1 < 16 && player_pos[1] - 1 >= 0))
        {
            maze[player_pos[0]][player_pos[1]] = ' ';
            player_pos[1] -= 1;
            moved = 1;
        }
    }
    else if (move_num == 6)
    {
        if (maze[player_pos[0]][player_pos[1] + 1] != 'W' && (player_pos[1] + 1 < 16 && player_pos[1] + 1 >= 0))
        {
            maze[player_pos[0]][player_pos[1]] = ' ';
            player_pos[1] += 1;
            moved = 1;
        }
    }
    return moved;
}

void show_maze()
{
    maze[player_pos[0]][player_pos[1]] = 'P';
    maze[monster_pos[0]][monster_pos[1]] = 'M';
    int row, column;
    for (row = 0; row < 16; row++)
    {
        print_horizontal_wall(65);
        printf("\n");
        for (column = 0; column < 16; column++)
        {
            printf("| %c ", maze[row][column]);
        }
        printf("|\n");
    }
    print_horizontal_wall(65);
}
/* Shows path_maze for debug
void show_path_maze() 
{
    int row, column;
    path_maze[player_pos[0]][player_pos[1]] = 'P';
    path_maze[monster_pos[0]][monster_pos[1]] = 'M';
    for (row = 0; row < 16; row++)
    {
        print_horizontal_wall(65);
        printf("\n");
        for (column = 0; column < 16; column++)
        {
            printf("| %c ", path_maze[row][column]);
        }
        printf("|\n");
    }
    print_horizontal_wall(65);
}*/

void print_horizontal_wall(int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        printf("-");
    }
}

void clear_maze()
{
    player_pos[0] = player_pos[1] = 0;
    monster_pos[0] = monster_pos[1] = 0;
    int row, column;
    for (row = 0; row < 16; row++)
    {
        for (column = 0; column < 16; column++)
        {
            maze[row][column] = ' ';
        }
    }
    maze[0][8] = maze[1][8] = maze[2][8] = maze[2][7] = maze[2][9] =
        maze[2][2] = maze[3][2] =
            maze[2][13] = maze[3][13] =
                maze[5][6] = maze[5][7] = maze[5][8] = maze[6][6] =
                    maze[9][3] = maze[9][4] = maze[9][5] = maze[10][5] =
                        maze[8][10] = maze[9][10] = maze[10][10] = maze[11][10] = maze[11][11] =
                            maze[12][2] = maze[13][2] = maze[14][2] = maze[14][3] = maze[14][4] = 'W';

    maze[7][0] = 'E';
}

void spawn_characters()
{
    srand((unsigned)time(NULL));

    while (1)
    {
        player_pos[0] = rand() % 16;
        player_pos[1] = rand() % 16;
        int distance_escape = abs(player_pos[0] - 7) + player_pos[1];
        if (maze[player_pos[0]][player_pos[1]] == ' ' && distance_escape >= 16)
        {
            break;
        }
    }

    while (1)
    {
        monster_pos[0] = rand() % 16;
        monster_pos[1] = rand() % 16;
        int distance_player = abs(monster_pos[0] - player_pos[0]) + abs(monster_pos[1] - player_pos[1]);

        if (maze[monster_pos[0]][monster_pos[1]] == ' ' && distance_player >= 16)
        {
            break;
        }
    }
}
