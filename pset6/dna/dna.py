import csv
from sys import argv, exit


if len(argv) != 3:
    print("Invalid Usage.")
    exit(1)

with open(argv[1], newline='') as csvfile:
    reader = csv.reader(csvfile)
    data = list(reader)

sequences = data[0][1:]


with open(argv[2], "r") as text:
    txt = text.read()

# Implement Function which checks the current string sequence with the one theoretically afterwards


def counter(string, text):
    counter = 0
    counter_list = []
    breaker = 0
    i = 0
    index = 0

    while i < len(text):
        if text[i] == string[0]:
            checker = True
            index = i
            while checker == True:
                for j in range(0, len(string)):
                    # print(len(text), i, index, text[index], string[j], counter)
                    if index == len(text):
                        checker = False
                        break
                        
                    if text[index] == string[j]:
                        counter += 1
                        index += 1
                    else:
                        counter = 0
                        checker = False
                        break
                    if counter >= (len(string)):
                        counter_list.append(counter)
        i += 1
        
    try:
        x = int((max(counter_list)) / len(string))
    except:
        print("No match")
        exit(1)
    return x


STR_counter = []


for i in range(0, len(sequences)):
    STR_counter.append(counter(sequences[i], txt))
    
row_count = sum(1 for row in data)

for i in range(1, row_count):
    counter = 0
    for j in range(0, len(data[i][1:])):
        if (int(data[i][j + 1])) == (STR_counter[j]):
            counter += 1
        if counter == len(STR_counter):
            print(data[i][0])
            exit(0)
    
print("No match")