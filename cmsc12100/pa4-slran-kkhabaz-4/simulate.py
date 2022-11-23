#test
'''
Polling places

Kameel Khabaz
Steph Ran

Main file for polling place simulation
'''

from operator import is_
import random
import queue
import click
import util



class Voter:
    '''
    Class for representing a voter.
    
    Attributes:
      arrival_time: the time the voter arrives at the polls
      voting_duration: the amount of time the voter takes to vote
      is_impatient: whether the voter is impatient (will they leave if 
                    they have to wait too long)
      start_time: time the voter is assigned to a voting booth if they vote
      departure_time: time the voter leaves the voting booth if they vote
      has_voted: whether or not the voter voted
  
    Methods:
        set_start_time(self, start_time):
            Set the start time when the voter begins voting
        set_voted(self, departure_time, has_voted):
            Set the departure time and "voted" status when the voter has
            finished voting
        get_impatient(self):
            Determines if the voter is impatient
    '''

    def __init__(self, arrival_time, voting_duration, is_impatient):
        '''
        Initialize the voter

        Args:
          arrival_time (float): the time the voter arrives at the polls
          voting_duration (float): the amount of time the voter takes to vote
          is_impatient (boolean): whether the voter is impatient (will they 
                                  leave if they have to wait too long)
        '''

        self.arrival_time = arrival_time
        self.voting_duration = voting_duration
        self.is_impatient = is_impatient

        # These attributes have not been determined yet
        self.start_time = None
        self.departure_time = None
        self.has_voted = False

    def set_start_time(self, start_time):
        '''
        When the voter starts voting, set the start time 

        Args:
          start_time (float): time the voter is assigned to a
                              voting booth if they vote
        '''

        self.start_time = start_time

    def set_voted(self, departure_time, has_voted):
        '''
        When the voter has voted, set the 2 appropriate attributes 

        Args:
          departure_time (float): time the voter leaves the
                                  voting booth if they vote
          has_voted (boolean): whether or not the voter voted
        '''

        self.departure_time = departure_time
        self.has_voted = has_voted
    
    def get_impatient(self):
        '''
        Gets if voter is impatient
        
        Returns (boolean): if voter is impatient
        '''

        return self.is_impatient

    def __repr__(self):
        '''
        Print a string representation of the voter
        '''

        return "Voter( {}, {}, {}, {}, {}, {})".format(self.arrival_time, 
                                                       self.voting_duration, 
                                                       self.is_impatient, 
                                                       self.start_time, 
                                                       self.departure_time, 
                                                       self.has_voted)



class VotingBooths:
    '''Class for representing a bank of voting booths.

    Attributes: None

    Methods:
        is_booth_available: bool
            is there at least one unoccupied booth
        is_some_booth_occupied: bool
            is there at least one occupied booth
        enter_booth(v):
            add a voter to a booth. requires a booth to be available.
        time_next_free(): float
            when will a booth be free next (only called when all the
            booths are occupied)
        exit_booth():
             remove the next voter to depart from the booths and
             return the voter and their departure_time.
    '''

    def __init__(self, num_booths):
        '''
        Initialize the voting booths.

        Args:
            num_booths: (int) the number of voting booths in the bank
        '''

        self._num_booths = num_booths
        self._q = queue.PriorityQueue()

    def is_booth_available(self):
        '''Is at least one booth open'''

        return self._q.qsize() < self._num_booths

    def is_some_booth_occupied(self):
        '''Is at least one booth occupied'''

        return self._q.qsize() > 0

    def enter_booth(self, v):
        '''
        Add voter v to an open booth

        Args:
            v: (Voter) the voter to add to the booth.

        Requirements: there must be an open booth.
        '''

        assert self.is_booth_available(), "All booths in use"
        assert v.start_time, "Voter's start time must be set."

        dt = v.start_time + v.voting_duration
        self._q.put((dt, v))

    def time_next_free(self):
        '''
        When will the next voter leave?

        Returns: next departure time

        Requirements: there must be at least one occupied booth.
        '''

        assert self.is_some_booth_occupied(), "No booths in use"

        # PriorityQueue does not have a peek method.
        # So, do a get followed by a put.
        (dt, v) = self._q.get()
        self._q.put((dt, v))
        return dt


    def exit_booth(self):
        '''
        Remove voter with lowest departure time.

        Returns: the voter and the voter's departure time

        Requirements: there must be at least one occupied booth.
        '''

        assert self.is_some_booth_occupied(), "No booths in use"

        (dt, v) = self._q.get()
        return v, dt


class Precinct(object):
    '''
    Class for representing precincts.

    Attributes: 
      name: Name of the precinct
      hours_open: Hours the precinct will remain open
      num_voters: Number of voters in the precinct
      arrival_rate: Rate at which voters arrive
      voting_duration_rate: Lambda for voting duration
      impatience_prob: the probability that a voter
                       will be impatient.
      voter_list: list of voters

    Methods:
        simulate(seed, voting_booths, impatience_threshold):
            Simulate election day for the precinct using the
            specified seed, voting_booths, and impatience threshold.
    '''

    def __init__(self, name, hours_open, num_voters, arrival_rate,
                 voting_duration_rate, impatience_prob):
        '''
        Constructor for the Precinct class

        Args:
          name: (str) Name of the precinct
          hours_open: (int) Hours the precinct will remain open
          num_voters: (int) Number of voters in the precinct
          arrival_rate: (float) Rate at which voters arrive
          voting_duration_rate: (float) Lambda for voting duration
          impatience_prob: (float) the probability that a voter
              will be impatient.
        '''

        self.name = name
        self.hours_open = hours_open
        self.num_voters = num_voters
        self.arrival_rate = arrival_rate
        self.voting_duration_rate = voting_duration_rate
        self.impatience_prob = impatience_prob

    def __generate_voter_list__(self, seed):
        '''
        Generate a list of voters 
        
        Args:
          seed: random seed
        '''

        random.seed(seed)

        voter_list = []
        
        # Keep track of time in minutes
        time = 0
        max_time = self.hours_open * 60

        # First step of simulation 
        gap, voting_duration, is_impatient = util.gen_voter_parameters(
            self.arrival_rate, self.voting_duration_rate, self.impatience_prob)
        time += gap

        for _ in range(self.num_voters):

            if time > max_time:
                break

            # Create a new voter
            new_voter = Voter(time, voting_duration, is_impatient)
            voter_list.append(new_voter)

            gap, voting_duration, is_impatient = util.gen_voter_parameters(
                self.arrival_rate,
                self.voting_duration_rate, 
                self.impatience_prob)
            # Move simulation forward
            time += gap
        
        return voter_list

    def simulate(self, seed, voting_booths, impatience_threshold):
        '''
        Simulate election day for the precinct using the specified seed,
        voting_booths, and impatience threshold.

        Args:
          seed: (int) the seed for the random number generator
          voting_booths: (VotingBooths) the voting booths assigned to the
              precinct for the day
          impatience_threshold: (int) the number of minutes an impatient voter
              is willing to wait (inclusive)

        Returns: list of Voters
        '''

        voters = self.__generate_voter_list__(seed)

        for voter in voters:
            if voting_booths.is_booth_available():
                # Booths are available
                voter.set_start_time(voter.arrival_time)
                voting_booths.enter_booth(voter)

            else:
                # No booths are available
                earliest_departure = voting_booths.time_next_free()
                projected_start = max(earliest_departure, voter.arrival_time)
                t_wait = projected_start - voter.arrival_time
                
                if (not voter.is_impatient) or t_wait <= impatience_threshold:
                    # If voter is patient or wait time is not too long 
                    removed_voter, departure_time = voting_booths.exit_booth()
                    removed_voter.set_voted(departure_time, True)
                    voter.set_start_time(projected_start)
                    voting_booths.enter_booth(voter)
            
        while voting_booths.is_some_booth_occupied():
            # Empty any remaining booths
            removed_voter, departure_time = voting_booths.exit_booth()
            removed_voter.set_voted(departure_time, True)
        
        return voters



def find_impatience_threshold(seed, precinct, num_booths, num_trials):
    '''
    For a given precinct, find the impatience threshold at which all
    voters are likely to vote.

    Args:
      seed (int): the initial seed for the random number generator
      precinct: (Precinct) the precinct to simulate
      num_booths: (int) number of voting booths to use in the simulations
      num_trials: (int) the number of trials to run

    Returns: (int) the median threshold from the trials
    '''

    assert num_trials > 0

    thresholds = []
    voting_booths = VotingBooths(num_booths)

    for _ in range(num_trials):
        threshold = impatience_thresholds_trial(seed, voting_booths, precinct)
        thresholds.append(threshold)
        seed += 1
        
    median_threshold = sorted(thresholds)[len(thresholds) // 2]
    return median_threshold

def impatience_thresholds_trial(seed, voting_booths, precinct):
    '''
    Simulate a precinct, tweaking the threshold for how long impatient voters 
    are willing to wait and finding the impatience threshold large enough to
    guarantee that all voters stay to vote. 
    
    Args:
      seed (int): the seed for the random number generator
      precinct: (Precinct) the precinct to simulate
    
    Returns: (int) impatience threshold large enough so that everyone votes
    '''
    impatience_threshold = -9 # Initialize so threshold = 1 at first run
    num_not_voted = 1 # Initialize to a nonzero value

    while num_not_voted > 0:
        # Increase impatience threshold until everyone votes
        impatience_threshold += 10
        voters = precinct.simulate(seed, voting_booths, impatience_threshold)
        num_not_voted = get_num_not_voted(voters)

    return impatience_threshold

def find_voting_booths_needed(seed, precinct, imp_threshold, num_trials):
    '''
    For a given precinct, seed, and impatience threshold, predict the number of
    booths needed to make it likely that all the voters will vote.

    Args:
      seed (int): the initial seed for the random number generator
      precinct: (Precinct) the precinct to simulate
      impatience_threshold: (int) the impatience threshold
      num_trials: (int) the number of trials to run

    Returns: (int) the median number of booths needed from the trials.
    '''

    assert num_trials > 0

    thresholds = []
    
    for _ in range(num_trials):
        threshold = num_booths_trial(seed, precinct, imp_threshold)
        thresholds.append(threshold)
        seed += 1
        
    median_threshold = sorted(thresholds)[len(thresholds) // 2]
    return median_threshold

def num_booths_trial(seed, precinct, imp_threshold):
    '''
    Simulate a precinct, tweaking the number of booths and finding the number 
    of booths necessary for all voters to stay to vote
    
    Args:
      seed (int): the seed for the random number generator
      precinct: (Precinct) the precinct to simulate
      imp_threshold (int): impatience threshold
    
    Returns: (int) impatience threshold large enough so that everyone votes
    '''
    num_booths = 0
    num_not_voted = 1 # Initialize to a nonzero value

    while num_not_voted > 0:
        # Increase number of booths until everyone votes
        num_booths += 1
        voting_booths = VotingBooths(num_booths)
        voters = precinct.simulate(seed, voting_booths, imp_threshold)
        num_not_voted = get_num_not_voted(voters)

    return num_booths

def get_num_not_voted(voters):
    '''
    Get the number of voters who haven't voted

    Args:
      voters (list of Voters): list of voters
     
    Returns: (int) number of voters who have not voted
    '''

    return sum([not v.has_voted for v in voters])  

@click.command(name="simulate")
@click.argument('precinct_file', type=click.Path(exists=True))
@click.option('--num-booths', type=int, default=1,
              help="number of voting booths to use")
@click.option('--impatience-threshold', type=float,
              default=1000, help="the impatience threshold")
@click.option('--print-voters', is_flag=True)
@click.option('--find-threshold', is_flag=True)
@click.option('--find-num-booths', is_flag=True)
@click.option('--num-trials', type=int, default=100,
              help="number trials to run")
def cmd(precinct_file, num_booths, impatience_threshold,
        print_voters, find_threshold, find_num_booths, num_trials):
    '''
    Run the program...
    '''
    #pylint: disable=too-many-locals
    p, seed = util.load_precinct(precinct_file)

    precinct = Precinct(p["name"],
                        p["hours_open"],
                        p["num_voters"],
                        p["arrival_rate"],
                        p["voting_duration_rate"],
                        p["impatience_prob"])

    if find_threshold:
        pt = find_impatience_threshold(seed, precinct, num_booths, num_trials)
        s = ("Given {} booths, an impatience threshold of {}"
             " would be appropriate for Precinct {}")
        print(s.format(num_booths, pt, p["name"]))
    elif find_num_booths:
        vbn = find_voting_booths_needed(seed, precinct,
                                        impatience_threshold, num_trials)
        s = ("Given an impatience threshold of {}, provisioning {}"
             " booth(s) would be appropriate for Precinct {}")
        print(s.format(impatience_threshold, vbn, p["name"]))
    elif print_voters:
        vb = VotingBooths(num_booths)
        voters = precinct.simulate(seed, vb, impatience_threshold)
        util.print_voters(voters)
    else:
        vb = VotingBooths(num_booths)
        voters = precinct.simulate(seed, vb, impatience_threshold)
        print("Precinct", p["name"])
        print("- {} voters voted".format(len(voters)))
        if len(voters) > 0:
            # last person might be impatient.  look
            # backwards for the first actual voter.
            last_voter_departure_time = None
            for v in voters[::-1]:
                if v.departure_time:
                    last_voter_departure_time = v.departure_time
                    break
            s = "- Polls closed at {} and last voter departed at {:.2f}."
            print(s.format(p["hours_open"]*60, last_voter_departure_time))
            nv = len([v for v in voters if not v.has_voted])
            print("- {} voters left without voting".format(nv))
            if not voters[-1].departure_time:
                print("  including the last person to arrive at the polls")


if __name__ == "__main__":
    cmd() # pylint: disable=no-value-for-parameter
