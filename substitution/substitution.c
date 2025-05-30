#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define ALPHABET_SIZE 26

char convert_char(char c, const char *key);
int is_valid_key(const char *key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        // Check if the program was called with exactly one argument
        printf("Usage: ./substitution key \n");
        return 1;
    }

    char *key = argv[1];

    // Check that the key is exactly 26 characters long and valid
    if (strlen(key) != ALPHABET_SIZE || !is_valid_key(key))
    {
        printf("Error: The key must be exactly 26 alphabetic characters and contain each letter "
               "exactly once.\n");
        return 1;
    }

    // get plain text from user
    string plaintext = get_string("plaintext: ");

    // Convert the plaintext to ciphertext
    char ciphertext[strlen(plaintext) + 1]; // +1 for the null-terminator
    for (int i = 0; i < strlen(plaintext); i++)
    {
        ciphertext[i] = convert_char(plaintext[i], key);
    }

    ciphertext[strlen(plaintext)] = '\0'; // Null-terminate the ciphertext

    // Print the ciphertext
    printf("ciphertext: %s\n", ciphertext);

    return 0;
}

// Function to check if the key contains each letter exactly once
int is_valid_key(const char *key)
{
    int letter_count[ALPHABET_SIZE] = {0}; // Array to count occurrences of each letter

    // Iterate through the key and count occurrences of each letter
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (!isalpha(key[i]))
        {
            return 0; // Key must only contain alphabetic characters
        }
        int index = tolower(key[i]) - 'a';
        if (index < 0 || index >= ALPHABET_SIZE || letter_count[index] > 0)
        {
            return 0; // Invalid if the letter is out of range or repeated
        }
        letter_count[index]++; // Increment the letter count
    }
    return 1; // The key is valid if no letter is repeated and all letters are present
}

// Function to convert a character using the key
char convert_char(char c, const char *key)
{
    if (isalpha(c))
    {
        // Determine if the character is uppercase or lowercase
        char base = isupper(c) ? 'A' : 'a';
        // Get the index of the letter in the alphabet (0-25)
        int index = tolower(c) - 'a';
        // Return the corresponding character from the key (maintaining case)
        return isupper(c) ? toupper(key[index]) : tolower(key[index]);
    }
    else
    {
        // If it's not a letter, just return the character as is (e.g., spaces, punctuation)
        return c;
    }
}
