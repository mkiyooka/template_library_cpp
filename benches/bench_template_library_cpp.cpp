#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

int main() {
    ankerl::nanobench::Bench().run("example", [&] {
        // ベンチマーク対象をここに記述する
        ankerl::nanobench::doNotOptimizeAway(0);
    });
}
