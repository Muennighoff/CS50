import cs50
from sys import argv, exit


# Check for correct length
if len(argv) != 2:
    print("Invalid Usage.")
    exit(1)

db = cs50.SQL("sqlite:///students.db")

list_results = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

# Iterating through the profiles
for index in range(0, len(list_results)):

    # Getting all the keys & values from each profile
    for key, value in list_results[index].items():

        # Formatting & printing
        if key == "birth":
            print(", born {}".format(value))

        elif key == "last":
            print(value, end="")

        elif value != None:
            print(value, end=" ")
