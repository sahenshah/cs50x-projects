import re


def count_letters(text):
    # Return the number of letters in text
    letter_count = 0

    # Compute letter count
    for char in text:
        if char.isalpha():
            letter_count += 1

    return letter_count


def count_words(text):
    # Return the number of words in text
    # Split the text by whitespace and count the resulting list length
    words = text.split()
    return len(words)


def count_sentences(text):
    # Return the number of sentences in text
    # Use regex to count occurrences of '.', '!', or '?'
    sentence_count = len(re.findall(r'[.!?]', text))
    return sentence_count


def main():
    # Prompt the user for some text
    text = input("Text: ")

    # Count the number of letters, words, and sentences in the text
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculate L and S for the Coleman-Liau index
    L = (letters * 100.0) / words
    S = (sentences * 100.0) / words

    # Compute the Coleman-Liau index
    index = (0.0588 * L) - (0.296 * S) - 15.8

    # Handle rounding
    grade = round(index)

    # Print the grade level
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


if __name__ == "__main__":
    main()
