nums = [35, 20, 15, 25, 47, 40, 62, 55, 65,95, 102, 117, 150, 182, 127, 219, 299, 277, 309, 576]
prev_nums = [35, 20, 15, 25, 47] 
nums_check = [40, 62, 55, 65,95, 102, 117, 150, 182, 127, 219, 299, 277, 309, 576]

nums = []
with open('2020_day10_puzzle_input.txt','r') as file:
    for row in file:
        nums.append(int(row.strip())) #remember to strip line and remove '\n'

# split
prev_nums = nums[:25]
nums_check = nums[25:]

# Task 1
def check_sum(prev_nums, nums_check):
    ''' 
    Return first number in nums_check that is NOT sum of 2 of prev nums before it
    Inputs:
      prev_nums (list): sliding list of previous numbers that we check
      nums_check (list): list of numbers that we need to check
    '''
    for n in nums_check:
        # See if n is the sum of two of the previous numbers 
        # Will check if n - c1 - c2 == 0 
        sum_prev_two = False
        for c1 in prev_nums:
            for c2 in prev_nums:
                if c1 == c2:
                    continue
                if n - c1 - c2 == 0:
                    sum_prev_two = True
                    break
        if not sum_prev_two:
            return n
        prev_nums.pop(0) #pop removes the index, remove removes the value
        prev_nums.append(n)
# Better Task 1
def check_sum(prev_nums, nums_check):
    ''' 
    Return first number in nums_check that is NOT sum of 2 of prev nums before it
    Inputs:
      prev_nums (list): sliding list of previous numbers that we check
      nums_check (list): list of numbers that we need to check
    '''
    for n in nums_check:
        # See if n is the sum of two of the previous numbers 
        # Will check if n - c1 - c2 == 0 
        # Here we improved the efficiency by a factor of 2
        sum_prev_two = False
        for idx1, c1 in enumerate(prev_nums):
            for idx2 in range(idx1 + 1, len(prev_nums)):
                c2 = prev_nums[idx2]
                if c1 == c2:
                    continue
                if n - c1 - c2 == 0:
                    sum_prev_two = True
                    break
        if not sum_prev_two:
            return n
        prev_nums.pop(0) #pop removes the index, remove removes the value
        prev_nums.append(n)

# Task 2 
def encryption_weakness(nums,invalid_num):
    '''
    Find encryption weakness
    Inputs:
      nums (list):
      invalid_num (float):

    Returns (num): minimum plus maximum of contiguous list 
    '''
    num_nums = len(nums)
    for start_pos in range(num_nums):
        s = 0
        contiguous_lst = []
        # Note that we use list indices instead of slicing the list
        # to avoid making too many copies of the list
        for idx in range(start_pos, num_nums):
            s += nums[idx] 
            contiguous_lst.append(nums[idx])
            if s == invalid_num:
                return min(contiguous_lst) + max(contiguous_lst)
            if s > invalid_num:
                break

# Task 2 better 
def encryption_weakness(nums,invalid_num):
    '''
    Find encryption weakness
    Inputs:
      nums (list):
      invalid_num (float):

    Returns (num): minimum plus maximum of contiguous list 
    '''
    num_nums = len(nums)
    for start_pos in range(num_nums):
        s = 0
        min_n = None
        max_n = None
        # Note that we use list indices instead of slicing the list
        # to avoid making too many copies of the list
        for idx in range(start_pos, num_nums):
            s += nums[idx] 
            contiguous_lst.append(nums[idx])
            if s == invalid_num:
                return min(contiguous_lst) + max(contiguous_lst)
            if s > invalid_num:
                break