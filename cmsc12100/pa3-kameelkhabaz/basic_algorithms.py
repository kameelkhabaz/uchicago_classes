"""
CS 121: Analyzing Election Tweets

Kameel Khabaz

Basic algorithms module

Algorithms for efficiently counting and sorting distinct 'entities',
or unique values, are widely used in data analysis.
"""

import math
from util import sort_count_pairs

# Task 1.1
def count_tokens(tokens):
    '''
    Counts each distinct token (entity) in a list of tokens.

    Inputs:
        tokens: list of tokens (must be immutable)

    Returns: dictionary that maps tokens to counts
    '''

    counts = {}

    for token in tokens:
        counts[token] = counts.get(token, 0) + 1

    return counts


# Task 1.2
def find_top_k(tokens, k):
    '''
    Find the k most frequently occuring tokens.

    Inputs:
        tokens: list of tokens (must be immutable)
        k: a non-negative integer

    Returns: list of the top k tokens ordered by count.
    '''

    # Error checking (DO NOT MODIFY)
    if k < 0:
        raise ValueError("In find_top_k, k must be a non-negative integer")

    counts = count_tokens(tokens)

    # Convert data into list of (token, count) tuples
    counts_lst = [(token, count) for token, count in counts.items()]
    
    sorted_counts = sort_count_pairs(counts_lst)

    # Extract k most frequent counts 
    top_sorted_counts = sorted_counts[0:k]

    # Extract tokens of the most frequent counts
    top_tokens = []
    for token, _ in top_sorted_counts:
        top_tokens.append(token)

    return top_tokens


# Task 1.3
def find_min_count(tokens, min_count):
    '''
    Find the tokens that occur *at least* min_count times.

    Inputs:
        tokens: a list of tokens  (must be immutable)
        min_count: a non-negative integer

    Returns: set of tokens
    '''

    #Error checking (DO NOT MODIFY)
    if min_count < 0:
        raise ValueError("min_count must be a non-negative integer")

    counts = count_tokens(tokens)

    freq_tokens = set()

    for token, count in counts.items():
        if count >= min_count:
            freq_tokens.add(token)

    
    return freq_tokens


# Task 1.4
def count_doc_tokens(docs):
    '''
    Count the number of times each distinct term occurs in each document.
    At the same time, generate a set of unique terms across all documents.

    Inputs:
      docs: list of list of tuples

    Returns (set, list of dictionaries): a tuple of a set of terms
    and a list of dictionaries mapping the terms in each document to 
    their counts
    '''

    terms = set()
    term_counts = []
    for doc in docs:
        # Find term counts for this document
        doc_counts = count_tokens(doc)  
        # Add new terms   
        terms = terms.union(set(doc_counts.keys()))
        term_counts.append(doc_counts)
     
    return terms, term_counts

def find_inv_doc_freqs(docs, terms, term_counts):
    '''
    Calculate the inverse document frequencies (idfs) of the terms.
    Inputs:
      docs: list of list of tuples
      terms: set of terms 
      term_counts: list of dictionaries mapping the terms in each 
      document to their counts
    
    Returns: dictionary mapping terms to inverse document frequencies
    '''

    num_docs = len(docs)
    idfs = {} # inverse document frequencies
    for term in terms:
        # Find the number of documents that contain the term
        term_ct = 0  
        for idx, _ in enumerate(docs):
            doc_counts = term_counts[idx]
            if doc_counts.get(term, 0) > 0:
                term_ct += 1

        idfs[term] = math.log(num_docs/term_ct)

    return idfs

def find_doc_salient(doc_counts, idfs, threshold):
    '''
    Compute the salient words for a specific document. A word is salient if
    its tf-idf score is strictly above a given threshold. This function 
    computes the augmented term frequencies and uses the inverse 
    document frequencies to compute the tf-idf scores.

    Inputs:
      doc_counts: dictionary mapping terms in the document to their counts
      idfs: dictionary mapping terms to their inverse document frequencies
      threshold: float

    Returns: set of salient words in the document
    '''

    doc_salient_words = set()
    
    # Check for empty dictionary
    if len(doc_counts) == 0:
        return doc_salient_words

    max_count = max(doc_counts.values())
    
    # Iterate through terms and their counts
    for term, count in doc_counts.items():
        # Compute term frequency and tf-idf score
        term_freq = 0.5 + 0.5 * (count / max_count)
        tf_idf = term_freq * idfs[term]
        if tf_idf > threshold:
            doc_salient_words.add(term)

    return doc_salient_words

def find_salient(docs, threshold):
    '''
    Compute the salient words for each document.  A word is salient if
    its tf-idf score is strictly above a given threshold.

    Inputs:
      docs: list of list of tokens
      threshold: float

    Returns: list of sets of salient words
    '''

    terms, term_counts = count_doc_tokens(docs)
    idfs = find_inv_doc_freqs(docs, terms, term_counts)

    salient_words = []
    for doc_counts in term_counts:
        salient_words.append(find_doc_salient(doc_counts, idfs, threshold))

    return salient_words



    


