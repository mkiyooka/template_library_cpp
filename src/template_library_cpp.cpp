#include "template_library_cpp/template_library_cpp.hpp"

namespace tpl {

int Calculator::add(int a, int b) const { return a + b; }

int Calculator::subtract(int a, int b) const { return a - b; }

int Calculator::multiply(int a, int b) const { return a * b; }

int Calculator::divide(int dividend, int divisor) const {
    if (divisor == 0) {
        throw std::invalid_argument("division by zero");
    }
    return dividend / divisor;
}

bool Calculator::is_positive(int value) const { return value > 0; }

}  // namespace tpl
