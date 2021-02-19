from cs50 import get_string

number = get_string("Number: ")
# transform into int to apply luhn's algorithm
numberint = int(number)
even = 0
uneven = 0

for i in range(1, 17):
    # Get each digit
    value = int((numberint % (10 ** i)) /(10 ** (i - 1)))

    # Do the according multiplication
    if (i % 2) == 0:
        pos0 = (value * 2)
        pos1 = int(pos0 % 10)
        pos2 = int((pos0 % 100) / 10)
        even += pos1 + pos2
    else:
        uneven += value

# Total sum
output = even + uneven

# Output's last digit is not  0
if output % 10 != 0:
    print("INVALID")

# Check company

# American Express
elif int(number[0]) == 3:
    if int(number[1]) == 4 or int(number[1]) == 7:
        print("AMEX")

# VISA
elif int(number[0]) == 4:
    print("VISA")

# Master Card
elif int(number[0]) == 5:
    if 1 <= int(number[0]) <= 5:
        print("MASTERCARD")





