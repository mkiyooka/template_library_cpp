/// @file calculator_demo.cpp
/// @brief Calculator クラスの使い方を示すサンプル。
///
/// このファイルはカバレッジレポートで各指標を確認するために使用する:
///   - 行カバレッジ  : 各 std::cout 行が実行されているか
///   - 関数カバレッジ: main() が呼ばれているか
///   - 分岐カバレッジ: is_positive() の true/false 両分岐を通るか

#include <iostream>

#include "template_library_cpp/template_library_cpp.hpp"

int main() {
    tpl::Calculator calc;

    // --- 四則演算 ---
    std::cout << "3 + 4 = " << calc.add(3, 4) << "\n";        // 7
    std::cout << "10 - 3 = " << calc.subtract(10, 3) << "\n"; // 7
    std::cout << "6 * 7 = " << calc.multiply(6, 7) << "\n";   // 42
    std::cout << "20 / 4 = " << calc.divide(20, 4) << "\n";   // 5

    // --- 分岐カバレッジを示す例 ---
    // is_positive(): true パスのみ通る（false パスは意図的に未実行）
    for (int v : {1, 2, 3}) {
        if (calc.is_positive(v)) {
            std::cout << v << " is positive\n";
        } else {
            // このパスはテストで実行されない（カバレッジレポートで赤く表示される）
            std::cout << v << " is not positive\n";
        }
    }

    // --- ゼロ除算の例外パスは tests/ でカバーする ---

    return 0;
}
