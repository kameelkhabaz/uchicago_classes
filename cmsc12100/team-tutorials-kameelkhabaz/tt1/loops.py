# This file is intentionally empty
#
# Feel free to add your code from the "Simple practice: Loops" section
# of the tutorial in this file.
lst3 = [7, 5, 6, 0, -23, 1, 14]

all_pos = True

for elem in lst3:
    all_pos = all_pos and (elem > 0)

print(all_pos)

pos_only = []

for elem in lst3:
    if elem > 0:
        pos_only.append(elem)
  
print(pos_only)

is_pos = []

for elem in lst3:
    if elem > 0:
        is_pos.append(True)
    else:
        is_pos.append(False)

print(is_pos)


lst4 = [0, 1, 1, 3, 2, 4, 6, 1, 7, 8]
M = max(lst4)

counts = [0] * (M + 1)

for elem in lst4:
    counts[elem] = counts[elem] + 1

print(counts)

