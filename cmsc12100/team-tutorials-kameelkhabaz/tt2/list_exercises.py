"""
CS121 Team Tutorial 2: Functions
"""

def count_twos(lst):
    """
    Count the number of twos in a list

    Input:
       lst (list): the list

    Returns (int): The number of twos in lst
    """
    count = 0
    for item in lst:
        if item == 2:
            count = count + 1
    return count

# Add your are_any_true function here
def are_any_true(lst):
    """
    Checks if at least one entry in list of booleans are True

    Inputs:
      lst (list): list of booleans 

    Returns (boolean): True if at least one entry in list is true, else False
    """

    for v in lst:
        if v:
            return True
              
    return False

# Add your add_lists function here
def add_lists(lst1,lst2):
    """
    Adds two lists element-wise

    Inputs:
      lst1 (list): first list
      lst2 (list): second list
    
    Lists must be of the same length. Corresponding elements 
    of lst1 and lst2 must be of the same time.

    Return (list): new list containing sum of corresponding 
                   values of two input lists
    """
    rlst = []
    for i, n in enumerate(lst1):
        rlst.append(n + lst2[i])
    # OR do return [a + b for a, b in zip(lst1, lst2)]
    return rlst

# Add your add_one function here
def add_one(lst):
    """
    Adds 1 to each elemnt of the list
    Modifies list in place

    Input:
      lst (list): input list to be modified
    """
    for i, n in enumerate(lst):
        lst[i] = n + 1
    
def go():
    '''
    Write code to verify that your functions work as expected here.
    Try to think of a few good examples to test your work.
    '''

    lst1 = [1, 2, 1, 0, 2, 0, 2]
    lst2 = [1, 1, 1, 0, 2, 0, 0]
    print(count_twos(lst1))
    print(count_twos(lst2))

    # Add your tests here

if __name__ == "__main__":
    go()
