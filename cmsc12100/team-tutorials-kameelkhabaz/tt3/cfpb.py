'''
Team Tutorial #4: Dictionaries
CMSC 121 / CAPP 30121
Autumn 2021
'''

import json

CFPB_16 = json.load(open("cfpb16_1000.json"))

# Task 1
def find_companies(complaints):
    '''
    Compute a list of companies complained about

    Inputs:
        complaints (list) A list of complaints, where each complaint is a
            dictionary

    Returns: (list or set) of companies
    '''
    companies = set()

    for complaint in complaints:
        companies.add(complaint["Company"])
    return companies


# Task 2
def count_complaints_about(complaints, company_name):
    '''
    Count complaints about a specified company

    Inputs:
        complaints (list) A list of complaints, where each complaint is a
            dictionary
        company_name (str): The company name to count complaints for

    Returns: count of complaints (int)
    '''
    num_complaints = 0
    for complaint in complaints:
        if complaint["Company"] == company_name:
            num_complaints += 1
    return num_complaints


# Task 3
def count_by_state(complaints):
    '''
    Compute counts by state of all complaints

    Inputs:
         complaints (list) A list of complaints, where each complaint is a
            dictionary

    Returns: (dict) that relates states to complaint number
    '''
    by_state = {}

    for complaint in complaints:
        state = complaint["State"]
        by_state[state] = by_state.get(state,0) + 1
    return by_state


# Task 4
def state_with_most_complaints(cnt_by_state):
    '''
    Find the state with the most complaints. Can break ties arbitrarily.

    Inputs:
        cnt_by_state (dict) A dictionary relating each state to the
            count of complaints in that state

    Returns: (str) the state with the most complaints
    '''
    max_val = max(cnt_by_state.values())

    for state, cnt in cnt_by_state.items():
        if cnt == max_val:
            return state


# Task 5
def count_by_company_by_state(complaints):
    '''
    Computes a dict of {company: {state: count, state: count}} for all states
        and companies

    Inputs:
        complaints (list) A list of complaints, where each complaint is a
            dictionary

    Returns: (dict) with count per company per state
    '''
    by_comp_state = {}

    for complaint in complaints:
        company = complaint["Company"]
        state = complaint["State"]
        if company not in by_comp_state:
            by_comp_state[company] = {state:0}
   

        by_comp_state[company][state] = by_comp_state[company].get(state,0) + 1
    return by_comp_state

# Task 6
def complaints_by_company(complaints):
    '''
    Create a dictionary that maps the name of a company to a list of the
    complaint dictionaries that concern that company.

    Inputs:
        complaints (list) A list of complaints, where each complaint is a
            dictionary

    Returns: (dict) mapping the name of the company to a list of complaints
    '''
    by_company = {}
    for complaint in complaints:
        company = complaint["Company"]
        by_company[company] = by_company.get(company,[]) + [complaint]
    return by_company
  
        


# Task 7
def count_by_company_by_state_2(complaints):
    '''
    Computes a dict of {company: {state: count, state: count}} for all states
        and companies

    Inputs:
        complaints (list) A list of complaints, where each complaint is a
            dictionary

    Returns: (dict) with count per company per state

    This implementation involves composing complaints_by_company with 
        count_by_state
    '''
    by_comp_state = {}
    by_company = complaints_by_company(complaints)
    for company_name, company_complaints in by_company.items():
        by_comp_state[company_name] = count_by_state(company_complaints)
    return by_comp_state
