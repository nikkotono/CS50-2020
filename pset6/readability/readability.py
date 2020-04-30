from cs50 import get_string

# get input
text = get_string("Text: ")

words = 1
sentences = 0
letters = 0
hundredsWords = 0

# count chars in string
for i in text:
    if (i.isalpha()):
        letters += 1
    if (i == '.' or i == '!' or i == '?'):
        sentences += 1
    if (i == ' '):
        words += 1

# calculations
avgLetters = 100 * float(letters) / float(words)
avgSentences = 100 * float(sentences) / float(words)

index = (0.0588 * avgLetters) - (0.296 * avgSentences) - 15.8
grade = round(index)

# print
if(index < 1):
    print("Before Grade 1")
elif(index > 16):
    print("Grade 16+")
else:
    print("Grade " + str(grade))
