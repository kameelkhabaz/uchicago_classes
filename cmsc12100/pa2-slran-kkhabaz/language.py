
"""
CS 121: Language shifts

Kameel Khabaz
Steph Ran

Functions for language shift simulation.

This program takes the following parameters:
    grid _file (string): the name of a file containing a sample region
    R (int): neighborhood radius
    A (float): the language state transition threshold A
    Bs (list of floats): a list of the transition thresholds B to
      use in the simulation
    C (float): the language state transition threshold C
      centers (list of tuples): a list of community centers in the
      region
    max_steps (int): maximum number of steps

Example use:
    $ python3 language.py -grid_file tests/writeup-grid-with-cc.txt
	  --r 2 --a 0.5 --b 0.9 --c 1.2 --max_steps 5
While shown on two lines, the above should be entered as a single command.
"""

import copy
import click
import utility

def check_proximity_com_center(centers, home_loc):
    """
    Determine if a SL-speaking home is within the service distance of a
      community center.

    Inputs:
      centers (list of tuples): a list of community centers in the 
        region
      home_loc (int,int): a tuple of the position of the SL-speaking home

    Returns (boolean): True if home is within service distance of 
      a community center.
    """
    home_x, home_y = home_loc

    for (center_x,center_y), service_dist in centers:
        # Check if the home is within the vicinity of the service center
        if (abs(center_x - home_x) <= service_dist and 
            abs(center_y - home_y) <= service_dist):
            return True

    return False

def determine_engagement_level(grid, R, home_loc):
    """
    Determine the engagement level of the home at a specified location

    Inputs:
      grid (list of lists of ints): the grid
      R (int): neighborhood radius
      home_loc (int,int): a tuple of the position of the home
    
    Returns (int): engagement level of the home
    """

    home_x, home_y = home_loc
    grid_size = len(grid)
    
    # Determine the neighborhood bounds
    start_x = max(home_x - R, 0)
    start_y = max(home_y - R, 0)

    end_x = min(home_x + R, grid_size-1)
    end_y = min(home_y + R, grid_size-1)

    S = 0  # sum of language preferences
    T = 0  # total number of homes in region

    for x in range(start_x, end_x + 1):
        for y in range(start_y, end_y + 1):
            T += 1
            S += grid[x][y]

    return S/T

def transmit_gen_com_center(grid, R, thresholds, centers, home_loc):
    """
    Determine the language state of the next generation of 
    speakers in a home according to the transmission rules, while 
    considering proximity to a community center.

    Inputs:
      grid (list of lists of ints): the grid
      R (int): neighborhood radius
      thresholds (float, float, float): the language
        state transition thresholds (A, B, C)
      centers (list of tuples): a list of community centers in the
        region
      home_loc (int,int): a tuple of the position of the home

    Returns (int): The home's language state at the next generation
    """

    home_x, home_y = home_loc
    current_state = grid[home_x][home_y]

    proposed_state = transmit_gen(grid, R, thresholds, home_loc)

    # If state will decrease, check proximity to community center
    if (proposed_state < current_state and 
        check_proximity_com_center(centers, home_loc)):
        return current_state

    return proposed_state

def transmit_gen(grid,R,thresholds,home_loc):
    """
    Determine the language state of the next generation of 
    speakers in a home according to the transmission rules, without 
    considering proximity to a community center.

    Inputs:
      grid (list of lists of ints): the grid
      R (int): neighborhood radius
      thresholds (float, float, float): the language
        state transition thresholds (A, B, C)
      home_loc (int,int): a tuple of the position of the home

    Returns (int): updated language state of the speakers in the home
    """
    
    home_x, home_y = home_loc
    home_state = grid[home_x][home_y]

    engagement_level = determine_engagement_level(grid, R, home_loc)
    
    # Check parent state and engagement level to determine child state
    if home_state == 0:
        # State of DL home only depends on engagement level
        return 0 if engagement_level <= thresholds[1] else 1

    elif home_state == 1:
        # State of bilingual homes can't decrease if close to a
        # community center
        if engagement_level < thresholds[1]:
            return 0
        elif engagement_level <= thresholds[2]:
            return home_state
        return 2

    elif home_state == 2:
        # State of SL homes can't decrease if close to a
        # community center
        if engagement_level <= thresholds[0]:
            return 0
        elif engagement_level < thresholds[1]:
            return 1
        return 2


def run_one_step(grid, R, thresholds, centers):
    """
    Run one step of the simulation and determine if grid has changed.

    Inputs:
      grid (list of lists of ints): the grid, modified in place
      R (int): neighborhood radius
      thresholds (float, float, float): the language
        state transition thresholds (A, B, C)
      centers (list of tuples): a list of community centers in the
        region

    retrun (boolean): True if the grid has changed in this step
    """

    grid_size = len(grid)
    grid_has_changed = False
    # Loop through the grid and update each home's state
    for x in range(grid_size):
        for y in range(grid_size):
            home_loc = (x,y)
            old_state = grid[x][y]
            grid[x][y] = transmit_gen_com_center(grid, R, \
              thresholds, centers, home_loc)
            if old_state != grid[x][y]:
                grid_has_changed = True
    
    return grid_has_changed
  
def count_states(grid):
    """
    Count the number of homes in each state.

    Inputs:
        grid (list of lists of ints): the grid
    
    Return (int,int,int): tuple with the frequency of each language state
    """
    tot_0 = 0
    tot_1 = 0
    tot_2 = 0

    for row in grid:
        for value in row:
            if value == 0:
                tot_0 += 1
            elif value == 1:
                tot_1 += 1
            else:
                tot_2 += 1
    
    return (tot_0, tot_1, tot_2)


def run_simulation(grid, R, thresholds, centers, max_steps):
    """
    Do the simulation.

    Inputs:
      grid (list of lists of ints): the grid, modified in place
      R (int): neighborhood radius
      thresholds (float, float, float): the language
        state transition thresholds (A, B, C)
      centers (list of tuples): a list of community centers in the
        region
      max_steps (int): maximum number of steps

  
    Returns (int, int, int): tuple with the frequency of each language state
    """

    step = 0
    grid_has_changed = True
    # Run the simulation until one of two stopping criterions are reached
    while step < max_steps and grid_has_changed:
        step += 1
        grid_has_changed = run_one_step(grid, R, thresholds, centers)
        
    return count_states(grid)

def simulation_sweep(grid, R, A, Bs, C, centers, max_steps):
    """
    Run the simulation with various values of threshold B.

    Inputs:
      grid (list of lists of ints): the grid
      R (int): neighborhood radius
      A (float): the language state transition threshold A
      Bs (list of floats): a list of the transition thresholds B to
        use in the simulation
      C (float): the language state transition threshold C
      centers (list of tuples): a list of community centers in the
        region
      max_steps (int): maximum number of steps

    Returns: a list of frequencies (tuples) of language states for
      each threshold B.
    """

    language_states = [(0, 0, 0)] * len(Bs)

    # Run the simulation for different B threshold parameters
    for i, B in enumerate(Bs):
        thresholds = (A, B, C)
        original_grid = copy.deepcopy(grid)
        language_states[i] = run_simulation(original_grid, \
          R, thresholds, centers, max_steps)

    return language_states



@click.command(name="language")
@click.option('--grid_file', type=click.Path(exists=True),
              default="tests/writeup-grid.txt", help="filename of the grid")
@click.option('--r', type=int, default=1, help="neighborhood radius")
@click.option('--a', type=float, default=0.6, help="transition threshold A")
@click.option('--b', type=float, default=0.8, help="transition threshold B")
@click.option('--c', type=float, default=1.6, help="transition threshold C")
@click.option('--max_steps', type=int, default=1,
              help="maximum number of simulation steps")
def cmd(grid_file, r, a, b, c, max_steps):
    '''
    Run the simulation.
    '''

    grid, centers = utility.read_grid(grid_file)
    print_grid = len(grid) < 20

    print("Running the simulation...")

    if print_grid:
        print("Initial region:")
        for row in grid:
            print("   ", row)
        if len(centers) > 0:
            print("With community centers:")
            for center in centers:
                print("   ", center)

    # run the simulation
    frequencies = run_simulation(grid, r, (a, b, c), centers, max_steps)

    if print_grid:
        print("Final region:")
        for row in grid:
            print("   ", row)

    print("Final language state frequencies:", frequencies)

if __name__ == "__main__":
    cmd()
