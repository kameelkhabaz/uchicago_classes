# Task 1
num_yes = 0 # NO NEED FOR LIST
group = set()

with open("2020_day6_puzzle_input.txt") as file:
    for line in file:
        if line.strip() == "":
            num_yes += len(group)
            group = set()
        else:
            group.update(set([s for s in line.strip()]))
            
num_yes += (len(group)
print(sum(num_yes))

# Task 2
num_yes = 0
group = set()
prev_line = ""
with open("2020_day6_puzzle_input.txt") as file:
    for line in file:
        if line.strip() == "":
            num_yes += len(group)
            group = set()
        else:
            if prev_line == "":
                group = set([s for s in line.strip()])
            else:
                group = group.intersection(set([s for s in line.strip()]))
        prev_line = line.strip()
            
num_yes += len(group)
print(sum(num_yes))