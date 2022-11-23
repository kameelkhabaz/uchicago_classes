from math import sin
from tree import Tree

def is_power_of_two(n):
    '''
    Determine if number is poower of 2. Number must be a non-negative integer
    '''
    if n == 0:
        return False
    elif n <= 2:
        return True
    elif n % 2 != 0:
        return False
    
    return is_power_of_two(n / 2)


def fib(n):
    '''
    Determine nth fibonacci number.
    '''

    if n == 0 or n == 1:
        return 1
    return fib(n-1) + fib(n-2)


def find_root_sqrt2(epsilon, a, b):
    '''
    Find square root of 2 (g(x) = x^2 - 2)
    '''
    c = (a + b) / 2
    g = c ** 2 - 2
    if abs(g) < epsilon:
        # base case
        return c
    elif g > 0:
        # recursive case
        return find_root_sqrt2(epsilon, a, c)
    elif g < 0:
        # recursive case
        return find_root_sqrt2(epsilon, c, b)


t0 = Tree("node0", 27)

t1 = Tree("node0", 1)
child1 = Tree("node1", 2)
child1.add_child(Tree("node2", 3))
t1.add_child(child1)
t1.add_child(Tree("node3", 4))
t1.add_child(Tree("node4", 5))


def count_leaves(t):
    '''
    Count the number of leaves in the tree rooted at t

    Inputs: (Tree) a tree

    Returns: (integer) number of leaves in t
    '''
    
    assert t is not None

    if t.num_children() == 0:
        return 1

    num_leaves = 0
    for kid in t.children:
        num_leaves += count_leaves(kid)

    return num_leaves


def add_values(t):
    '''
    Goood docstring
    '''
    assert t is not None

    if t.num_children() == 0:
        return t.value

    sum_vals = t.value
    for kid in t.children:
        sum_vals += add_values(kid)

    return sum_vals
