def f(x):
    '''
    Real valued square function  f(x) == x^2
    '''

    return x*x


def integrate():
    ''' Integrate the function f using the rectangle method '''
    N = 10000
    dx = (1 - 0)/N
    total_area = 0
    
    for i in range(N):
        total_area = total_area + f(i*dx) * dx
    ### DO NOT MODIFY THE FOLLOWING LINE!
    return total_area
