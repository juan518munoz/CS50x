import sys
import csv
from cs50 import SQL

# Prompt user for correct input
if len(sys.argv) != 2:
    print("Please specify a csv datasheet")
    exit()

# Select SQL database
db = SQL("sqlite:///students.db")

# db.execute("QUERY")

# Open CSV file
with open(sys.argv[1]) as csvfile:
    csv_read = csv.DictReader(csvfile)
    for row in csv_read:

        # Split name column
        nm = row["name"].split()

        # Insert row into database
        # No middle name
        if len(nm) == 2:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", nm[0], None, nm[1], row["house"], row["birth"])

        # Has middle name
        else:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", nm[0], nm[1], nm[2], row["house"], row["birth"])
