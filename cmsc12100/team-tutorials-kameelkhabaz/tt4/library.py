'''
Team Tutorial #4: Library class
'''


class Library:

    ### REPLACE "pass" WITH YOUR Library constructor (__init__) HERE ###
    def __init__(self, island, name, reference, book, microform):
        self.island = island
        self.name = name
        self.reference = reference
        self.book = book
        self.microform = microform

    ### YOUR total_circulation method HERE ###
    def total_circulation(self):
        return (self.reference + self.book + self.microform)
    ### YOUR has_microform method HERE ###
    def has_microform_catalogue(self):
        return self.microform > 0


### FUNCTIONS ###

def branch_with_biggest_circulation(libraries):
    '''
    Find the library with the largest total number of
    items in circulation

    Input:
        libraries: (list of Library) libraries

    Returns: name of library (string)
    '''
    biggest_branch = ""
    biggest_branch_circ = 0
    for library in libraries:
        if library.total_circulation() > biggest_branch_circ:
            biggest_branch = library.name
            biggest_branch_circ = library.total_circulation()

    return biggest_branch

def percentage_with_microform(libraries):
    '''
    Find the percentage of libraries that have
    microform catalogues

    Input:
        libraries: (list of Library) libraries

    Returns: percentage (float)
    '''
    ### YOUR CODE HERE ###
    num_microform = 0
    num = 0

    for library in libraries:
        num += 1
        if library.has_microform_catalogue():
            num_microform += 1
    
    return num_microform / num * 100
