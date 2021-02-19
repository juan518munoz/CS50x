from cs50 import get_int

height = get_int("Height: ")

while height < 1 or height > 8:
    height = get_int("Height: ")

for y in range(1, height + 1):
    for space in range(height - y):
        print(" ", end="")
    for block in range(y):
        print("#", end="")
    print("  ", end="")
    for block in range(y):
        print("#", end="")
    print()
