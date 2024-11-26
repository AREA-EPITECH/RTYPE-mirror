//
// Created by lferraro on 11/21/24.
//

#include <criterion/criterion.h>
#include "shared/include/Sparse_array.hpp"

Test(Sparse_array, initial_state) {
    Sparse_array<int> array;
    cr_assert_eq(array.size(), 0, "Size of the sparse array should initially be 0.");
}

Test(Sparse_array, insert_and_access) {
    Sparse_array<int> array;
    array.insert_at(3, 42);
    cr_assert_eq(array.size(), 4, "Size should expand to accommodate index 3.");
    cr_assert(array[3].has_value(), "Value at index 3 should exist.");
    cr_assert_eq(array[3].value(), 42, "Value at index 3 should be 42.");
}

Test(sparse_array, erase_element) {
    Sparse_array<int> array;
    array.insert_at(2, 99);
    array.erase(2);
    cr_assert_not(array[2].has_value(), "Value at index 2 should be erased.");
}

Test(Sparse_array, emplace_element) {

    Sparse_array<int> array;
    array.emplace_at(1, 5);
    cr_assert(array[1].has_value(), "Emplaced element at index 1 should exist.");
    cr_assert_eq(array[1].value(), 5, "X value should be 5.");
}