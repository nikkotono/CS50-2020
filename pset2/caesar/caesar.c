#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // usage reqs
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    char *cipher = argv[1];

    // usage reqs
    for (int i = 0; i < strlen(cipher); i++)
    {
        if (isdigit(cipher[i]) == 0)
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }

    }

    int num = atoi(argv[1]);

    // allocate mem
    string plaintext = get_string("plaintext: ");
    char *ciphertext = malloc((strlen(plaintext) + 1) * sizeof(char *));
    char toCipher;

    // iterate through string's chars
    for (int i = 0; i < strlen(plaintext); i++)
    {
        toCipher = plaintext[i];

        // if digit or non alphabetical
        if (isdigit(plaintext[i]) || isalpha(plaintext[i]) == 0)
        {
            strncat(ciphertext, &toCipher, 1);
        }
        // if uppercase
        if (isupper(plaintext[i]))
        {
            toCipher = ((((toCipher + num) - 65) % 26) + 65);
            strncat(ciphertext, &toCipher, 1);
        }
        //if lowercase
        else if (islower(plaintext[i]))
        {
            toCipher = ((((toCipher + num) - 97) % 26) + 97);
            strncat(ciphertext, &toCipher, 1);
        }
    }

    printf("ciphertext: %s\n", ciphertext);
    free(ciphertext);
    return 0;
}