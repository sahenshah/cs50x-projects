def get_positive_int(prompt):
    while True:
        try:
            n = int(input(prompt))
            if 1 <= n <= 8:
                return n
            else:
                print("Please enter a positive integer between 1 and 8, inclusive.")
        except ValueError:
            print("Please enter a valid integer.")


def main():
    # Prompt user for positive integer between 1 and 8
    height = get_positive_int("Height: ")

    # Print the half-pyramids
    for i in range(1, height + 1):
        # Print spaces for the left pyramid
        for j in range(height - i):
            print(" ", end="")

        # Print hashes for the left pyramid
        for j in range(i):
            print("#", end="")

        # Print the gap (2 spaces)
        print("  ", end="")

        # Print hashes for the right pyramid
        for j in range(i):
            print("#", end="")

        # Move to the next line
        print()


if __name__ == "__main__":
    main()
