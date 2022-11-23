# pg 2
def a(x, y):
    print("a0", x, y)
    w = b(y)
    print("w", w)
    w = b(x) - w
    print("w", w)
    print("a1", x, y)
def b(x):
    print("b0", x)
    x = c(x) + 10
    print("b1", x)
    return x
def c(z):
    print("c0", z)
    if z % 2 == 0:
        return z // 2
    else:
        return 3 * z + 1
    print("c1", z)

# pg 3
d = {"Alice": [0],
     "Bob": [4, 2],
     "Charlie": [0],
     "Foods": ["apple", "banana",
               "carrot", "olive",
               "pear", "raspberry"]}
def f(x):
    lst = d.get(x, [3])
    print("x", x)
    for i in lst:
        print(i, d["Foods"][i])
        del d["Foods"][i]
f("Bob")
f("Denny")
f("Alice")
f("Charlie")
print(d["Foods"])

# pg 4
class Mystery(object):
    def __init__(self, a, b):
        self.a = a
        self.b = b
        self.c = 0
        self.d = []
    def one(self, a, b):
        x = self.a + self.b
        self.a = a
        self.b = b
        self.c += 1
        return x + self.c
    def two(self, q):
        self.d.append(q)
        return len(self.d)
    def three(self):
        return sum(self.d)
m_A = Mystery(3, 5)
m_B = Mystery(100, 200)
print(m_A.one(10, 7))
print(m_A.one(2, 3))
print(m_B.one(1, 6))
print(m_B.two(10))
print(m_B.two(20))
print(m_A.three())
print(m_B.three())
# pg 5
class MysteryA(object):
    def __init__(self, a, b):
        self.a = (a, b)
    def one(self):
        self.a = (self.a[0] + 1, self.a[1])
    def __str__(self):
        return str(self.a)
class MysteryB(object):
    def __init__(self, a, b):
        self.a = a
        self.b = b
        self.c = []
    def one(self, c):
        for i in range(c):
            d = MysteryA(self.a, self.b + i)
            self.c.append(d)
    def two(self):
        for f in self.c:
            print(f)
    def three(self, c, d):
        self.c[d].one()
x = MysteryB(1, 2)
y = MysteryB(3, 4)
x.one(2)
x.two()
x.three(3, 1)
y.one(3)
y.two()
y.three(3, 2)
x.two()
y.two()

x.one(2)
# pg 6
def mystery(x):
    if x == 1:
        return [1]
    else:
        output = [1]
        prev = mystery(x - 1)
        for i in range(1, len(prev)):
            output.append(prev[i - 1] + prev[i])
        output.append(1)
    return output
for i in range(1,6):
    print(mystery(i))

# pg 7
def r(n):
    if n % 2 == 1:
        return 0
    else:
        return 1 + r(n // 2)
print(r(20))
print(r(21))
print(r(24))

# pg 8
def mystery(l):
    n = len(l)
    if n == 0:
        return []
    middle = n // 2
    front = mystery(l[:middle])
    back = mystery(l[middle+1:])
    if n % 2 == 1:
        return front + [l[middle]] + back
    else:
        return front + back
print(mystery([11, 12, 13, 14, 15]))
print(mystery([11, 12, 13, 14, 15, 16]))

# pg 9
def replace_in_place(lst, a, b):
    """
    Replaces every occurrence of a given value in a list with
    another value.
    Parameters:
    - lst: List of integers. Note: This function will modify this
           list in-place (every occurrence of parameter a will be
           replaced with the value of parameter b)
    - a, b: Integers
    Returns: Nothing
    Example:
      >>> lst = [1, 10, 2, 10, 3]
      >>> replace_in_place(lst, 10, -1)
      >>> print(lst)
      [1, -1, 2, -1, 3]
    """
    for i, n in enumerate(lst):
        if n == a:
            lst[i] = b

# pg 10

def replace(lst, a, b):
    """
    Takes a list and creates a new list where every occurrence
    of a given value has been replaced by another value.
    Parameters:
    - lst: List of integers.
    - a, b: Integers.
    Returns: A new list where every occurrence of a in lst has
             been replaced with b
    Example:
      >>> lst = [1,10,2,10,3]
      >>> replace(lst, 10, -1)
      [1, -1, 2, -1, 3]
      >>> print(lst)
      [1, 10, 2, 10, 3]
    """
    rlst = []
    for n in lst:
        if n == a:
            rlst.append(b)
        else:
            rlst.append(n)
    return rlst

def count_incremen_window(lst, win_size):
    """
    Goes through list with sliding win
    Counts # times sum of vals in win larger
    than prev win
    
    Input:
      lst (list of ints): List of values
      win_size (int): Window size
    Returns:
      incr (int): # times the sum of values
        in win is larger than 
        prev win.
    """    
    prev = None
    incr = 0
    for i in range(len(lst)- win_size+1):
        win = lst[i : i + win_size]
        sum_win = sum(win)
        if prev is not None:
            if sum_win > prev:
                incr += 1
        prev = sum_win

    return incr