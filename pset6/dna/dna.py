from sys import argv, exit
import csv
import re
import operator

# map where we will store seq to count
counts = dict()
people = dict()
sequences = ["AGATC","TTTTTTCT","AATG","TCTAG","GATA","TATC","GAAA","TCTG"]

# import enum
# class enum(Enum)
# SeqsLarge = {
#     0 = AGATC,
#     TTTTTTCT = 1,
#     AATG = 2,
#     TCTAG = 3,
#     GATA = 4,
#     TATC = 5,
#     GAAA = 6,
#     TCTG = 7
# }

# class SeqsSmall(Enum):
#     AGATC = 0
#     AATG = 1
#     TATC = 2

def main():
    # check proper usage
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)


    # read DNA sequence into 'dna'
    sequence = argv[2]
    with open(sequence, "r") as DNA:
        DNAReader = csv.DictReader(DNA)

        # store sequence inside dna
        for dnaRow in DNA:
            dna = dnaRow


    # read names csv file and add into 'people'
    filename = argv[1]
    with open(filename,'r') as userDB:
        reader = csv.DictReader(userDB)

        for row in reader:

            name     = row["name"]
            AGATC    = row["AGATC"]
            AATG     = row["AATG"]
            TATC     = row["TATC"]

            if re.search("large",filename):
                TTTTTTCT = row["TTTTTTCT"]
                TCTAG    = row["TCTAG"]
                GATA     = row["GATA"]
                GAAA     = row["GAAA"]
                TCTG     = row["TCTG"]
                # AGATC,TTTTTTCT,AATG,TCTAG,GATA,TATC,GAAA,TCTG
                people[name] = AGATC + "," + TTTTTTCT + "," + AATG + "," + TCTAG + "," + GATA + "," + TATC + "," + GAAA + "," + TCTG

            else:
                people[name] = AGATC + "," + AATG + "," + TATC

    # initializing to zero so we can check keys inside counts
    for keys in sequences:
        counts[keys] = 0

    # count sequence runs in dna sequence and tally them in 'counts'
    for keys in sequences:
        temp = 0
        max_ = 0
        keyLength = len(keys)

        # iterate through whole dna strand
        for i in range(len(dna)):

            # reset temp count in case of not counting highest STR
            while temp > 0:
                temp -= 1
                continue

            # if we found a repeated STR
            if dna[i: keyLength + i] == keys:
                # if they are sequential, add one to temp count, jump i to beggining of next dna block
                while dna[i - keyLength: i] == dna[i: keyLength + i]:
                    temp += 1
                    i += keyLength

                # get highest temp count into max_
                if temp > max_:
                    max_ = temp

        # we first initialized to zero. now we are certain we found first key
        counts[keys] += 1

        # add max_ count to dict
        if keys in counts:
            counts[keys] += max_
        else:
            counts[keys] = 1

    # print (counts)
    # print (people)


    # compare STR with people
    for person in people.items():

        if re.search("large",filename):
            largeSeq = person[1].split(',')

            #AGATC,TTTTTTCT,AATG,TCTAG,GATA,TATC,GAAA,TCTG
            if str(counts['AGATC']) == str(largeSeq[0]) and str(counts['TTTTTTCT']) == str(largeSeq[1]) and str(counts['AATG']) == str(largeSeq[2]) and str(counts['TCTAG']) == str(largeSeq[3]) and str(counts['GATA']) == str(largeSeq[4]) and str(counts['TATC']) == str(largeSeq[5])  and str(counts['GAAA']) == str(largeSeq[6])  and str(counts['TCTG']) == str(largeSeq[7]):
                print(person[0])
                exit(0)

        elif re.search("small",filename):
            smallSeq = person[1].split(',')

            if str(counts['AGATC']) == str(smallSeq[0]) and str(counts['AATG']) == str(smallSeq[1]) and str(counts['TATC']) == str(smallSeq[2]):
                print(person[0])
                exit(0)
    # print(SeqsLarge(1))


main()
print('no match')
# print (people, sep=" | ")
# print (counts)

