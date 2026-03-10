#pragma once

#include <stdexcept>

namespace tpl {

/// 四則演算を提供するシンプルな計算機クラス。
/// カバレッジの各指標（行・関数・分岐）を確認できるよう
/// ゼロ除算チェックなどの分岐を含む。
class Calculator {
public:
    /// 加算
    int add(int a, int b) const;

    /// 減算
    int subtract(int a, int b) const;

    /// 乗算
    int multiply(int a, int b) const;

    /// 除算。divisor が 0 の場合は std::invalid_argument を投げる。
    int divide(int dividend, int divisor) const;

    /// 値が正（> 0）なら true を返す。
    bool is_positive(int value) const;
};

}  // namespace tpl
