"""
CS 121: Analyzing Election Tweets

Kameel Khabaz

Analyze module

Functions to analyze tweets. 
"""

import unicodedata
import sys

from basic_algorithms import find_top_k, find_min_count, find_salient

##################### DO NOT MODIFY THIS CODE #####################

def keep_chr(ch):
    '''
    Find all characters that are classifed as punctuation in Unicode
    (except #, @, &) and combine them into a single string.
    '''
    return unicodedata.category(ch).startswith('P') and \
        (ch not in ("#", "@", "&"))

PUNCTUATION = " ".join([chr(i) for i in range(sys.maxunicode)
                        if keep_chr(chr(i))])

# When processing tweets, ignore these words
STOP_WORDS = ["a", "an", "the", "this", "that", "of", "for", "or",
              "and", "on", "to", "be", "if", "we", "you", "in", "is",
              "at", "it", "rt", "mt", "with"]

# When processing tweets, words w/ a prefix that appears in this list
# should be ignored.
STOP_PREFIXES = ("@", "#", "http", "&amp")


#####################  MODIFY THIS CODE #####################


############## Part 2 ##############

# Task 2.1
def get_all_entities(tweets, entity_desc):
    '''
    Get a list of all of the entities in a tweet.

    Inputs:
      tweets: a list of tweets
      entity_desc: a tuple such as ("hashtags", "text", True),
        ("user_mentions", "screen_name", False), etc.
    
    Returns: list of entities
    '''

    key, subkey, case_sensitive = entity_desc

    all_entities = [] # List of all of the entities
    for tweet in tweets:
        # List of the entities of type key in the tweet
        entities = tweet["entities"][key]
        for entity in entities:
            if case_sensitive:
                all_entities.append(entity[subkey])
            else:
                all_entities.append(entity[subkey].lower())

    return all_entities

def find_top_k_entities(tweets, entity_desc, k):
    '''
    Find the k most frequently occuring entitites.

    Inputs:
      tweets: a list of tweets
      entity_desc: a tuple such as ("hashtags", "text", True),
        ("user_mentions", "screen_name", False), etc.
      k: integer

    Returns: list of entities
    '''
    
    tweet_entries = get_all_entities(tweets, entity_desc)
    return find_top_k(tweet_entries, k)


# Task 2.2
def find_min_count_entities(tweets, entity_desc, min_count):
    '''
    Find the entitites that occur at least min_count times.

    Inputs:
        tweets: a list of tweets
        entity_desc: a triple such as ("hashtags", "text", True),
          ("user_mentions", "screen_name", False), etc.
        min_count: integer

    Returns: set of entities
    '''
    
    tweet_entries = get_all_entities(tweets, entity_desc)
    return find_min_count(tweet_entries, min_count)



############## Part 3 ##############

# Pre-processing step and representing n-grams

def preprocessing(tweet, case_sensitive, elim_stop_words):
    '''
    Perform preprocessing of a tweet by converting the abridged text
    into a list of cleaned words
    
    Inputs:
      tweet: the tweet, which is a dictionary
      case_sensitive: boolean 
      elim_stop_words: boolean 
    
    Returns: list of strings
    '''

    abridged_text = tweet["abridged_text"]

    # Turn abridged text into list of words
    words = abridged_text.split()
    words_keep = []

    for word in words:
        # Remove punctuation
        cleaned_word = word.strip(PUNCTUATION)

        # If word has become empty string, just move on
        if cleaned_word == "":
            continue

        # Make word lower case if not case sensitive
        if not case_sensitive:
            cleaned_word = cleaned_word.lower()

        # Skip the word if we eliminate stop words and this is a stop word
        if elim_stop_words and cleaned_word in STOP_WORDS:
            continue 

        # Skip words that start with punctuation
        if not cleaned_word.startswith(STOP_PREFIXES):
            words_keep.append(cleaned_word)

    return words_keep

# Task 3.1
def compute_n_gram(words, n):
    '''
    Compute n-grams of a tweet after pre-processing the tweet's abridged text.
    
    Inputs:
      words: a list of strings 
      n: positive integer
    
    Returns: list of tuples of n-grams
    '''

    num_words = len(words)
    n_grams = []

    for i in range(num_words - (n - 1)):
        n_gram = tuple(words[i : i + n])
        n_grams.append(n_gram)

    return n_grams

def get_n_grams(tweets, case_sensitive, elim_stop_words, n):
    '''
    Get a list of all of the n-grams in all tweets. The function 
    first preprocesses the text in each tweet and then constructs
    the list of n-grams.
    
    Inputs:
      tweets: a list of tweets
      case_sensitive: boolean
      elim_stop_words: boolean
      n: postive integer
    
    Returns: a list of n-grams
    '''

    n_grams = []

    # Construct a list of all of the n-grams from all tweets
    for tweet in tweets:
        words = preprocessing(tweet, case_sensitive, elim_stop_words)
        n_grams.extend(compute_n_gram(words, n))

    return n_grams

def find_top_k_ngrams(tweets, n, case_sensitive, k):
    '''
    Find k most frequently occurring n-grams.

    Inputs:
        tweets: a list of tweets
        n: integer
        case_sensitive: boolean
        k: integer

    Returns: list of n-grams
    '''

    n_grams = get_n_grams(tweets, case_sensitive, True, n)

    return find_top_k(n_grams, k)


# Task 3.2
def find_min_count_ngrams(tweets, n, case_sensitive, min_count):
    '''
    Find n-grams that occur at least min_count times.

    Inputs:
        tweets: a list of tweets
        n: integer
        case_sensitive: boolean
        min_count: integer

    Returns: set of n-grams
    '''

    n_grams = get_n_grams(tweets, case_sensitive, True, n)

    return find_min_count(n_grams, min_count)


# Task 3.3
def find_salient_ngrams(tweets, n, case_sensitive, threshold):
    '''
    Find the salient n-grams for each tweet.

    Inputs:
        tweets: a list of tweets
        n: integer
        case_sensitive: boolean
        threshold: float

    Returns: list of sets of strings
    '''

    n_grams = []

    # Construct a list of lists of the n-grams from the tweets
    for tweet in tweets:
        words = preprocessing(tweet, case_sensitive, False)
        n_grams.append(compute_n_gram(words, n))

    return find_salient(n_grams, threshold)
