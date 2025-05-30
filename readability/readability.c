#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    double L = (letters * 100.00 / words);
    double S = (sentences * 100.00 / words);

    // Compute the Coleman-Liau index
    float index = (0.0588 * L) - (0.296 * S) - 15.8;

    // Handle Rounding
    int grade = (int) (index < 0 ? (index - 0.5) : (index + 0.5));

    // Print the grade level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}

int count_letters(string text)
{
    // Return the number of letters in text
    int letter_count = 0;

    // Compute letter count
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isalpha(text[i]))
        {
            letter_count++;
        }
    }

    return letter_count;
}

int count_words(string text)
{
    // Return the number of words in text
    int word_count = 0;
    bool in_word = false;

    // Compute word count
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (isspace(text[i]))
        {
            if (in_word)
            {
                in_word = false; // We're now outside a word
            }
        }
        else
        {
            if (!in_word)
            {
                in_word = true; // We're now inside a word
                word_count++;   // Increment word count
            }
        }
    }

    return word_count;
}

int count_sentences(string text)
{
    // Return the number of sentences in text
    int sentence_count = 0;

    // Compute letter count
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
    }

    return sentence_count;
}
