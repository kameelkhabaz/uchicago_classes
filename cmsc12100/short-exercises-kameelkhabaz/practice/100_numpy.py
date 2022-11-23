# 100 numpy exercises
# https://github.com/rougier/numpy-100/blob/master/100_Numpy_exercises.md  

# 1
import numpy as np 

# 3 
v = np.zeros(10)
# or np.repeat(None, 10)

# 6
v = np.zeros(10)
v[4] = 1

# 7
v = np.arange(10,50) # inclusive 

# 8
v = np.arange(10)
v = v[::-1] # this is not a copy 

# 9
v = np.arange(9).reshape(3, 3)

# 10
a = [1,2,0,0,4,0]
v = np.argwhere(a).reshape(-1,) # reshape as 1d
# can also use np.nonzero

# 11
I = np.eye(3)

# 12
v = np.random.rand(3,3,3)

# 13
v = np.random.rand(10, 10)
min_e = v.min()
max_e = v.max()

# 14
v = np.random.rand(30)
mn = v.mean()

# 15
v = np.zeros([4,4])
v[[0, -1], :] = 1
v[:, [0, -1]] = 1
# ALTERNATIVE
v = np.ones((4, 4))
v[1:-1, 1:-1] = 0

# 16
na = np.zeros(np.array(v.shape) + 2)
na[1:-1, 1:-1] = v
# np.pad(v, pad_width = 1)

#
v = np.diag(np.arange(1, 5), k = -1)

# 19
v = np.zeros((8, 8))
v[::2, ::2] = 1
v[1::2, 1::2] = 1

# 20
np.unravel_index(100, (6, 7, 8))
# Alternatively, to find n the lement in position (3,5,3) do 
# np.ravel_multi_index((3, 5, 3), (6, 7, 8))
# 21
np.tile([[1, 0], [0, 1]], (4, 4))

# 22
v = np.random.rand(5,5)
(v - v.mean()) / (v.std())

# 24
A = np.arange(15).reshape(5, 3)
B = np.arange(6).reshape(3, 2)
np.dot(A, B)

# 25
v = np.arange(10)
v[3:9] *= -1

# 29
v = np.random.uniform(-10, 10, 10)
v = np.copysign(np.ceil(np.abs(v)),v)
#more readable but less efficient
np.where(Z>0, np.ceil(Z), np.floor(Z))

# 30
np.intersect1d(A, B)

# 35
A = np.ones(3)*1
B = np.ones(3)*2
np.add(A,B,out=B)
np.divide(A,2,out=A)
np.negative(A,out=A)
np.multiply(A,B,out=A)

# 36
r = np.random.random_sample(10) * 10
# Truncation (Extract integer part)
r1 = np.floor(r)
r2 = np.trunc(r)
r3 = r - r % 1
r4 = r // 1
# rounding
r5 = np.round(r)

# 37
m = np.random.random_sample(5 * 5) * 4
m = m.reshape((5,5))
# or 
m = np.zeros((5,5)) + np.arange(5)
m = np.tile(np.arange(0,5),(5,1))

# 38
a = np.random.randint(0,10, size=10)

# 39
v = np.random.random_sample(10)
# by default, random sample includes the starting index but not the ending index
# by default, linsapce includes both indices. We can put False to remove the endpoint
# and we can slice out the starting point
v = np.linspace(0,1,11,endpoint=False)[1:]
# 40
np.sort(v)

# 42
eq = np.all(A == B) # assume equal size, exactly equal
eq = np.allclose(A,B) # assume equal size, check if about equal
eq = np.array_equal(A, B)

# 44
X = np.random.randint(-100, 100, (10,2))
r = np.sqrt(X[:, 0] ** 2 + X[:, 1] ** 2)
theta = np.arctan(X[:,1] / X[:,0])
theta = np.arctan2(X[:,1], X[:,0]) # gets correct quadrant 
theta_deg = np.degrees(theta)
# angle in 

# 45
r = np.random.random_sample(10)
max_idx = np.argmax(r)
r[max_idx] = 0

# 46
cs = np.linspace(0, 1, 100)
# tile tiles the array with desired shape (shape is the number of repeats of the entire array)
Xs = np.tile(cs,(5,1))
Ys = np.tile(cs.reshape(5,1),(1,5))
xx, yy = np.meshgrid(cs, cs)

# 47
X = np.arange(8)
Y = X + 0.5
# The .outer method tells you to apply the function to each pair of elems in X and Y
C = 1.0 / np.subtract.outer(X, Y)
print(np.linalg.det(C))

# 50
X = np.random.random_sample(10) * 20 - 10
s = 3
closest_idx = np.abs(X - s).argmin()
closest = X[closest_idx]

# 52
Ps = np.random.random_sample(200).reshape((100,2))
Dists = np.zeros((100, 100))
for i in range(100):
    for j in range(100):
        Dists[i, j] = np.linalg.norm(Ps[i,:] - Ps[j,:])
    
# OR 
X = Ps[:,0]
Y = Ps[:,1]
D = np.sqrt((X - np.array([X]).T) ** 2 + (Y - np.array([Y]).T) ** 2)


# 55
a = np.array([[1, 2], [3, 4]])

for index, x in np.ndenumerate(a):
    print(index, x)