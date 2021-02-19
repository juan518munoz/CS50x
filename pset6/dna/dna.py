import csv
import sys

# Check for correct user input
if len(sys.argv) < 3:
    print ("Usage: python dna.py data.csv sequence.txt")
    sys.exit()

# Open & read DATA sequence
dat = open(sys.argv[1], "r")
dstring = csv.reader(dat)
header = next(dstring)

# Open & read sequence
sqc = open(sys.argv[2], "r")
sstring = sqc.read()

# Create base list to STR counts
sequence = []

# Read headers from database and update structure
for i in header:
    if i != "name":
        sequence.append(i)

# Check sequence maximun count
m = 0
for j in sequence:
    high = 0
    k = len(j)

    # Read sequence to assign values
    for i in range(len(sstring)):
        true = 0
        count = 0
        l = 0

        # Check until there's no match
        while true == 0:

            # Get size of sequence and string to compare
            temp = sstring[i + l: i + k + l]

            # Match found
            if j == temp:
                count += 1
                l += len(j)
                continue

            # No Match
            elif count > high:
                high = count
            true = 1

    # Assign j highest value found
    sequence[m] = high
    m += 1

# Compare sequence with subjects to determine if it's it's dna
for row in dstring:
    pos = 0
    name = 0
    match = 0
    for column in row:

        # First column is the name
        if pos == 0:
            name = column

        # Compare values
        elif int(column) == sequence[pos - 1]:
                match += 1
        pos += 1

    # Subject mathces everything
    if match == (len(header) - 1):
        print(name)
        exit()

# No match found
print("No match")