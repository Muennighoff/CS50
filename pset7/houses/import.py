import csv
import cs50
from sys import argv, exit

# Check for correct length
if len(argv) != 2:
    print("Invalid Usage.")
    exit(1)

# This could be left out, but with it it deletes the current students.db and redoes it! -> W/o it we couldn't run the code again -> We can if
# we also comment out the CREATE TABLE line!
# open("students.db", "a")

db = cs50.SQL("sqlite:///students.db")

# Create tables in DB --> No need here, since we're supposed to use an existing one
# db.execute("CREATE TABLE students (first, middle, last, house, birth)")

# Open CSV
with open(argv[1], "r") as file:
    
    # Create DictReader
    reader = csv.DictReader(file)

    for row in reader:
        name_list = list(row["name"].split(" "))
        house = row["house"]
        birth = row["birth"]
        
        if len(name_list) == 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       name_list[0], name_list[1], name_list[2], house, birth)
        else:
            db.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)",
                       name_list[0], name_list[1], house, birth)