# task1 

def task1(start_nums,num_steps):
    '''
    Find the spoken number at the step num_steps given the starting numbers

    Inputs:
      start_nums (list): a list of integers
      num_steps (int): the maximum step
    
    Returns (int): the last spoken number
    '''
    # Dictionary maps each number to a tuple of the last 2 times it was spoken
    # No need to story any more than that
    nums = {}
    # Initialize the starting numbers
    for idx, n in enumerate(start_nums):
        nums[n] = (idx + 1, None)
    last_num = start_nums[-1]
    start_turn = len(start_nums) + 1

    for turn in range(start_turn, num_steps + 1):
        last1, last2 = nums.get(last_num, (None, None))
        # Check if the number last number spoken has been spoken before 
        if last2 is None:
            last_num = 0
        else:
            last_num = last1 - last2
        # Update history of the new number
        last1, _ = nums.get(last_num, (None, None))
        nums[last_num] = (turn, last1)
    return last_num