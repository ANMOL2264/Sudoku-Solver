#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define N 9

void setDifficulty(int, int);
int for_each_subgrid[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int grid[N][N] = {0};
int user_grid[N][N] = {0};
bool not_removed[N][N] = {false}; // a boolean array used to identify which cell is permanent and which can be modified
char box_value_final[] = "  ";    // takes the returned string from citoa function
char list[9][9] = {"         ",
                   "         ",
                   "         ",
                   "         ",
                   "         ",
                   "         ",
                   "         ",
                   "         ",
                   "         "};
void box() // Actual grid structure which will be shown on console
{
    printf("\n");
    for (int row = 0; row < 9; row++)
    {
        printf("  %c  |  %c  |  %c  ||  %c  |  %c  |  %c  ||  %c  |  %c  |  %c  ", list[row][0], list[row][1], list[row][2], list[row][3], list[row][4], list[row][5], list[row][6], list[row][7], list[row][8]);
        if (row == 2 || row == 5)
        {
            printf("\n-------------------------------------------------------\n-------------------------------------------------------\n");
        }
        else
            printf("\n-------------------------------------------------------\n");
    }
}
void *citoa(int num, char *str) // convert integer to string
{
    str[0] = ' ';
    int rem = num % 10;
    str[1] = rem + '0';
    str[2] = ' ';
    str[3] = '\0';
    return str;
}
void random_list(int numbers[]) // Randomize the array of number frim 1 to 9 ti make a unique puzzle
{
    for (int i = 8; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }
}
bool isSafe(int grid1[N][N], int row, int col, int allocated) // Checks if the alloted number follows the condition of suduko
{
    // checking for row and column
    for (int i = 0; i < N; i++)
    {
        if (grid1[row][i] == allocated || grid1[i][col] == allocated)
        {
            return false;
        }
    }
    // checking for 3x3 subgrid
    int startRow = row - (row % 3);
    int startCol = col - (col % 3);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid1[startRow + i][startCol + j] == allocated)
            {
                return false;
            }
        }
    }
    return true;
}
bool generate(int grid1[N][N], int row, int col) // Generates a complete filled grid
{
    if (row == (N - 1) && col == N)
    {
        return true;
    }

    if (col == N)
    {
        col = 0;
        row++;
    }

    random_list(for_each_subgrid);

    for (int i = 0; i < 9; i++)
    {
        int num = for_each_subgrid[i];
        if (isSafe(grid1, row, col, num))
        {
            grid1[row][col] = num;
            if (generate(grid1, row, col + 1))
            {
                return true;
            }
            grid1[row][col] = 0;
        }
    }
    return false;
}
void setDifficulty(int upper, int lower) // Sets the difficulty of problem by removing values in cells randomly on the basis of upper and lower limit
{
    upper++;
    int number_of_cell_to_keep = (rand() % (upper - lower)) + lower, row, col;
    for (int i = 0; i < number_of_cell_to_keep; i++)
    {
        do
        {
            row = rand() % N;
            col = rand() % N;
        } while (not_removed[row][col]);

        not_removed[row][col] = true;
        user_grid[row][col] = grid[row][col];
        char *ptr = citoa(grid[row][col], box_value_final);
        list[row][col] = *(ptr + 1);
    }
}
void transfer_solution_to_box() // transfers the solved grid in integer array to character array to show it using box()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            char *ptr = citoa(user_grid[i][j], box_value_final);
            list[i][j] = *(ptr + 1);
        }
    }
}
bool solve(int grid2[N][N], int row, int col) // Solve the sudoku problem
{
    if (row == (N - 1) && col == N)
    {
        return true; // All cells are filled
    }

    if (col == N)
    {
        col = 0;
        row++;
    }

    int nextRow = row, nextCol = col;
    while ((not_removed[nextRow][nextCol]) && (nextRow < N))
    {
        if (nextCol == (N - 1))
        {
            nextCol = 0;
            nextRow++;
        }
        else
            nextCol++;
    }
    if (nextRow >= N)
    {
        return true;
    }
    random_list(for_each_subgrid);
    // Find the next empty cell

    for (int i = 0; i <= 8; i++)
    {
        int num = for_each_subgrid[i];
        if (isSafe(grid2, nextRow, nextCol, num))
        {
            grid2[nextRow][nextCol] = num;

            if (solve(grid2, nextRow, nextCol + 1))
            {
                return true;
            }
            grid2[nextRow][nextCol] = 0; // Backtrack
        }
    }
    return false;
}

int main()
{
    srand(time(NULL));
    while (1)
    {
        if (generate(grid, 0, 0))
        {
            /*1. Easy: Around 36 to 46 given cells .
            2. Medium: Around 32 to 35 given cells.
            3. Hard: Around 28 to 31 given cells.
            4. Expert: Around 22 to 27 given cells.
            5. Diabolical: Fewer than 22 given cells.*/
            setDifficulty(46, 36); // first argument is maximum number and second is the minimum number of elements to be pre-filled for sudoku problem generation in accordance with specific difficulties
            break;
        }
    }
    printf("\nSudoku to solve \n");
    box();

    printf("\n\n\n");
    while (1)
    {
        if (solve(user_grid, 0, 0))
        {
            transfer_solution_to_box();
            break;
        }
    }
    printf("Solved Sudoku \n");
    box();
}