#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for positive integer
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1);

    // print an n-by-n grid of bricks/spaces
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < (n * 2); j++)
        {
            // if middle, add gap
            if (j == n)
            {
                printf("  ");
            }

            // Decide whether " ", or "#" based on location in grid
            if (j <= (n - i - 1))
            {
                printf(" ");
            }
            else if (j >= (n + i))
            {
                printf("");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }
}
