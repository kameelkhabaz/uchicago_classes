"""
Short Exercises #3
Kameel Khabaz
"""


def find_candidates_from_city(candidates, office_loc):
    """
    Given a list of candidates, construct a list of the candidate IDs
    for candidates with a campaign headquartered in the specified location.

    Inputs:
        candidates: list of candidates
        office_loc (string, string): a tuple of the form 
            (city name, state abbreviation)

    Returns: list of candidate IDs (strings)
    """

    IDs = []

    for candidate in candidates:
        if candidate["City"] == office_loc[0] and candidate["State"] == office_loc[1]:
            IDs.append(candidate["Candidate_ID"])
     
    return IDs


def construct_dict_from_lists(keys, values):
    """
    Given a list of (key, index) pair and a list of values, construct a
    dictionary that maps each key to the value in the list of values at the
    specified index.

    Inputs:
        keys: a list of (key, index) pairs, where each key is a (unique)
            immutable value (string, int, etc.), and each index is an integer
        values: a list of values

    Returns: dictionary
    """

    mydict = {}
    for key,idx in keys:
        mydict[key] = values[idx]

    return mydict



def construct_homestate_dict(candidates):
    """
    Construct a dictionary that maps a candidate ID to the candidate's
    home state.

    Inputs:
        candidates: list of candidates    

    Returns: dictionary that maps candidate id (string) to a state
        abbreviation (string)
    """

    homestate_dict = {}
    
    for candidate in candidates:
        homestate_dict[candidate["Candidate_ID"]] = candidate["State"]
    return homestate_dict


def find_successful_fund_raisers(cand_to_count, threshold):
    """
    Given a dictionary that maps candidate IDs to the number of donations
    received by the campaigns, compute a list of the candidates who have
    received at least the threshold number of contributions.

    Inputs:
        cand_to_count: dictionary that maps Candidate IDs to integers
        threshold (int): the threshold for labeling a candidate as successful

    Returns: list of Candidate IDs
    """

    successful_candidates = []
    for candidate_ID, donations in cand_to_count.items():
        if donations > threshold:
            successful_candidates.append(candidate_ID)
     
    return(successful_candidates)
    
    


def construct_cands_by_state(candidates):
    """
    Construct a mapping from states to the candidates from that state.

    Inputs:
      candidates: list of candidate dictionaries

    Returns: dictionary that maps a state abbreviation (string) to a
     list of dictionaries for candidates from that state.
    """

    candidate_mapping = {}

    for candidate in candidates:
        state = candidate["State"]
        if state not in candidate_mapping:
            candidate_mapping[state] = [candidate]
        else:
            candidate_mapping[state].append(candidate)

    return candidate_mapping