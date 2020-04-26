#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // look for a candidate called name
    // if candidate found, update preferences so that they are the voter's rank preference and return true
    // if no candidate found, dont update any preferences, return false.

    // every voter
    for (int i = 0; i < voter_count; i++)
    {
        // every rank
        for (int j = 0; j < candidate_count; j++)
        {
            if (strcmp(candidates[j].name, name) == 0)
            {
                preferences[voter][rank] = j;
                // printf("candidates[j].name = %s, name = %s, preferences[i] = %d, voter = %d,rank = %d, preferences[i][j] = %d\n",candidates[j].name,name,*preferences[i],voter,rank,preferences[i][j]);
                return true;
            }
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    // update vote counts for all non eliminated candidates
    // go through each of the voters and update the vote counts
    // making sure to only update vote counts for candidates who are still in the race

    // every voter
    for (int i = 0; i < voter_count; i++)
    {
        // every rank
        for (int j = 0; j < candidate_count; j++)
        {
            // get candidate ID
            int cndId = preferences[i][j];

            // if candidate hasnt been eliminated
            if (candidates[cndId].eliminated == false)
            {
                candidates[cndId].votes++;
                break;
            }
        }
    }

    // //// test print preferences array
    // for(int i = 0; i < voter_count; i++)
    // {
    //     for(int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%d ",preferences[i][j]);
    //     }
    //     printf("\n");
    // }

    // //// test print candidates array
    // for(int i = 0; i < candidate_count; i++)
    // {
    //     printf("name: %s\n",candidates[i].name);
    //     printf("votes: %d\n",candidates[i].votes);
    // }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    // if any candidate has more than half the vote,
    // print out their name and return true
    // if nobody has won, return false

    char *winner;
    int majority = (voter_count / 2.0) + 0.5;

    // printf("majority is %d\n",majority);

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > majority)
        {
            winner = candidates[i].name;
            printf("%s\n", winner);
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO

    int min = candidates[0].votes;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < min && !candidates[i].eliminated)
        {
            min = candidates[i].votes;
        }
    }

    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    bool tie = true;
    int notEliminated = 0;
    int candidateMin = 0;

    for (int i = 1; i < candidate_count; i++)
    {
        if (!candidates[i - 1].eliminated)
        {
            notEliminated++;

            if (candidates[i - 1].votes == min)
            {
                candidateMin++;
            }
        }
    }

    if (notEliminated != candidateMin)
    {
        tie = false;
    }

    return tie;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    // TODO
    // eliminate anyone who is still in the race who has the min number of votes

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}
