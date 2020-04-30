from cs50 import get_float

# get input
change = -1.0
while(change < 0):
    change = get_float("Change owed: ")

cents = 0
cents = round(change*100)

coins = 0

# quarters
while(cents >= 25):
    coins += 1
    cents -= 25

# dimes
while(cents >= 10):
    coins += 1
    cents -= 10

# nickels
while(cents >= 5):
    coins += 1
    cents -= 5

# pennies
while(cents > 0):
    coins += 1
    cents -= 1

# print
print(coins)