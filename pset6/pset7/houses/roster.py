from sys import argv, exit
import cs50
import csv
import re

if len(argv) != 2:
    print("Usage: python roster.py Gryffindor")
    exit(1)

house = argv[1]

open('students.db','r').close()
db = cs50.SQL("sqlite:///students.db")

query = 'SELECT * FROM students WHERE students.house = \'' + house + '\' ORDER BY students.last;'
# print(query)

obj = db.execute(query)
# print(obj)

for i in obj:
    # print(i)

    for j in i.items():
        temp = str(j[1])
        key = str(j[0])
        # print(key)

        if temp == 'None':
            continue
        elif re.search('19',temp):
            print('born ' + temp, end="")
        elif re.search(house, temp):
            # print(house + ',', end=" ")
            continue
        elif key == 'last':
            print(temp + ',', end=" ")
        else:
            print(temp, end=" ")
    print()