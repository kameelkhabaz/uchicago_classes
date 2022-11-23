# 2020 Day 10

# Task 1
jolt_diffs = {} # dictionary mapping each jolt difference to number of times that appears


jolts = [16, 10, 15, 5, 1, 11, 7, 19, 6, 12 ,4] # Would be imported from file
jolts = [28,33,18,42,31,14,46,20,48,47,24,23,49,45,19,38,39,11,1,32,25,35,8,17,7,9,4,2,34,10,3]
jolts = []
with open('2020_day10_puzzle_input.txt','r') as file:
    for row in file:
        jolts.append(int(row.strip()))

jolts.sort() # must sort list 
jolts.append(jolts[-1] + 3) # add 3 to end 

def task1(jolts):
    for idx, jolt in enumerate(jolts):
        if idx == 0:
            diff = jolt
        else:
            diff = jolt - jolts[idx - 1]

        jolt_diffs[diff] = jolt_diffs.get(diff, 0) + 1

    return jolt_diffs[1] * jolt_diffs[3]

def task1_better(jolts):
    # take in jolts with 0 added as first element, cleaner code
    for idx, jolt in enumerate(jolts):
        diff = jolt - jolts[idx - 1]
        jolt_diffs[diff] = jolt_diffs.get(diff, 0) + 1

    return jolt_diffs[1] * jolt_diffs[3]

def task2_generate_BAD(jolts, pre):
    '''
    Generate all possible arrangements
    jolts must be sorted
    return list of lists
    '''

    if len(jolts) == 1:
        # base case
        # return a list of the jolts list (list of list)
        return [jolts]
    arrangements = []
    for idx, jolt in enumerate(jolts):
        if jolt - pre <= 3 and idx + 1 < len(jolts):
            arrs = task2_generate(jolts[idx + 1:], jolt)
            for arr in arrs:
                # appends jolt to list
                arrangements.append([jolt] + arr)

    arrangements.append([jolts[-1]]) #need to add very last element of list, clean up recursin to fix base case to do this properly not working now 

    return arrangements

def task2_generate(jolts, pre):
    '''
    Generate all possible arrangements
    jolts must be sorted
    return list of lists
    '''
    if len(jolts) == 0:
        # base case
        # return a list of the jolts list (list of list)
        return [[]]

    arrangements = []
    for idx, jolt in enumerate(jolts):
        if jolt - pre <= 3:
            arrs = task2_generate(jolts[idx + 1:], jolt)
            for arr in arrs:
                # appends jolt to list
                arrangements.append([jolt] + arr)
        else:
            # YOU NEED THE ELSE HERE FOR EFFICIENCY
            # IF DIFFERENCE IS GREATER THAN 3 then no need to check other jolts
            # this is because since the jolts array is sorted, every value after the current jolt
            # will be larger than the current jolt and thus will be larger than the pre value 
            break 
    return arrangements

def task2_counting(jolts, pre, next_idx):
    # returns a count of num of distinct arrangements
    if next_idx >= len(jolts) - 1:
        # base case
        # if next_idx is len(jolts) - 1, that is invalid because next index will be empty and so count will be 0
        return 1
    count = 0
    for idx in range(next_idx, len(jolts)):
        jolt = jolts[idx]
        # recursive case 
        if jolt - pre <= 3:
            count += task2_counting(jolts, jolt, idx + 1)
        else:
            # remember else here
            break
    return count


# The actual best solution uses memoization, which stores solutions when sequences are identical
def task2_counting_memo(jolts, start_jolt, next_idx, memo):
    # memo is dict with key as (next_idx, start_jolt) and value as count 
    # returns a count of num of distinct arrangements

    if (next_idx, start_jolt) in memo:
        # check if this is already stored
        return memo[(next_idx, start_jolt)]

    if next_idx >= len(jolts) - 1:
        # base case
        # if next_idx is len(jolts) - 1, that is invalid because next index will be empty and so count will be 0
        return 1

    count = 0
    for idx in range(next_idx, len(jolts)):
        jolt = jolts[idx]
        # recursive case 
        if jolt - start_jolt <= 3:
            count += task2_counting(jolts, jolt, idx + 1)
        else:
            # remember else here
            break

    memo[(next_idx, start_jolt)] = count

    return count