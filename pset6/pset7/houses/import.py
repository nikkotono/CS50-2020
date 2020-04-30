from sys import argv, exit
import cs50
import csv

if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

csvFile = argv[1]

open('students.db','w').close()
db = cs50.SQL("sqlite:///students.db")
db.execute('CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)')

# store rows into students table inside students.db
with open(csvFile, "r") as characters:
    reader = csv.DictReader(characters)

    for row in reader:
        name  = row['name']
        house = row['house']
        birth = row['birth']

        fullName = name.split(" ")
        if len(fullName) == 3:
            first  = fullName[0]
            middle = fullName[1]
            last   = fullName[2]
        elif len(fullName) == 2:
            first  = fullName[0]
            middle = None
            last   = fullName[1]
        # print(first, middle, last, house,birth)
        db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES(?,?,?,?,?)', first, middle, last, house, birth)