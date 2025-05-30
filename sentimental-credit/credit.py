def get_positive_long(prompt):
    while True:
        try:
            user_input = input(prompt)
            if user_input.isdigit() and int(user_input) > 0:
                return user_input
            else:
                print("Please enter a positive integer.")
        except ValueError:
            print("Please enter a valid integer.")


def main():
    # Prompt user for a positive integer
    number_str = get_positive_long("Number: ")

    # Convert the first and second characters to integers
    length = len(number_str)
    d1 = int(number_str[0])
    d2 = int(number_str[1])

    # Calculate the checksum using Luhn's algorithm
    count = 0
    is_second = False

    for i in range(length - 1, -1, -1):
        digit = int(number_str[i])

        if is_second:
            # Multiply every other digit by 2 starting with the second-to-last digit
            digit *= 2

            # If the result is two digits, sum the digits
            if digit > 9:
                digit -= 9  # Same as summing the digits of digit

        # Add the sum to the total
        count += digit

        is_second = not is_second  # Toggle flag

    # If the total's last digit is 0, the number is valid
    is_valid = (count % 10 == 0)

    # Check length and starting digits to determine the card provider
    if is_valid:
        if length == 15 and d1 == 3 and (d2 == 4 or d2 == 7):
            print("AMEX")
        elif length == 16:
            if d1 == 5 and (d2 >= 1 and d2 <= 5):
                print("MASTERCARD")
            elif d1 == 4:
                print("VISA")
            else:
                is_valid = False
        elif length == 13 and d1 == 4:
            print("VISA")
        else:
            is_valid = False
    else:
        is_valid = False

    # If not AMEX, MASTERCARD, or VISA, print INVALID
    if not is_valid:
        print("INVALID")


if __name__ == "__main__":
    main()
