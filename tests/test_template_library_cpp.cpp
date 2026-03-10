#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "template_library_cpp/template_library_cpp.hpp"

TEST_CASE("Calculator::add") {
    tpl::Calculator calc;
    CHECK(calc.add(3, 4) == 7);
    CHECK(calc.add(-1, 1) == 0);
}

TEST_CASE("Calculator::subtract") {
    tpl::Calculator calc;
    CHECK(calc.subtract(10, 3) == 7);
    CHECK(calc.subtract(0, 5) == -5);
}

TEST_CASE("Calculator::multiply") {
    tpl::Calculator calc;
    CHECK(calc.multiply(6, 7) == 42);
    CHECK(calc.multiply(-2, 3) == -6);
}

TEST_CASE("Calculator::divide") {
    tpl::Calculator calc;

    SUBCASE("normal division") {
        CHECK(calc.divide(20, 4) == 5);
        CHECK(calc.divide(-9, 3) == -3);
    }

    // 分岐カバレッジ: divisor == 0 の例外パスを通す
    SUBCASE("division by zero throws") {
        CHECK_THROWS_AS(calc.divide(1, 0), std::invalid_argument);
    }
}

TEST_CASE("Calculator::is_positive") {
    tpl::Calculator calc;

    // 分岐カバレッジ: true パスのみテスト（false パスは意図的に未テスト）
    CHECK(calc.is_positive(1) == true);
    CHECK(calc.is_positive(100) == true);
}
