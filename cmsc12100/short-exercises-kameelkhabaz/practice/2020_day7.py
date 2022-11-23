import re
lines = ['light red bags contain 1 bright white bag, 2 muted yellow bags.','dark orange bags contain 3 bright white bags, 4 muted yellow bags.','bright white bags contain 1 shiny gold bag.','muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.','shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.','dark olive bags contain 3 faded blue bags, 4 dotted black bags.','vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.','faded blue bags contain no other bags.','dotted black bags contain no other bags.']
lines = []
with open('2020_day7_input.txt','r') as file:
    for line in file:
        lines.append(line)
rules = read_rules(lines)

def read_rules(lines):
    """
    Parses the string representation of the rules.

    Input:
      - lines (list of strings): Each string is one rule (one line
        in the input, as specificed in the problem)

    Returns: Dictionary where the keys are colors, and the values are
        lists of (color, amount) tuples. This represents the number
        of bags of other colors that can be contained in a bag of a
        given color.
    """

    rules = {}

    for line in lines:
        m = re.match("(.*) bags contain (.*)\.", line)
        container_bag, contained_bags = m.groups()

        if contained_bags == "no other bags":
            bags = []
        else:
            bags = []
            bag_strs = contained_bags.split(", ")
            for bag in bag_strs:
                amount, color1, color2, _ = bag.split()
                bags.append((f"{color1} {color2}", int(amount)))

        rules[container_bag] = bags

    return rules

def task1(rules, color, valid_bags):
    '''
    Determine number of bags containing a specific color recursively.
    Inputs:
      rules (dict): the input dictionary 
      color (string): the string that we check
      valid_bags (set): bags that have already been counted 
    Returns: 
      count (int): count of the number of bags that contain a specific color
    '''
    # base case: no bags contain the color
    # recursive case: when a bag contains the color 
    count = 0
    for outer_bag, value in rules.items():
        inner_bags = [inner_bag for inner_bag, _ in value]
        if color in inner_bags and outer_bag not in valid_bags:
            valid_bags.add(outer_bag)
            count += 1
            count += task1(rules, outer_bag, valid_bags)
    return count 

def task2(rules, color):
    '''
    Determine number of individual bags required inside single bag
    Inputs:
      rules (dict): the input dictionary 
      color (string): the string that is the bag color we check
    Returns: 
      count (int): count of the number of bags inside single bag  
    '''
    count = 0
    # base case: list is empty, loop doesn't run, return 0
    for inner_bag, num in rules[color]:
        # recursive case: bag contains other bags 
        # add number of inner bags IMMEDIATELY in the color bag
        count += num 
        count += num * task2(rules,inner_bag) 
        # if inner bags contains other bags, add these
    return count