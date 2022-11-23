"""
Short Exercises #6
"""


from tree import Tree


# Exercise 1
def sum_cubes(n):
    """
    Recursively calculates the sum of the first n positive cubes.

    Input:
        n: positive integer.
    
    Returns: (integer) the value of the sum 1^3 + 2^3 + ... + n^3.
    
    This function may not use any loops or list comprehensions.
    """
    # base case
    if n == 1:
        return 1
    else:
        # recursive case
        return n ** 3 + sum_cubes(n-1)


# Exercise 2
# this is a PERMUTATIONS PROBLEM
def sublists(lst):
    """
    Computes all sublists of the input list.
    Input:
        lst: list of values
    Returns: (list of list of values) list of all sublists of lst.
    """
    # base case
    if lst == []:
        return [[]]
    else:
        # recursive case
        prop_sublists = sublists(lst[1:]) 
        lstsapp = []
        for sublist in prop_sublists:
            # Here you NEEDD to make a new list because we are still using 
            # the original sublists (we return them)
            lstsapp.append([lst[0]] + sublist)
        return  lstsapp + prop_sublists

# Exercise 3
def min_depth_leaf(tree):
    """
    Computes the minimum depth of a leaf in the tree (length of shortest
    path from the root to a leaf).

    Input:
        tree: a Tree instance.
    
    Returns: (integer) the minimum depth of of a leaf in the tree.
    """
    # base case
    if tree.num_children() == 0:
        return 0
    else:    
        # recursive case
        depths = [min_depth_leaf(child) for child in tree.children]
        return 1 + min(depths)


# Exercise 4
def prune_tree(tree, keys_to_prune):
    '''
    Returns a new tree with that is identical to the original tree, except
    that any node whose key is in keys_to_prune is removed, along with its
    descendants. If the key of the root is in keys_to_prune, then
    return None

    Inputs:
        tree: a Tree instance.
        keys_to_prune: set of keys.
    
    Returns: (Tree) the pruned tree.
    '''
    # base case #1 
    if tree.key in keys_to_prune:
        return None

    # if no children, return the root 
    pruned_tree = Tree(tree.key, tree.value)

    # recursive case 
    for child in tree.children:
        subtree = prune_tree(child, keys_to_prune)
        if subtree is not None:
            pruned_tree.add_child(subtree)

    return pruned_tree
