from cs50 import get_int

height = 0
# get input
while height > 8 or height < 1:
    height = get_int("height: ")

for i in range(height):
    # spaces
    for j in range(height - 1 - i):
        print(" ", end="")
    # hashes
    for k in range(i+1):
        print("#", end="")
    # rows
    print("")
