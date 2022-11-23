"""
CS 121
Short Exercises #2
Kameel Khabaz
"""

def peep(p, e):
    """
    Determine whether or not peep = pp^e

    Inputs:
      p (int): first digit
      e (int): second digit

    Returns: True if peep = pp^e, False otherwise
    """

    ### EXERCISE 1 -- Replace pass with your code
    peep = p + e * 10 + e * 10 ** 2 + p * 10 ** 3
    ppe = (p + p * 10) ** e

    if peep == ppe:
      return True
    
    return False

def has_more(lst1, lst2, target):
    """
    Determine which list contains more of the target value

    Inputs:
      lst1 (list): first list
      lst2 (list): second list
      target: the target value

    Returns: True if lst1 contains more of target, False otherwise
    """

    ### EXERCISE 2 -- Replace pass with your code
    n1 = count_num_occur(lst1,target)
    n2 = count_num_occur(lst2,target)

    if n1 > n2:
        return True

    return False

def count_num_occur(lst, target):
    """
    Count the number of times the target value occurs in a list

    Inputs:
      lst1 (list): list
      target (num): target value
    
    Returns (num): number of times target occurs in lst1
    """

    rv = 0
    for n in lst:
        if n == target:
          rv += 1
    
    return rv

def make_star_strings(lst):
    """
    Create a list of star strings

    Input:
      lst (list of nonnegative integers): the list

    Returns: A list of strings of stars (*)
    """

    ### EXERCISE 3 -- Replace pass with your code
    character = "*"

    rlst = []
    for num in lst:
        str = ""
        for i in range(num):
            str += character
            
        rlst.append(str)
    return rlst

def replace(lst, replacee, replacer):
    """
    Replace one element in a list with another

    Input:
      lst (list): the list
      replacee: the element to replace
      replacer: the element to replace replacee with

    Returns: None, modifies lst in-place
    """

    ### EXERCISE 4 -- Replace pass with your code
    
    for i,n in enumerate(lst):
      if n == replacee:
        lst[i] = replacer

def rows_and_columns_contain(lst, target):
    """
    Determines whether every row and every column of a list
      of lists contains a target value

    lst (list of lists): the list of lists
    target: the target value

    Returns: True if every row and every column of lst contains
      target, False otherwise
    """

    ### EXERCISE 5 -- Replace pass with your code

    lstT = make_transpose(lst)

    if not rows_contain(lst,target):
        return False
      
    if not rows_contain(lstT,target):
        return False

    return True

def rows_contain(lst,target):
    """
    Determines whether every row of a list
      of lists contains a target value

    lst (list of lists): the list of lists
    target: the target value

    Returns: True if every row of lst contains
      target, False otherwise
    """

    for row in lst:
        if target not in row:
            return False

    return True

def make_transpose(lst):
    """
    Makes a transpose of a list of lists

    Inputs:
        lst (list): list of lists, can be
        represented as a m x n matrix.
    
    Returns: list of lists that is the transpose
    """

    ncol = len(lst[0])

    lstT = []  #essentially the transpose
    #Preallocating a list of columns here
    for c in range(ncol):
        lstT.append([])

    for row in lst:
        for i,n in enumerate(row):
            lstT[i].append(n)
    return lstT
    



    
