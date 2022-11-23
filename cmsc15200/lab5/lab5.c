#include <stdio.h>
#include <stdlib.h>

/* Kameel Khabaz
 * Lab 5
 * February 15, 2022 */

enum face {
    JACK, QUEEN, KING
};

enum suit {
    HEARTS, DIAMONDS, SPADES, CLUBS
};

enum card_tag {
    FACE, NUMBERED
};

struct face_card {
    enum face rank;
    enum suit suit;
};

struct numbered_card {
    unsigned int rank;
    enum suit suit;
};

union card_type {
    struct face_card f;
    struct numbered_card n;
};

struct card {
    union card_type type;
    enum card_tag tag;
};

enum suit get_suit(struct card card)
{
    /* Get the suit for a card of any type */
    enum card_tag tag = card.tag;
    if (tag == FACE) {
        return card.type.f.suit;
    } else if (tag == NUMBERED) {
        return card.type.n.suit;
    } else {
        fprintf(stderr,"Error: invalid card tag %d\n",card.tag);
        exit(1);
    }
}
 
enum suit flush(struct card *hand, unsigned int num_cards)
{
    // Get suit for 1st card
    enum suit suit = get_suit(hand[0]);
    for (int i = 1; i < num_cards; i++) {
        // check if other cards have same type 
        if (get_suit(hand[i]) != suit) {
            return -1;
        }
    }
    return suit;
}

int N_of_a_kind(struct card *hand, unsigned int num_cards, unsigned int N)
{
    unsigned int counts[13] = {0};
    enum face f_rank;
    unsigned int n_rank;
    // store counts as A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K
    for (int i = 0; i < num_cards; i++) {
        if (hand[i].tag == FACE) {
            f_rank = hand[i].type.f.rank;
            if (++counts[10 + f_rank] >= N)
                return 1;
        } else if (hand[i].tag == NUMBERED) {
            n_rank = hand[i].type.n.rank;
            if (++counts[n_rank - 1] >= N) 
                return 1;
        } else {
            fprintf(stderr,"Error: invalid card tag %d\n",hand[0].tag);
            exit(1);
        }
    }
    return 0;
}

unsigned int twenty_one(struct card *hand, unsigned int num_cards)
{
    unsigned int score = 0;
    unsigned int n_rank;
    unsigned int n_aces = 0; // keep track of # of aces, then add at end 
    for (int i = 0; i < num_cards; i++) {
        if (hand[i].tag == FACE) {
            score += 10;
        } else if (hand[i].tag == NUMBERED) {
            n_rank = hand[i].type.n.rank;
            if (n_rank > 1) { 
                // if not an ace add rank value
                score += n_rank;    
            } else if (n_rank == 1) {
                n_aces++;
            } else { 
                // invalid rank
                fprintf(stderr,"Error: invalid rank %d\n", n_rank);
                exit(1);
            }    
        }
    }
    for (int i = 0; i < n_aces; i++) {
        if (score + 11 <= 21) {
            score += 11;
        } else {
            score++;
        }
    }
    return score;
}

struct card make_numbered_card(unsigned int rank, enum suit suit)
{
    struct card card;
    card.tag = NUMBERED;
    card.type.n.rank = rank;
    card.type.n.suit = suit;
    return card;
}


struct card make_face_card(enum face rank, enum suit suit)
{
    struct card card;
    card.tag = FACE;
    card.type.f.rank = rank;
    card.type.f.suit = suit;
    return card;
}

int main()
{
    struct card c1 = make_numbered_card(1, CLUBS);
    struct card c2 = make_numbered_card(9, DIAMONDS);
    struct card c3 = make_numbered_card(3, HEARTS);
    struct card hand[] = {c1, c2, c3};
    printf("Testing hand {Ace of Clubs, 9 of Diamonds, 3 of Hearts}\n");
    printf("Testing if hand is a flush: expect -1, got %d\n", flush(hand, 3));
    printf("Testing if hand has 0 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand,3,0));
    printf("Testing if hand has 1 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand,3,1));
    printf("Testing if hand has 2 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand,3,2));
    printf("Testing if hand has 3 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand,3,3));
    printf("Computing score: expect 13, got %d\n", twenty_one(hand,3));   

    struct card c4 = make_face_card(JACK, CLUBS);
    struct card c5 = make_face_card(KING, SPADES);
    struct card hand2[] = {c1, c4};
    printf("Testing hand {Jack of Clubs, Ace of Clubs}\n");
    printf("Testing if hand is a flush: expect 3, got %d\n", flush(hand2, 2));
    printf("Testing if hand has 0 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand2,2,0));
    printf("Testing if hand has 1 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand2,2,1));
    printf("Testing if hand has 2 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand2,2,2));
    printf("Testing if hand has 3 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand2,2,3));
    printf("Computing score: expect 21, got %d\n", twenty_one(hand2,2));  
    
    struct card hand3[] = {c2, c4, c5};
    printf("Testing hand {9 of Diamonds, Jack of Clubs, King of Spades}\n");
    printf("Testing if hand is a flush: expect -1, got %d\n", flush(hand3, 3));
    printf("Testing if hand has 0 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand3,3,0));
    printf("Testing if hand has 1 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand3,3,1));
    printf("Testing if hand has 2 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand3,3,2));
    printf("Testing if hand has 3 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand3,3,3));
    printf("Computing score: expect 29, got %d\n", twenty_one(hand3,3));  
    
    struct card c6 = make_face_card(KING,CLUBS);
    struct card hand4[] = {c1, c5, c6};
    printf("Testing hand {Ace of Clubs, King of Spades, King of Clubs}\n");
    printf("Testing if hand is a flush: expect -1, got %d\n", flush(hand4, 3));
    printf("Testing if hand has 0 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand4,3,0));
    printf("Testing if hand has 1 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand4,3,1));
    printf("Testing if hand has 2 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand4,3,2));
    printf("Testing if hand has 3 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand4,3,3));
    printf("Computing score: expect 21, got %d\n", twenty_one(hand4,3));  
    
    struct card c7 = make_numbered_card(1,CLUBS);
    struct card hand5[] = {c1, c6, c7};
    printf("Testing hand {Ace of Clubs, King of Clubs, Ace of Clubs}\n");
    printf("Testing if hand is a flush: expect 3, got %d\n", flush(hand5, 3));
    printf("Testing if hand has 0 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand5,3,0));
    printf("Testing if hand has 1 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand5,3,1));
    printf("Testing if hand has 2 of a kind: expect %d got %d\n",1,
            N_of_a_kind(hand5,3,2));
    printf("Testing if hand has 3 of a kind: expect %d got %d\n",0,
            N_of_a_kind(hand5,3,3));
    printf("Computing score: expect 22, got %d\n", twenty_one(hand5,3));  

    return 0;
}
    

