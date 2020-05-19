import cs50
import re

while True:
    try:
        s = cs50.get_string("Text: ")
        break
    except:
        print("Please enter a string.")

letters = 0
words = 1
sentences = 0

for unit in s:
    if unit.isalpha():
        letters += 1
    if unit == " ":
        words += 1
    if unit == "." or unit == "?" or unit == "!":
        sentences += 1

def Coleman_F (letters, words, sentences):

    L = letters / words * 100

    S = sentences / words * 100

    index = 0.0588 * L - 0.296 * S - 15.8

    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print("Grade", round(index))

Coleman_F (letters, words, sentences)
