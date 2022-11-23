# Task 1
def update_position(coor, x, y, movement):
    '''Modify input list coor'''

    letter = movement[0]
    num = int(movement[1:])

    dx = 0
    dy = 0
    if letter == "R":
        dx = 1
    elif letter == "L":
        dx = -1
    elif letter == "U":
        dy = 1
    elif letter  == "D":
        dy = -1

    for _ in range(num):
        x = x + dx
        y = y + dy
        coor.add((x, y))

    return x, y

w1 = "R75,D30,R83,U83,L12,D49,R71,U7,L72"
w1 = w1.split(",")
coor1 = set()

w2 = "U62,R66,U55,R34,D71,R55,D58,R83"
w2 = w2.split(",")
coor2 = set()


x, y = (0,0)
for movement in w1:
    x, y = update_position(coor1, x, y, movement)
 
x, y = (0,0)
for movement in w2:
    x, y = update_position(coor2, x, y, movement)

intersections = coor1.intersection(coor2)
closest_intersection = None

for intersection in intersections:
    dist = abs(intersection[0]) + abs(intersection[1])
    if closest_intersection is None or dist < closest_intersection[1]:
        closest_intersection = (intersection, dist)



