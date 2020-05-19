import cs50
from sys import exit


while True:
    try:
        number = cs50.get_string("CC Number, please: ")
        length = len(number)
        number_int = int(number)
        break
    except:
        print("Please put in a number.")

first_digit = int(number[0])
second_digit = int(number[1])

# Luhns Algorithm: Multiply every other digit by 2 from second-to-last and then add up !the digits of them!
# Add this sum to sum of digits that were not multiplied
# If the total's last digit is 0 (congruent to 10) it's a valid number

sum1 = 0
sum2 = 0

for i in range(length - 2, -1, -2):
    if int(number[i]) * 2 > 9:
        sum2 += sum(int(i) for i in str(int(number[i]) * 2))
    else:
        sum2 += (int(number[i]) * 2)

for i in range(length - 1, -1, -2):
    sum1 += int(number[i])


# Checking Validity
if (sum1 + sum2) % 10 != 0:
    print("INVALID")
    exit(1)

if length == 15 and first_digit == 3 and (second_digit == 7 or second_digit == 4):
    print("AMEX")
elif length == 16 and first_digit == 5 and (1 <= second_digit <= 5):
    print("MASTERCARD")
elif (length == 13 or length == 16) and first_digit == 4:
    print("VISA")
else:
    print("UNKNOWN")

