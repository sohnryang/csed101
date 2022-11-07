/*
 * CSED101 Assignment #1
 * Submission by Ryang Sohn (ryangsohn@postech.ac.kr)
 * Compiled & Tested on macOS 13.0 @ AArch64 using clang v15.0.2
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// User inputs for betting
#define BET_CALL 1
#define BET_RAISE 2
#define BET_FOLD 3

// Returned amount by `*_turn` functions
#define AMOUNT_CALL 0
#define AMOUNT_FOLD -1

// Scores of card combination
#define SCORE_TRIPLE 3
#define SCORE_STRAIGHT 2
#define SCORE_DOUBLE 1
#define SCORE_NO_PAIR 0

// Player types
#define PLAYER_USER 0
#define PLAYER_COMPUTER 1

// Check if `num` is in a closed interval [a, b].
int is_valid_num(int a, int b, int num);

// Clear current screen using "clear" command.
// Either `clear` or `cls` command should exist on the system.
void clear_screen();

// Pick a random number from half-open interval [lo, hi).
// `hi` must be greater than `lo`.
int rand_range(int lo, int hi);

// Return the smaller argument.
int min(int a, int b);

// Print chips of both players, in horizontal fashion.
void print_chips_horizontal(int user_chips, int com_chips);

// Print chips of both players, in vertical fashion.
void print_chips_vertical(int user_chips, int com_chips);

// Print current game status when given round and chip counts.
void print_game_status(int round, int user_chips, int com_chips);

// Print a long horizontal line.
void print_horizontal_line();

// Create four new cards.
void card_shuffle(int *shared_card1, int *shared_card2, int *user_card,
                  int *computer_card);

// Print given cards.
// If user card should be hidden, set `user_card` = -1.
void print_card_info(int shared_card1, int shared_card2, int user_card,
                     int computer_card);

// Perform human player's betting turn.
// The player has `user_chips` from the start of the betting round, and
// `user_betting_chips` points to currently betted amount of human player. The
// opponent betted `betted_chips` before turn `turn`.
int user_turn(int user_chips, int *user_betting_chips, int betted_chips,
              int turn);

// Determine score of card combination.
int calc_hand(int card, int shared_card1, int shared_card2);

// Perform computer player's betting turn.
// The evaluation of the opponent's hand is saved in `user_hand`. The computer
// player can bet up to `com_chips`, and `com_betting_chips` points to currently
// betted amount of computer player. The opponent betted `betted_chips` before
// turn `turn`.
int computer_turn(int user_hand, int com_chips, int *com_betting_chips,
                  int betted_chips, int turn);

// Determine winner of the game.
int calc_winner(int shared_card1, int shared_card2, int user_card,
                int computer_card);

int main() {
  srand(time(NULL));
  // Both players start from 50 chips.
  int round = 1, user_chips = 50, computer_chips = 50,
      past_winner = PLAYER_COMPUTER; // the human player plays first
  while (true) {
    if (round > 10 || user_chips * computer_chips == 0)
      break;
    clear_screen();
    int shared_card1, shared_card2, user_card, computer_card;
    card_shuffle(&shared_card1, &shared_card2, &user_card, &computer_card);
    print_game_status(round, user_chips, computer_chips);
    printf("\n");
    print_card_info(shared_card1, shared_card2, -1, computer_card);
    printf("\n");

    // It doesn't matter if we calculate winner before game ends, because the
    // program has all information about cards.
    int winner =
        calc_winner(shared_card1, shared_card2, user_card, computer_card);
    // Both players start bet at least one chip each.
    int user_bet = 1, computer_bet = 1;
    if (user_chips > 1 && computer_chips > 1) {
      printf("┏━━━━━━━━━━━━━━━━━━━┓\n");
      printf("┃      Betting      ┃\n");
      printf("┗━━━━━━━━━━━━━━━━━━━┛\n");
      printf("\n");
      for (int turn_count = 0;; turn_count++) {
        print_chips_horizontal(user_bet, computer_bet);
        printf("\n");
        int amount;
        // By using modulo operation, `turn` cycles between 0 and 1.
        int turn = 1 - (past_winner + turn_count) % 2;
        if (turn == PLAYER_USER)
          amount = user_turn(user_chips, &user_bet, computer_bet, turn_count);
        else
          amount = computer_turn(
              calc_hand(user_card, shared_card1, shared_card2), computer_chips,
              &computer_bet, user_bet, turn_count);
        printf("\n");
        if (amount == AMOUNT_FOLD) {
          winner = !turn; // turn is either 0 or 1, so this works
          break;
        } else if (amount == AMOUNT_CALL)
          break;
      }
      print_chips_horizontal(user_bet, computer_bet);
      printf("\n\n");
      print_horizontal_line();
      printf("\n");
      printf("Betting Finished\n");
      printf("\n");
    }
    print_card_info(shared_card1, shared_card2, user_card, computer_card);
    printf("\n");
    if (winner == PLAYER_USER) {
      printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
      printf("┃      User win!     ┃\n");
      printf("┗━━━━━━━━━━━━━━━━━━━━┛\n");
      user_chips += computer_bet;
      computer_chips -= computer_bet;
    } else {
      printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
      printf("┃    Computer win!   ┃\n");
      printf("┗━━━━━━━━━━━━━━━━━━━━┛\n");
      computer_chips += user_bet;
      user_chips -= user_bet;
    }
    printf("\n");
    past_winner = winner;
    print_chips_vertical(user_chips, computer_chips);
    printf("\n\n");
    printf("Proceed or Not? [Go: 1, End: -1]: ");
    round++; // increment counter regardless of continuation.
    int cmd;
    scanf("%d", &cmd);
    if (cmd == -1)
      break;
  }
  round--; // round is incremented just before the loop ended.
  print_horizontal_line();
  printf("Number of Games: %d\n", round);
  printf("Chips remaining:\n");
  print_chips_vertical(user_chips, computer_chips);
  printf("\n");
  if (user_chips >= computer_chips)
    printf("User win!\n");
  else
    printf("Computer win!\n");
  return 0;
}

int is_valid_num(int a, int b, int num) { return a <= num && num <= b; }

void clear_screen() {
  system("clear"); // for UNIX based OSes only
}

int rand_range(int lo, int hi) {
  int interval_size = hi - lo;
  // If we wanted more entropy (aka. randomness), this is a bad idea...
  return rand() % interval_size + lo;
}

int min(int a, int b) { return a < b ? a : b; }

void print_chips_horizontal(int user_chips, int com_chips) {
  printf("┏━━━━━━━━━━┳━━━━━━━━━┓\n");
  printf("┃User: %2d  ┃ Com: %2d ┃\n", user_chips, com_chips);
  printf("┗━━━━━━━━━━┻━━━━━━━━━┛\n");
}

void print_chips_vertical(int user_chips, int com_chips) {
  printf("┏━━━━━━━━━━━━━━━┳━━━━━┓\n");
  printf("┃ User          ┃ %3d ┃\n", user_chips);
  printf("┃ Computer      ┃ %3d ┃\n", com_chips);
  printf("┗━━━━━━━━━━━━━━━┻━━━━━┛\n");
}

void print_game_status(int round, int user_chips, int com_chips) {
  printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃  %2d Game Starts!   ┃\n", round);
  printf("┗━━━━━━━━━━━━━━━━━━━━┛\n\n");
  printf("Chips remaining:\n");
  print_chips_vertical(user_chips, com_chips);
}

void print_horizontal_line() {
  printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
}

void card_shuffle(int *shared_card1, int *shared_card2, int *user_card,
                  int *computer_card) {
  *shared_card1 = rand_range(1, 11);
  *shared_card2 = rand_range(1, 11);
  *user_card = rand_range(1, 11);
  *computer_card = rand_range(1, 11);
}

void print_card_info(int shared_card1, int shared_card2, int user_card,
                     int computer_card) {
  printf("┏━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━┓\n");
  printf("┃  Computer  ┃       Shared Cards      ┃     User   ┃\n");
  printf("┣━━━━━━━━━━━━╋━━━━━━━━━━━━┳━━━━━━━━━━━━╋━━━━━━━━━━━━┫\n");
  printf("┃  ┏━━━━━━┓  ┃  ┏━━━━━━┓  ┃  ┏━━━━━━┓  ┃  ┏━━━━━━┓  ┃\n");
  printf("┃  ┃ ♠    ┃  ┃  ┃ ♠    ┃  ┃  ┃ ♠    ┃  ┃  ┃ ♠    ┃  ┃\n");
  printf("┃  ┃      ┃  ┃  ┃      ┃  ┃  ┃      ┃  ┃  ┃      ┃  ┃\n");
  if (user_card != -1) // check if user card should be hidden
    printf("┃  ┃  %2d  ┃  ┃  ┃  %2d  ┃  ┃  ┃  %2d  ┃  ┃  ┃  %2d  ┃  ┃\n",
           computer_card, shared_card1, shared_card2, user_card);
  else
    printf("┃  ┃  %2d  ┃  ┃  ┃  %2d  ┃  ┃  ┃  %2d  ┃  ┃  ┃   ?  ┃  ┃\n",
           computer_card, shared_card1, shared_card2);
  printf("┃  ┃      ┃  ┃  ┃      ┃  ┃  ┃      ┃  ┃  ┃      ┃  ┃\n");
  printf("┃  ┃    ♠ ┃  ┃  ┃    ♠ ┃  ┃  ┃    ♠ ┃  ┃  ┃    ♠ ┃  ┃\n");
  printf("┃  ┗━━━━━━┛  ┃  ┗━━━━━━┛  ┃  ┗━━━━━━┛  ┃  ┗━━━━━━┛  ┃\n");
  printf("┗━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━┻━━━━━━━━━━━━┛\n");
}

int user_turn(int user_chips, int *user_betting_chips, int betted_chips,
              int turn) {
  bool successful_input = false;
  int additional_amount;
  while (!successful_input) {
    printf("USER ┃  [Call: 1 | Raise: 2 | Fold: 3]: ");
    int bet_mode;
    scanf("%d", &bet_mode);
    if (!is_valid_num(1, 3, bet_mode) || (turn == 0 && bet_mode == BET_CALL)) {
      printf("USER ┃  [Invalid input]\n");
      continue;
    }
    successful_input = true;

    switch (bet_mode) {
    case BET_CALL:
      additional_amount = AMOUNT_CALL;
      *user_betting_chips = min(betted_chips, user_chips);
      break;
    case BET_RAISE:
      printf("USER ┃  [Input number of chips for Raise]: ");
      scanf("%d", &additional_amount);
      *user_betting_chips = betted_chips + additional_amount;
      break;
    case BET_FOLD:
      additional_amount = AMOUNT_FOLD;
      break;
    }
  }
  return additional_amount;
}

int calc_hand(int card, int shared_card1, int shared_card2) {
  // Check higher-scoring combinations first.
  if (card == shared_card1 && card == shared_card2)
    return SCORE_TRIPLE;
  else if ((card - shared_card1) * (shared_card1 - shared_card2) == 1 ||
           (card - shared_card2) * (shared_card2 - shared_card1) == 1 ||
           (shared_card1 - card) * (card - shared_card2) == 1 ||
           (shared_card1 - shared_card2) * (shared_card2 - card) == 1 ||
           (shared_card2 - card) * (card - shared_card1) == 1 ||
           (shared_card2 - shared_card1) * (shared_card1 - card) == 1)
    return SCORE_STRAIGHT;
  else if (card == shared_card1 || card == shared_card2)
    return SCORE_DOUBLE;
  else
    return SCORE_NO_PAIR;
}

int computer_turn(int user_hand, int com_chips, int *com_betting_chips,
                  int betted_chips, int turn) {
  if (turn == 0) { // raise by 1 if first turn
    *com_betting_chips = betted_chips + 1;
    printf("COM  ┃  Raise, +%d\n", 1);
    return 1;
  }
  if (user_hand == SCORE_NO_PAIR && rand_range(0, 2)) {
    // If the opponent is no pair, call by 50% probability.
    int additional_amount = rand_range(1, 6);
    *com_betting_chips = betted_chips + additional_amount;
    if (*com_betting_chips > com_chips) {
      *com_betting_chips = com_chips;
      printf("COM  ┃  Call\n");
      return AMOUNT_CALL;
    }
    printf("COM  ┃  Raise, +%d\n", additional_amount);
    return additional_amount;
  } else if (user_hand != SCORE_NO_PAIR && rand_range(0, 10) < 7) {
    // If the opponent is not no pair, fold by 70% probability.
    printf("COM  ┃  Fold\n");
    return AMOUNT_FOLD;
  }
  *com_betting_chips = min(com_chips, betted_chips);
  printf("COM  ┃  Call\n");
  return AMOUNT_CALL;
}

int calc_winner(int shared_card1, int shared_card2, int user_card,
                int computer_card) {
  int user_score = calc_hand(user_card, shared_card1, shared_card2),
      computer_score = calc_hand(computer_card, shared_card1, shared_card2);
  if (user_score > computer_score)
    return PLAYER_USER;
  if (user_score < computer_score)
    return PLAYER_COMPUTER;
  if (user_card >= computer_card)
    return PLAYER_USER;
  return PLAYER_COMPUTER;
}
