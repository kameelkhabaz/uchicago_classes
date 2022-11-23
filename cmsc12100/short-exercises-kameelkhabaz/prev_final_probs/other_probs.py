
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


# pg 11-12
# issue is that this function returns a float
# This is BAD as function is specified to return an integer 
def update_tools(inventory, ledger):
    for l in ledger:
        if l < 0 and inventory[abs(l) - 1] == 0:
            print("Warning: tool", abs(l), "has no units", \
                "available, but ledger says it was borrowed")
        else:
            inventory[abs(l) - 1] += l / abs(l) # get sign, WILL BE PENALIZED
# corrected on paper
# REMEMBER BOTH CONDITIONS FOR IF
# FUNCTION MUST RETRUN INTEGER
def update_tools(inventory, ledger):
    for change in ledger:
        pos = abs(change) - 1
        if inventory[pos] == 0 and change < 0:
            print("Warning: tool", abs(change),"has no units", \
                "available","but ledger says it was borrowed")
        else:
            if change < 0:
                inventory[pos] -= 1
            else:
                inventory[pos] += 1

# pg 13-14
def compute_fares(trips, stations):
    fares = []
    for trip in trips:
        fare = 0
        for station in trip:
            fare += stations[station]
        fares.append(fare) #REMEMBER TO ADD THIS 

    return fares

# pg 16-18
def compute_frequencies(lst, lb, ub):
    L = len(lst) # USE LENGTH NOT THE SUM
    # freqs = [0 for _ in range(lb, ub + 1)] MUST RETURN FLOAT
    freqs = [0.0] * (ub - lb + 1)
    for n in lst:
        if n >= lb and n <= ub:
            freqs[n - lb] += 1/L
    freq_out = 1 - sum(freqs)

    return freqs, freq_out

# pg 19-22
# very bad doesn't work at all
def gen_committee(potential_members, diversity_dimension, target_per_group):
    comm_assign = {}
    num_in_grp = {}
    for p in potential_members:
        if num_in_grp.get(p[diversity_dimension], 0) < target_per_group:
            comm_assign.get(p[diversity_dimension], []).append(p["Name"])

    return comm_assign
# Better
# NEED GOOD VAR NAMES
def gen_committee(potential_members, diversity_dimension, target_per_group):
    comm_assign = {}
    num_in_grp = {}
    for p in potential_members:
        grp = p[diversity_dimension]
        if num_in_grp.get(grp, 0) < target_per_group:
            if grp in comm_assign:
                comm_assign[grp].append(p["Name"])
            else:
                comm_assign[grp] = [p["Name"]]
                
        num_in_grp[grp] = num_in_grp.get(grp, 0) + 1 #DONT FORGET
    return comm_assign
# BEST
def gen_committee(potential_members, diversity_dimension, target_per_group):
    group_names = {}
    for pot_mem in potential_members:
        grp = pot_mem[diversity_dimension]
        if grp not in group_names:
            group_names[grp] = []
        if len(group_names[grp]) < target_per_group:
            group_names[grp].append(pot_mem["Name"])
    return group_names

# page 22 - 26
def build_status(num_assignments, students, submissions):
    subs = {}
    for student in students:
        #subs[student] = [None for _ in range(num_assignments)]
        subs[student] = [None] * num_assignments
    
    for student, n, label in submissions:
        if student not in students:
            print("No such student:", student)
        elif n < 0 or n >= num_assignments:
            print("No such assignment:", n)
        else:
            subs[student][n] = label
    
    return subs

def find_missing(status_dict, assignment_num):
    missing = []
    for student, subs in status_dict.items():
        if subs[assignment_num] is None:
            missing.append(student)
    return missing

# page 27-29
# MANY SMALL MISTAKES HERE - BE VERY VERY CAREFUL
#1: find_cands must return a list as a set is not subscriptable.
    # either need to cast the set into a list or use a list but check if the
    # candidate is in the list
#2: the empty ballot is [] not None
#3: we check the first cand in the ballot (ballot[0]) not entire ballot
    #MUST INDEX
#4: # votes per cand is the VALUE of votes not the KEYS
#5: when you check if top-rankd cand is in losers you must check if the 
    # ballots list is empty
#6: you must remember to increment the round, and this incrementation must be 
    # done at the right spot to ensure the correct final round is returned.
    # if you increment at each iteration of the loop, then initiate runoff_round
    # to 0.

def find_cands(votes, n_votes):
    '''
    Get candidate with n_votes votes
    
    Returns: set
    '''
    cands = set()
    for cand, n in votes.items():
        if n == n_votes:
            cands.add(cand)
    return list(cands)

def election_round(ballots, thresh):
    votes = {}
    for ballot in ballots:
        if ballot == []:
            continue

        votes[ballot[0]] = votes.get(ballot[0], 0) + 1
    
    most_votes = max(votes.values())
    if most_votes < thresh:
        # no winner
        least_votes = min(votes.values())
        losers = find_cands(votes, least_votes)
        # LOSERS S HOULD BE A SET
        for ballot in ballots:
            if ballot != [] and ballot[0] in losers:
                ballot.pop(0)
        return None, None

    # there is a winner
    winner = find_cands(votes, most_votes)
    winner = winner[0] # always 1 winner
    return winner, most_votes

def election(ballots):
    runoff_round = 0
    candidate = None
    thresh = len(ballots) // 2 + 1 # To get a majority
    while candidate is None:
        runoff_round += 1
        candidate, nvotes = election_round(ballots, thresh)
    
    return candidate, runoff_round, nvotes

# page 30-32
# 7:00
# SMALL MISTAKE FIX IT
# 1: FORGOT TO PUT .items() in loop over dict DONT FORGET
# Use TUPLE UNPACKING and CLEAR VARIABLE NAMES
def compute_tiers(priority_counts, tiers):
    tiered_counts = {}
    for course, count in priority_counts.items():
        tiered_count = []
        for lb, ub in tiers:
            tiered_count.append(sum(count[lb:ub]))
        tiered_counts[course] = tiered_count
    return tiered_counts

def compute_tiers(priority_counts, tiers):
    tiered_counts = {}
    for course, count_lst in priority_counts.items():
        tiered_counts[course] = [0] * len(tiers)
        for i, (lb, ub) in enumerate(tiers):
            tiered_counts[course][i] = sum(count_lst[lb:ub])
    return tiered_counts

# page 33 - 37
# 15:00
# Passes tests on exam
# Be sure to type out <= for less than/equal to sign and not have line under the <
class Professor:
    ''' Class to represent a professor '''
    def __init__(self, name, tenured, fields):
        '''
        Constructor
        Parameters:
        - name (string): The professor's full name
        - tenured (boolean): True if the professor has tenure, False otherwise
        - fields (list of strings): The professor's fields of study
        '''
        self.name = name
        self.tenured = tenured
        self.fields = fields

class Committee:
    ''' Class to represent a committee '''
    def __init__(self, name, professors):
        '''
    Constructor
        Parameters:
        - name (string): The committee's name
        - professors (list of Professor objects): The professors that
          are on the committee
        '''
        self.name = name
        self.__professors = professors
    def size(self):
        return len(self.__professors)

    def num_tenured(self):
        count = 0
        for prof in self.__professors:
            if prof.tenured:
                count += 1
        return count 

    def unique_fields(self):
        unique = set()
        for prof in self.__professors:
            #unique.update(set(prof.fields))
            # No need for inner set here
            unique.update(prof.fields)
        return list(unique) # always must return correct type 

def get_bad_committees(committees):
    bad_committees = []
    for com in committees:
        fields = com.unique_fields()
        if len(fields) <= 2:
            ntenured = com.num_tenured()
            if ntenured < (com.size() // 2) + 1:
                bad_committees.append(com)
        # if > 2 fields, no need to check other condition

# Better to use 1 if statement
def get_bad_committees(committees):
    bad_committees = []
    for com in committees:
        fields = com.unique_fields()
        ntenured = com.num_tenured()     
        if len(fields) <= 2 and ntenured < (com.size() // 2) + 1:
            bad_committees.append(com) 
    return bad_committees

# pages 38 - 42
# 11:07
# Good with given tests
class Assignment(object):
    ''' Class to represent assignments '''
    def __init__(self, aid, name, points):
        '''
        Constructor
        Parameters:
        - aid (string): An assignment identifier (e.g., "pa1")
        - name (string): A descriptive name (e.g. "Lists/loops Homework")
        - points (int): The number of points this assignment is worth
        The class also has a private attribute: __students_submitted,
        a list containing integers, where each integer is a
        student identifier. This list models the students who have
        made a submission for the assignment
        '''
        self.aid = aid
        self.name = name
        self.points = points
        self.__students_submitted = []
    def add_student_submission(self, student_id):
        '''
        Adds a student to the set of students who have made a submission
        Parameter:
        - student_id (int): A student identifier (e.g., 10123456)
        Returns: Nothing, but will add the submission to the __students_submitted
                 list (unless that student has already made a submission, in
                 which case we raise an exception)
        '''
        if student_id in self.__students_submitted:
            raise Exception(student_id, "has already submitted!")
        else:
            self.__students_submitted.append(student_id)
    def num_submissions(self):
        ''' Returns the number of submissions made for this assignment '''
        return len(self.__students_submitted)

class Course(object):
    def __init__(self, code, name, num_students):
        '''
    Constructor
        Parameters:
        - code (string): A course code (e.g., "CMSC 12100")
        - name (string): A descriptive name
        - num_students (int): The number of students in the
        course
        The class also has a private __assignments attribute
        that contains the assignments in this course. It is
        a dictionary that maps assignment identifiers (e.g., "pa1")
        to Assignment objects.
        '''
        self.code = code
        self.name = name
        self.num_students = num_students
        self.__assignments = {}

    def add_assignment(self, assignment):
        self.__assignments[assignment.aid] = assignment
    
    def get_assignment_by_id(self, aid):
        return self.__assignments.get(aid, None)
    
    def total_points(self):
        npts = 0
        for a in self.__assignments.values():
            npts += a.points
        return npts
    
    def percent_submitted(self, aid):
        a = self.get_assignment_by_id(aid)
        if a is None:
            return None
        return 100 * a.num_submissions() / self.num_students

# For recursion problems you either find permutations or combinations
# page 43 - 45
# 33:00
# took long time but nice, remember <= 
# Making copy of dict each time is inefficient. We can just modify
# the existing dict and modify it back
def make_change(cents, drawer):
    if cents == 0:
        return [[]]
        # base case
    
    all = []

    for coin_value, num_coins in drawer.items():
        if num_coins > 0 and coin_value <= cents:
            s_drawer = drawer.copy() # bad, DONT COPY THE DICT
            s_drawer[coin_value] -= 1
            s_combs = make_change(cents - coin_value, s_drawer)
            for s_comb in s_combs:
                s_comb.insert(0, coin_value)
                all.append(s_comb)

    return all

# Corrected 
# Common pattern of recursion with permutations
# JUST MAKE A CHANGE AND CHANGE IT BACK - DONT FORGET THIS STRATEGY
def make_change(cents, drawer):
    if cents == 0:
        return [[]]
        # base case
    all = []
    for coin_value, num_coins in drawer.items():
        if num_coins > 0 and coin_value <= cents:
            drawer[coin_value] -= 1
            s_combs = make_change(cents - coin_value, drawer)
            drawer[coin_value] += 1
            for s_comb in s_combs:
                s_comb.insert(0, coin_value)
                all.append(s_comb)
    return all
# page 46 - 50
# 20:00, similar to a tree-type recursion problem
# We do NOT have to copy the board because all further calls down the tree
# all have the same edit to the board as the original call (they have the x in 
# the same position)
# When you want to explore what happens when certain values are placed in a data
# structure it is a COMMON STRATEGY to set a value before the recursive call and 
# to UNDO it after the recursive function call.
def count_outcomes(board, empties, turn):
    # base case 1 (someone won)
    w = winner(board)
    if w == 1:
        return 1, 0, 0
    elif w == -1:
        return 0, 1, 0
    # base case 2 (no more moves left)
    if len(empties) == 0:
        return 0, 0, 1
    
    tX = 0 # num X wins
    tO = 0 # num O wins
    tT = 0 # num Tie

    for i, (x, y) in enumerate(empties):
        # Notice how we do NOT have to copy the board
        board[x][y] = turn
        s_empties = empties[:i] + empties[i + 1:]
        wX, wO, wT = count_outcomes(board, s_empties, -turn)
        tX += wX
        tO += wO
        tT += wT
        board[x][y] = 0 # must reset board position to not have to copy board
    
    return tX, tO, tT

# Wasn't asked but want to implement to test other func, testing didn't work
import numpy as np
def winner(board):
    s = len(board)
    b = np.array(board)
    if np.fliplr(b).trace() == s or b.trace() == s:
        return 1
    if np.fliplr(b).trace() == -s or b.trace() == -s:
        return -1
    # check rows and sums
    col_sums = b.sum(axis = 0)
    row_sums = b.sum(axis = 1)

    if s in col_sums or s in row_sums:
        return 1
    if -s in col_sums or -s in row_sums:
        return -1
    return None

# page 51- 52
# 33:00
# Remember when appending to value of dictionary that is a list, must first check
# if the value is empty. Then initialize and can append.

# I'm still having trouble figuring out how to avoid repeat values. Checking
# sched not in schedules removes the word-for-word repeat, but many redundant 
# schedules are left in the output.
def find_schedules(instructors, constraints, max_per_day):
    # base case
    if len(instructors) == 0:
        return [{}]
    schedules = []
    for ins in instructors:
        days = constraints[ins]
        for day in days:
            s_instructors = instructors[:]
            s_instructors.remove(ins)
            s_schedules = find_schedules(s_instructors, constraints, max_per_day)
            for sched in s_schedules:
                if day not in sched:
                    sched[day] = []

                if len(sched[day]) < max_per_day:
                    sched[day].append(ins)
                    if sched not in schedules:
                        schedules.append(sched)

    return schedules

# Corrected Solution
def find_schedules(instructors, constraints, max_per_day):
    # base case
    if len(instructors) == 0:
        return [{}]
    schedules = []
    ins = instructors[0]
    days = constraints[ins]
    for day in days:
        s_instructors = instructors[:]
        s_instructors.remove(ins)
        s_schedules = find_schedules(s_instructors, constraints, max_per_day)
        for sched in s_schedules:
            if day not in sched:
                sched[day] = []

            if len(sched[day]) < max_per_day:
                sched[day].append(ins)
                schedules.append(sched)

    return schedules

# Try 2 from paper

# Remember when appending to value of dictionary that is a list, must first check
# if the value is empty. Then initialize and can append.

# If you have the schedules for b and c, just CHECK where to add a
# Combine schedule of a with each possible schedule of b and c 
# The job here is to find out how to COMBINE the result of the recursive
# function call with the new instructor
# So get closer to base case by REDUCING SIZE OF INSTRUCTORS LIST
def find_schedules(instructors, constraints, max_per_day):
    if len(instructors) == 0:
        return [{}] # must always return the same type
    schedules = []
    for day in constraints[instructors[0]]:
        sub_scheds = find_schedules(instructors[1:], constraints, \
            max_per_day) # it is ok to make same call each time because we want 
            # a copy from each function call
        for sub_sched in sub_scheds:
            if day not in sub_sched:
                sub_sched[day] = [] # MUST DO THIS DONT FORGET
            if len(sub_sched[day]) < max_per_day:
                sub_sched[day].append(instructors[0])
                schedules.append(sub_sched)
    return schedules

# Different base case
# During Office Hours: Making base case have 1 instructor is easier
def find_schedules(instructors, constraints, max_per_day):
    schedules = []
    if len(instructors) == 1:
        inst = instructors[0]
        for day in constraints[inst]:
            schedules.append({day:[inst]})
        return schedules
    for day in constraints[instructors[0]]:
        sub_scheds = find_schedules(instructors[1:], constraints, \
            max_per_day) # it is ok to make same call each time because we want 
            # a copy from each function call
        for sub_sched in sub_scheds:
            if day not in sub_sched:
                sub_sched[day] = [] # MUST DO THIS DONT FORGET
            if len(sub_sched[day]) < max_per_day:
                sub_sched[day].append(instructors[0])
                schedules.append(sub_sched)
    return schedules

# page 53- 55
# 30:00
# Minor mistakes in spelling and list nestedness
# You need to return a list of lists for base case 
def flights(itinerary, schedules, min_layover, max_layover):
    '''
    Find all possible sequences of flights matching itinerary
    Inputs:
      itinerary: list of strings
      schedules: dict of dicts of lists
      min_layover: int
      max_layover: int
    Returns: list of lists of possible flight sequences
    '''
    return flights_r(itinerary, schedules, min_layover, max_layover, 0)
# Permutation problem
# Put comments in recursive helper functions
def flights_r(itinerary, schedules, min_layover, max_layover, time):
    """
    Recursive helper function to find list of possible flight sequences
    Inputs:
      Same as above plus
      time (int): current time, when last flight as arrived
  
    Returns: list of lists of possible flight sequences
    """
    if len(itinerary) <= 1:
        # Need return [["done"]] NOT ["done"]
        # BASE CASE MUST ALWAYS RETURN SAME TYPE AS GENERAL CASE
        return [["done"]]
    lst = []
    depart = itinerary[0]
    arrive = itinerary[1]
    poss_flights = schedules[depart][arrive]
    for poss_flight in poss_flights:
        time_waiting = poss_flight[0] - time
        if time_waiting >= min_layover and time_waiting <= max_layover:
            next_flights = flights_r(itinerary[1:], schedules, min_layover, \
                max_layover, poss_flight[1])
            for flights_seq in next_flights:
                # Better here to insert the new possible flight to not copy list
                # But remember insert doesn't return the list so you need 2 lines
                # If you add them remember that you need possible flights must be
                # a list of lists, so need do [poss_flight] + flights_seq and  
                # NOT poss_flight + flights_seq
                flights_seq.insert(0,poss_flight)
                lst.append(flights_seq)
    return lst

schedules = {"chicago":{"detroit":    [ [1.5, 3.0], [2.75, 4.25], [5.0, 6.0] ],
                        "omaha":      [ [1.0, 2.0], [3.0, 4.0] ],
                        "pittsburgh": [ [2.5, 3.5] ]
                       },
             "detroit":{"omaha":      [ [2.0, 3.0], [3.0, 4.0], [4.0, 5.0] ],
                        "chicago":    [ [1.0, 2.0], [3.0, 4.0] ],
                        "pittsburgh": [ [2.5, 3.5], [5.0, 6.0] ]
                       },
             "omaha":{"pittsburgh": [ [1.5, 4.5], [6.0, 9.0], [6.5, 9.5] ],
                      "chicago":    [ [1.0, 2.0], [3.0, 4.0] ],
                      "detroit":    [ [5.5, 7.5], [8.0, 9.0] ]},
             "pittsburgh":{"chicago": [ [10.5, 11.5], [11.25, 12.75] ],
                           "detroit": [ [ 1.5,  2.5], [  4.0,   5.0] ],
                            "omaha":  [ [ 2.0,  3.0], [  3.0,   4.0] ]}}
# page 56-57 
# 2:37
# REMEMBER TO CHECK THE ROOT VALUE OF THE TREE
def max_salary(employees):
    if employees.num_children() == 0:
        return employees.value
    max_sal = employees.value # initialize
    for child in employees.children:
        csal = max_salary(child)
        if csal > max_sal:
            max_sal = csal
    return max_sal
