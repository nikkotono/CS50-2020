#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");

    int words     = 1;
    int sentences = 0;
    int letters   = 0;
    int hundredsWords = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
        if (text[i] == ' ')
        {
            words++;
        }
    }

    float avgLetters   = 100.0f * ((float)letters / (float)words);
    float avgSentences = 100.0f * ((float)sentences / (float)words);

    // printf("%f\n",avgLetters);
    // printf("%f\n",avgSentences);

    // index formula = 0.0588 * L - 0.296 * S - 15.8
    // L = average number of letters per 100 words in the text
    // S = average number of sentences per 100 words in the text

    float index = (0.0588 * avgLetters) - (0.296 * avgSentences) - 15.8;
    int   grade = round(index);

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
        printf("Grade %i\n", grade);
    }

    return 0;
}