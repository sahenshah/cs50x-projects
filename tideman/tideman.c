#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Loop through the candidate names
    for (int i = 0; i < candidate_count; i++)
    {
        // If the name matches a candidate, record the rank
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; // Store the candidate index in the rank position
            return true;     // Successfully recorded the vote
        }
    }

    // If name is not found, return false
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate over all pairs of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // If candidate i is ranked higher than candidate j, increment preferences[i][j]
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0; // Initialize pair_count to 0 at the start.

    // Iterate over each pair of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count;
             j++) // Avoid duplicate pairs by starting j from i + 1
        {
            // If i is preferred over j, add the pair (i, j)
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++; // Increment the pair count
            }
            // If j is preferred over i, add the pair (j, i)
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++; // Increment the pair count
            }
            // If there is a tie (preferences[i][j] == preferences[j][i]), do not add any pair
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Sort pairs by strength of victory in decreasing order
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            // Calculate the strength of victory for the current pair
            int strength1 = preferences[pairs[j].winner][pairs[j].loser] -
                            preferences[pairs[j].loser][pairs[j].winner];
            int strength2 = preferences[pairs[j + 1].winner][pairs[j + 1].loser] -
                            preferences[pairs[j + 1].loser][pairs[j + 1].winner];

            // If the first pair has less strength, swap them
            if (strength1 < strength2)
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
            // If strength is equal, use alphabetical order as a tiebreaker
            else if (strength1 == strength2)
            {
                if (strcmp(candidates[pairs[j].winner], candidates[pairs[j + 1].winner]) > 0)
                {
                    pair temp = pairs[j];
                    pairs[j] = pairs[j + 1];
                    pairs[j + 1] = temp;
                }
            }
        }
    }
}

// Helper function to check if adding edge i -> j would create a cycle
bool creates_cycle(int i, int j)
{
    // If we reach the start node i while trying to go from j, there's a cycle
    if (i == j)
        return true;

    // Recursively check all locked edges
    for (int k = 0; k < candidate_count; k++)
    {
        if (locked[j][k] && creates_cycle(i, k))
        {
            return true;
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Iterate over all pairs
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Check if locking this pair would create a cycle
        if (!creates_cycle(winner, loser))
        {
            locked[winner][loser] = true; // Lock the pair
        }
    }
}

// Print the winner(s) of the election (the source of the graph)
void print_winner(void)
{
    bool has_winner = false;

    // Iterate through each candidate to find the source(s)
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;

        // Check if any candidate locks over candidate i
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i]) // If there's an edge from j to i
            {
                is_source = false; // Candidate i is not a source
                break;
            }
        }

        // If no candidate locks over i, i is a source (winner)
        if (is_source)
        {
            printf("%s\n", candidates[i]);
            has_winner = true;
        }
    }

    // If no winner is found, print a tie message
    if (!has_winner)
    {
        printf("No winner (all candidates are tied).\n");
    }
}
