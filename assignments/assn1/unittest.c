#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#define UNIT_TESTING
#include "assn1.c"

static void is_valid_num_test(void **state) {
  assert_true(is_valid_num(1, 2, 1));
  assert_false(is_valid_num(1, 2, 3));
}

static void min_test(void **state) {
  assert_int_equal(min(1, 2), 1);
  assert_int_equal(min(2, 4), 2);
}

static void calc_hand_test(void **state) {
  assert_int_equal(calc_hand(1, 1, 1), SCORE_TRIPLE);
  assert_int_equal(calc_hand(2, 1, 3), SCORE_STRAIGHT);
  assert_int_equal(calc_hand(1, 1, 2), SCORE_DOUBLE);
  assert_int_equal(calc_hand(1, 2, 4), SCORE_NO_PAIR);
}

static void calc_winner_test(void **state) {
  assert_int_equal(calc_winner(1, 1, 1, 2), PLAYER_USER);
  assert_int_equal(calc_winner(2, 3, 1, 4), PLAYER_COMPUTER);
  assert_int_equal(calc_winner(2, 2, 2, 2), PLAYER_USER);
  assert_int_equal(calc_winner(2, 3, 2, 3), PLAYER_COMPUTER);
  assert_int_equal(calc_winner(2, 4, 6, 8), PLAYER_COMPUTER);
  assert_int_equal(calc_winner(2, 2, 2, 2), PLAYER_USER);
}

static void computer_bet_test(void **state) {
  int com_betted = 1, user_betted = 1;
  int amount = computer_turn(SCORE_NO_PAIR, 50, &com_betted, user_betted, 0);
  assert_int_equal(amount, 1);
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(is_valid_num_test), cmocka_unit_test(min_test),
      cmocka_unit_test(computer_bet_test), cmocka_unit_test(calc_hand_test),
      cmocka_unit_test(calc_winner_test)};
  return cmocka_run_group_tests(tests, NULL, NULL);
}
