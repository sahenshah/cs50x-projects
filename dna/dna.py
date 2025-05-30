import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Missing command-line argument")
        exit(1)

    # Read database file into a variable
    database_filename = sys.argv[1]
    sequence_filename = sys.argv[2]

    # Read DNA sequence file into a variable
    rows = []
    with open(database_filename, "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            rows.append(row)

    # Extract the list of STRs from the first row of the CSV file
    strs = list(rows[0].keys())[1:]  # Skip the 'name' column

    # Find longest match of each STR in DNA sequence
    with open(sequence_filename, "r") as file:
        sequence = file.read().strip()

    # Find longest match of each STR in the DNA sequence
    str_counts = {}
    for subsequence in strs:
        str_counts[subsequence] = longest_match(sequence, subsequence)

    # Check database for matching profiles
    for row in rows:
        match = True
        for subsequence in strs:
            # Compare STR counts as integers
            if int(row[subsequence]) != str_counts[subsequence]:
                match = False
                break
        if match:
            print(row["name"])
            return

    # If no match is found
    print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
