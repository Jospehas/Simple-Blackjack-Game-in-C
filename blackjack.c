/*
Stuff to add as of 03/09/26:
[-] Betting system
    Players should have an initial amount of money at the start of the game,
    which is betted throughout the game, wherein a pot exists. A player
    completely wins if the opponent's money becomes zero.

    Variables: player.money, player,bet, pot
    Functions: N/A
    Implementation: Players are asked to bet an amount at the start of the game,
    which is added to the pot and deducted from player.money. If a player wins,
    they win all the chips in the pot.

[-] Dealer
    What is blackjack without a dealer? The dealer has different rules compared
    to the players. They cannot double down or split like the players can. They
    must also draw on 16 and pass on 17. They also cannot bet any amount, and must
    be the pot.

[-] Double Down
    Players are able to double down on their initial bet and get drawn
    one extra card.

    Function: doubleDown
    Implementation: getCard is called and their initial bet is deducted from their
    money and added to the pot. The enemy must also be able to do this.

[-] Splitting
    Players are given the option to split their hand if they have two cards
    of the same rank. The player must also make an additional bet equal to
    their previous one.

    Player: selfSplit, enemySplit
    Function: N/A
    Implementation: The split players must already be implemented in the main
    function but must only be displayed if a player has split. The first
    card of the split player must be equal to the 2nd index of the original
    player and the 2nd index of the original player must be set to empty.

[-] Natural Blackjack and 3:2 Payout
    A natural blackjack card will be superior to any other blackjack,
    unless the dealer also has blackjack, and therefore is a draw. If
    a player does win a natural Blackjack, then it must pay 3:2.

 */

#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

const int handSize = 5;
const int deckSize = 52;

typedef enum {
    SPADES, HEARTS, DIAMONDS, CLUBS, NO_SUIT
} Suit;

typedef enum {
    NO_VALUE, ACE_1, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
    EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE_11
} Value;

typedef struct card {
    Suit suit;
    Value value;
    int blanks;
} Card;

typedef struct player {
    Card hand[5];
    char name[50];
    char decision;
    int cardCount;
    int money;
    int bet;
    bool doubledDown;
    bool splitted;
} Player;

int sum(Card hand[]) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        if (hand[i].value < TEN) {
            sum += hand[i].value;
        } else if (hand[i].value == ACE_11){
            sum += 11;
        } else {
            sum += 10;
        }
    }
    return sum;
}

void displayCard(Card hand[]) {
    switch(hand->value) {
        case ACE_1: case ACE_11:    printf("Ace");
        break;
        case JACK:  printf("Jack");
        break;
        case QUEEN: printf("Queen");
        break;
        case KING:  printf("King");
        break;
        default:    printf("%d", hand->value);
        break;
    }

    printf(" of ");

    switch(hand->suit) {
        case SPADES:    printf("Spades");
        break;
        case HEARTS:    printf("Hearts");
        break;
        case DIAMONDS:  printf("Diamonds");
        break;
        case CLUBS:     printf("Clubs");
        break;
        default:        printf("Invalid suit");
        break;
    }
}

void displayHand(Player player) {
    for (int i = 0; i < 5; i++) {
        if (player.hand[i].value != 0) {
            displayCard(&player.hand[i]);
            if (player.hand[i + 1].value != 0) {
                printf(", ");
            } else {
                printf(".");
            }
        }
    }
}

void getCard(Player *player, Card cards[], int *cardsTaken) {
    for (int i = 0; i < 5; i++) {
        Card card = cards[51 - *cardsTaken];
        if (player->hand[i].value == NO_VALUE) {
            player->hand[i] = card;
            (*cardsTaken)++;
            break;
        }
    }
}

void clear () {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();
    system("clear");
}

int main() {
    Card cards[deckSize];
    bool game_loop = true;
    bool round_loop = true;
    int game = 0;
    int round;
    int cardsTaken = 0;

    srand(time(NULL));

    // Create players
    Player self;
    Player selfSplit;
    Player enemy;
    Player enemySplit;
    Player dealer;

    // Initial Values
    self.money = 500;
    enemy.money = 500;
    // The dealer is the pot.
    dealer.money = 0;

    printf("Welcome to...\n\n");
    printf("____________                 __         __               __    \n");
    printf("\\_____    \\ | _____    ____ |  | __    |__|____    ____ |  | __\n");
    printf("|    |___/  | \\__  \\ _/ ___\\|  |/ /    |  \\__  \\ _/ ___\\|  |/ /\n");
    printf("|    |   \\  |__/ __ \\   \\___|    <     |  |/ __ \\   \\___|    <\n");
    printf("|______  /____(____  /\\___  |__|_ \\/\\__|  (____  /\\___  |__|_ \\\n");
    printf("       \\/          \\/     \\/     \\/\\______|    \\/     \\/     \\/\n");

    printf("\n\nPlease enter your name: ");
    scanf(" %s", self.name);
    printf("\nPlease enter the enemy's name: ");
    scanf(" %s", enemy.name);
    printf("Welcome, %s.\n\n", self.name);

    // Game Loop
    while(game_loop) {
        game++;

        // Initial values that should reset every time a new game starts
        self.doubledDown = false;
        self.splitted = false;
        enemy.doubledDown = false;
        enemy.splitted = false;
        dealer.money = 0;
        round = 1;

        // Fill deck.
        for (int i = 0; i < 52; i++) {
            if (i < 13) {
                cards[i].suit = SPADES;
            } else if (i < 26) {
                cards[i].suit = HEARTS;
            } else if (i < 39) {
                cards[i].suit = DIAMONDS;
            } else {
                cards[i].suit = CLUBS;
            }
            cards[i].value = i % 13 + 1;
        }
        // Shuffle deck.
        for (int j = 0; j < 1000; j++) {
            int randNum1 = rand() % 52;
            int randNum2 = rand() % 52;

            Card temp = cards[randNum1];
            cards[randNum1] = cards[randNum2];
            cards[randNum2] = temp;
        }
        //Empty hand
        for (int i = 0; i < 5; i++) {
            self.hand[i].value = NO_VALUE;        self.hand[i].suit = NO_SUIT;
            selfSplit.hand[i].value = NO_VALUE;   selfSplit.hand[i].suit = NO_SUIT;
            enemy.hand[i].value = NO_VALUE;       enemy.hand[i].suit = NO_SUIT;
            enemySplit.hand[i].value = NO_VALUE;  enemySplit.hand[i].suit = NO_SUIT;
            dealer.hand[i].value = NO_VALUE;      dealer.hand[i].suit = NO_SUIT;
        }

        printf("[Press enter to continue.]\n");
        clear();

        printf("Place your bets.\n");
        printf("Bet: ");
        scanf("%d", &self.bet);
        dealer.money += self.bet;
        // Enemy bets the same amount as the player, though it may be changed.
        dealer.money += self.bet;

        printf("You all draw two cards\n");
        getCard(&self, cards, &cardsTaken); getCard(&self, cards, &cardsTaken);
        getCard(&enemy, cards, &cardsTaken); getCard(&enemy, cards, &cardsTaken);
        getCard(&dealer, cards, &cardsTaken); getCard(&dealer, cards, &cardsTaken);
        printf("\n[Press enter to continue.]\n");
        clear();

        while(round_loop) {

            enemy.decision = 'P';

            printf("--------[Game #%d, Round #%d]--------\n", game, round);

            //Display hand
            printf("Current sum: %d\n", sum(self.hand));
            printf("Your cards: ");
            displayHand(self);
            if (self.splitted == true) {
                printf("Your split: ");
                displayHand(selfSplit);
            }

            // Display enemy hand
            printf("\nEnemy sum: %d\n", sum(enemy.hand));
            printf("Enemy's cards: ");
            displayHand(enemy);
            if (enemy.splitted == true) {
                printf("Your split: ");
                displayHand(enemySplit);
            }

            // Display dealer's hand
            printf("Dealer's cards: ");
            displayCard(&dealer.hand[0]);
            //Change value of aces
            for (int i = 0; i < 5; i++) {
                switch(self.hand[i].value) {
                    case ACE_1:
                        printf("\nChange "); displayCard(&self.hand[i]); printf(" to 11? [Y/N]: ");
                        scanf(" %c", &self.decision);
                        if (self.decision == 'Y')
                            self.hand[i].value = ACE_11;
                        printf("Current sum: %d\n", sum(self.hand));
                        break;
                    case ACE_11:
                        printf("\nChange "); displayCard(&self.hand[i]); printf(" to 1? [Y/N]: ");
                        scanf(" %c", &self.decision);
                        if (self.decision == 'Y')
                            self.hand[i].value = ACE_1;
                        printf("Current sum: %d\n", sum(self.hand));
                        break;
                    default:
                        break;
                }
            }

            if (self.splitted == false) {
                printf("Do you want to split? [Y/N]:");
                scanf(" %c", &self.decision);
                switch (self.decision) {
                    case 'Y':
                    break;
                    case 'N':
                    break;
                    default:
                    break;
                }
            } else {
                printf("\n\nWhat shall you do on your split?: \n");
                printf("[1] Draw a card\n");
                printf("[2] Pass\n");
            }

            if (self.doubledDown == false) {
                printf("\n\nWhat shall you do?: \n");
                printf("[1] Draw a card\n");
                printf("[2] Pass\n");
                if (self.splitted == false)
                    printf("[3] Double down\n");
                printf("Enter your number of choice:");

                scanf(" %c", &self.decision);

                switch (self.decision) {
                    case '1':
                        printf("You draw a card.");
                        getCard(&self, cards, &cardsTaken);
                    break;
                    case '2':
                        printf("You pass.");
                    break;
                    case '3':
                        printf("You double down.");
                        getCard(&self, cards, &cardsTaken);
                        self.money -= self.bet;
                        dealer.money += self.bet;
                    break;
                    default:
                        printf("Invalid decision.");
                        return 127;
                    break;
                }   printf("\n");
            }


            // Dealer draws on 16, passes on 17
            if (sum(dealer.hand) < 17) {
                dealer.decision = 'D';
                getCard(&dealer, cards, &cardsTaken);
                printf("\n");
            }

            // Win and lose conditions
            // Should be upgraded to fit Natural Blackjack, etc.
            if (sum(self.hand) > 21 || sum(enemy.hand) > 21
            || (self.decision == 'P' && enemy.decision == 'P')) {
                if ((sum(self.hand) > sum(enemy.hand) && sum(self.hand) <= 21)) {
                    printf("You win!\n");
                    printf("%d > %d\n\n", sum(self.hand), sum(enemy.hand));
                    break;
                } else if (sum(self.hand) <= 21 && sum(enemy.hand) > 21) {
                    printf("%s blew it. You win!\n", enemy.name);
                    printf("%d > %d\n\n", sum(self.hand), sum(enemy.hand));
                    break;
                } else if (sum(self.hand) < sum(enemy.hand) && sum(enemy.hand) <= 21) {
                    printf("You lose!\n");
                    printf("%d < %d\n\n", sum(self.hand), sum(enemy.hand));
                    break;
                } else if ((sum(self.hand) > 21 && sum(enemy.hand) <= 21)) {
                    printf("You blew it. %s wins!\n", enemy.name);
                    printf("%d < %d\n\n", sum(self.hand), sum(enemy.hand));
                    break;
                } else {
                    printf("Draw!\n\n");
                    break;
                }
            }
            clear();
            round++;
        }
    }
        return 0;
}
