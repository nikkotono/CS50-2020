#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    char *key = argv[1];

    // check reqs
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // check reqs
    for (int i = 0; i < strlen(key); i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
    }

    // find duplicate chars
    int count = 0;
    for (int i = 0; i < strlen(key); i++)
    {
        char temp = tolower(key[i]);
        count += (int)temp - 97;

        if (i > 0)
        {
            if (key[i - 1] == key[i])
            {
                printf("Cannot duplicate characters in key.\n");
                return 1;
            }
        }
    }

    // in case duplicates are non-sequential
    int letterCount = 325;

    if (count != letterCount)
    {
        printf("Cannot duplicate characters in key.\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    char *ciphertext = malloc((strlen(plaintext) + 1) * sizeof(char *));

    for (int i = 0; i < strlen(plaintext); i++)
    {
        // alphabetical index number
        int num = 0;

        // if uppercase
        if (isupper(plaintext[i]))
        {
            num = plaintext[i] - 65;
            char temp = toupper(key[num]);
            strncat(ciphertext, &temp, 1);
        }
        // if lowercase
        if (islower(plaintext[i]))
        {
            num = plaintext[i] - 97;
            char temp = tolower(key[num]);
            strncat(ciphertext, &temp, 1);
        }
        // if digit or nonalpha
        if (isdigit(plaintext[i]) || isalpha(plaintext[i]) == 0)
        {
            strncat(ciphertext, &plaintext[i], 1);
        }
    }

    // print
    printf("ciphertext: %s\n", ciphertext);
    free(ciphertext);
    return 0;
}