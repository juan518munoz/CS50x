import sys
import csv
from cs50 import SQL

# Check correct user input
if len(sys.argv) != 2:
    print("Please specify a house")
    exit()

# Query database for students in house
db = SQL("sqlite:///students.db")


# Print student's name, birth
text = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", sys.argv[1])
for student in text:

    # First name
    print(student["first"], end =" ")
    # Middle name
    if student["middle"] != None:
        print(student["middle"], end =" ")
    # Last
    print(student["last"], end =", ")
    # Birth
    print(student["birth"])