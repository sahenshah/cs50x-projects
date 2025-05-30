#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int count = 0;
    long input = 0;
    char Number[20];
    bool Valid = 0;
    int Len, d1, d2;
    int is_second = 0;

    // Prompt user for positive integer
    while (input < 1)
    {
        input = get_long("Number: ");
    }

    // convert to string to check length and extract digits
    sprintf(Number, "%ld", input);

    // Convert the first and second characters to integers
    Len = strlen(Number);
    d1 = Number[0] - '0';
    d2 = Number[1] - '0';

    // Calc CheckSum
    for (int i = Len - 1; i >= 0; i--)
    {
        // Skip non-digit characters
        if (!isdigit(Number[i]))
        {
            return 0; // Invalid input
        }

        int digit = Number[i] - '0';

        if (is_second)
        {
            // Multiply Every other digit by 2 starting with the numbers second-to-last digit
            digit *= 2;

            // If the result is two digits, sum the digits
            if (digit > 9)
            {
                digit -= 9; // Same as summing the digits of digit
            }
        }

        // Add the sum to the sum of the digits that werent multiplied by 2
        count += digit;

        is_second = !is_second; // Toggle flag
    }

    // If the totals last digit is 0, the number is valid
    if (count % 10 == 0)
        Valid = 1;

    // Check Length and starting 2 digits to determine provider
    // Check AMEX
    if (Valid)
    {
        if (Len == 15)
        {
            if (d1 == 3 && (d2 == 4 || d2 == 7))
            {
                printf("AMEX\n");
            }
            else
            {
                Valid = 0;
            }
        }
        // Check MASTERCARD
        else if (strlen(Number) == 16)
        {
            if (d1 == 5 && (d2 >= 1 && d2 <= 5))
            {
                printf("MASTERCARD\n");
            }
            else if (d1 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                Valid = 0;
            }
        }
        //  Check VISA
        else if (strlen(Number) == 13 && d1 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            Valid = 0;
        }
    }

    // Not AMEX, MASTERCARD or VISA... INVALID
    if (!Valid)
    {
        printf("INVALID\n");
    }
}
