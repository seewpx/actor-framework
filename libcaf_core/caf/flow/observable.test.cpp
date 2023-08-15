// This file is part of CAF, the C++ Actor Framework. See the file LICENSE in
// the main distribution directory for license terms and copyright or visit
// https://github.com/actor-framework/actor-framework/blob/master/LICENSE.

#include "caf/flow/observable.hpp"

#include "caf/test/caf_test_main.hpp"
#include "caf/test/fixture/flow.hpp"
#include "caf/test/nil.hpp"
#include "caf/test/test.hpp"

using caf::test::nil;
using std::vector;

using namespace caf;

WITH_FIXTURE(test::fixture::flow) {

// Note: we always double the checks for the operator-under-test by calling it
//       once on a blueprint and once on an actual observable. This ensures that
//       the blueprint and the observable both offer the same functionality.

TEST("skip(n) skips the first n elements in a range of size m") {
  SECTION("skip(0) does nothing") {
    SECTION("blueprint") {
      check_eq(collect(range(1, 0).skip(0)), nil);
      check_eq(collect(range(1, 1).skip(0)), vector{1});
      check_eq(collect(range(1, 2).skip(0)), vector{1, 2});
      check_eq(collect(range(1, 3).skip(0)), vector{1, 2, 3});
    }
    SECTION("observable") {
      check_eq(collect(mat(range(1, 0)).skip(0)), nil);
      check_eq(collect(mat(range(1, 1)).skip(0)), vector{1});
      check_eq(collect(mat(range(1, 2)).skip(0)), vector{1, 2});
      check_eq(collect(mat(range(1, 3)).skip(0)), vector{1, 2, 3});
    }
  }
  SECTION("skip(n) truncates the first n elements if n < m") {
    SECTION("blueprint") {
      check_eq(collect(range(1, 3).skip(1)), vector{2, 3});
      check_eq(collect(range(1, 3).skip(2)), vector{3});
    }
    SECTION("observable") {
      check_eq(collect(mat(range(1, 3)).skip(1)), vector{2, 3});
      check_eq(collect(mat(range(1, 3)).skip(2)), vector{3});
    }
  }
  SECTION("skip(n) drops all elements if n >= m") {
    SECTION("blueprint") {
      check_eq(collect(range(1, 3).skip(3)), nil);
      check_eq(collect(range(1, 3).skip(4)), nil);
    }
    SECTION("observable") {
      check_eq(collect(mat(range(1, 3)).skip(3)), nil);
      check_eq(collect(mat(range(1, 3)).skip(4)), nil);
    }
  }
  SECTION("skip(n) stops early when the next operator stops early") {
    SECTION("blueprint") {
      check_eq(collect(range(1, 5).skip(2).take(0)), nil);
      check_eq(collect(range(1, 5).skip(2).take(1)), vector{3});
      check_eq(collect(range(1, 5).skip(2).take(2)), vector{3, 4});
      check_eq(collect(range(1, 5).skip(2).take(3)), vector{3, 4, 5});
      check_eq(collect(range(1, 5).skip(2).take(4)), vector{3, 4, 5});
    }
    SECTION("observable") {
      check_eq(collect(mat(range(1, 5)).skip(2).take(0)), nil);
      check_eq(collect(mat(range(1, 5)).skip(2).take(1)), vector{3});
      check_eq(collect(mat(range(1, 5)).skip(2).take(2)), vector{3, 4});
      check_eq(collect(mat(range(1, 5)).skip(2).take(3)), vector{3, 4, 5});
      check_eq(collect(mat(range(1, 5)).skip(2).take(4)), vector{3, 4, 5});
    }
  }
  SECTION("skip(n) forwards errors") {
    SECTION("blueprint") {
      check_eq(collect(obs_error().skip(0)), sec::runtime_error);
      check_eq(collect(obs_error().skip(1)), sec::runtime_error);
    }
    SECTION("observable") {
      check_eq(collect(mat(obs_error()).skip(0)), sec::runtime_error);
      check_eq(collect(mat(obs_error()).skip(1)), sec::runtime_error);
    }
  }
}

} // WITH_FIXTURE(test::fixture::flow)

CAF_TEST_MAIN()