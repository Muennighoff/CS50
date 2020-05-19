import cs50


height = cs50.get_int("Height: ")

if height > 8 or height <= 0:
    print("Invalid height.")
    height = cs50.get_int("Height: ")
    
for i in range(height):
    print(" " * (height - (int(i) + 1)) + "#" * (int(i) + 1) + "  " + "#" * (int(i) + 1))
    
    
    
