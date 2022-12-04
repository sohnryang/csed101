#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "functions.h"

static void append_list_test(void **state) {
  List list = {0, 0, NULL};
  Song song1 = {"a", "test", 0, 0}, song2 = {"b", "test", 0, 0},
       song3 = {"c", "test", 0, 0};

  append_list(&list, song2);
  assert_non_null(list.head);
  assert_int_equal(list.size, 1);
  assert_string_equal(list.head->data.title, "b");

  append_list(&list, song1);
  assert_non_null(list.head);
  assert_int_equal(list.size, 2);
  assert_string_equal(list.head->data.title, "a");
  assert_non_null(list.head->next);
  assert_string_equal(list.head->next->data.title, "b");

  append_list(&list, song3);
  assert_non_null(list.head);
  assert_int_equal(list.size, 3);
  assert_string_equal(list.head->data.title, "a");
  assert_non_null(list.head->next);
  assert_string_equal(list.head->next->data.title, "b");
  assert_non_null(list.head->next->next);
  assert_string_equal(list.head->next->next->data.title, "c");
}

static void remove_list_test(void **state) {
  List list = {0, 0, NULL};
  Song songs[] = {
      {"a", "test", 0, 0}, {"b", "test", 0, 0}, {"c", "test", 0, 0}};
  for (int i = 0; i < 3; i++)
    append_list(&list, songs[i]);

  remove_list(&list, "b");
  assert_int_equal(list.size, 2);
  assert_non_null(list.head);
  assert_string_equal(list.head->data.title, "a");
  assert_non_null(list.head->next);
  assert_string_equal(list.head->next->data.title, "c");
}

static void sort_list_test(void **state) {
  List list = {0, 0, NULL};
  Song songs[] = {
      {"a", "test", 0, 2}, {"b", "test", 0, 5}, {"c", "test", 0, 1}};
  for (int i = 0; i < 3; i++)
    append_list(&list, songs[i]);
  List *sorted = sort_list(&list);

  assert_non_null(sorted->head);
  assert_int_equal(sorted->size, 3);
  assert_string_equal(sorted->head->data.title, "b");
  assert_non_null(sorted->head->next);
  assert_string_equal(sorted->head->next->data.title, "a");
  assert_non_null(sorted->head->next->next);
  assert_string_equal(sorted->head->next->next->data.title, "c");
}

int main() {
  const struct CMUnitTest tests[] = {cmocka_unit_test(append_list_test),
                                     cmocka_unit_test(remove_list_test),
                                     cmocka_unit_test(sort_list_test)};
  return cmocka_run_group_tests(tests, NULL, NULL);
}
