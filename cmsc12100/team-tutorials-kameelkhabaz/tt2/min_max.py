"""
Abstraction practice for Team Tutorial 2 (Functions)
"""

def compute_frequencies(lst):
    """
    Count how often each value between 0 and M (the maximum
    value in the list) occurs in the input list.

    Inputs:
      lst (list of ints): list of integers between 0 and some upper bound M
        (inclusive), where M is expected to be relative small (say,
        less than 1000).

    Returns (list of ints): list where the ith element is the number of times
     i occurs in lst.
    """

    # allocate space to hold the lst
    frequencies = [0] * (max(lst) + 1)

    for val in lst:
        frequencies[val] = frequencies[val] + 1

    return frequencies


def find_most_frequent_values(lst):
    """
    Find the value or values (in the case of ties) that occur most
    frequently in the list.

    Inputs:
      lst (list of ints): list of integers between 0 and some upper bound M
        (inclusive), where M is expected to be relative small
        (say, less than 1000).

    Returns (list of ints): list of the int(s) that occur most frequently.
    """

    # Determine how frequently most frequent value(s) occurs.
    frequencies = compute_frequencies(lst)
    max_freq = max(frequencies)

    # Find all the values that occur max_freq times.
    return find_val(frequencies,max_freq)

def find_least_frequent_values(lst):
    """
    Find the value or values (in the case of ties) that occur least
    frequently in the list.

    Inputs:
      lst (list of ints): list of integers between 0 and some upper bound M
        (inclusive), where M is expected to be relative small
        (say, less than 1000).

    Returns (list of ints): list of the int(s) that occur least frequently.
    """

    frequencies = compute_frequencies(lst)
    #filter out values of 0
    nonzero_frequencies = []
    for v in frequencies:
        if v > 0:
            nonzero_frequencies.append(v)

    min_freq = min(nonzero_frequencies)

    # Find all the values that occur min_freq times.
    return find_val(frequencies,min_freq)

def find_val(lst,val):
    """
    Find the indices of the elements in a list 
    that match some specified value

    Input:
      lst (list): list that will be searched
      val (num): value that will be searched for
    
    Return (list): list of indices of elements
    in a list that match the specified value
    """
    rv = []
    for i, num in enumerate(lst):
        if num == val:
            rv.append(i)
    return rv