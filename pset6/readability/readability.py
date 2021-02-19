from cs50 import get_string

# Ask user for imput
text = get_string("Text: ")

# Counters
letter = 0
word = 1
sentence = 0

# Read string until it's end
for i in range(len(text)):

    # Check for lettersin text
    if 65 <= ord(text[i]) <= 90 or 97 <= ord(text[i]) <= 122:
        letter += 1

    # Check for end of word
    elif ord(text[i]) == 32:
        word += 1

    # Check for end of sentence
    elif ord(text[i]) == 33 or ord(text[i]) == 63 or ord(text[i]) == 46:
        sentence += 1

# Average words
L = (100 * letter) / word

# Average sentences
S = (100 * sentence) / word

# Calculate use algorithm
index = round(0.0588 * L - 0.296 * S - 15.8)

# Print grade
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")

