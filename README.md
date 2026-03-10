# C++ Library Template

C++17 ライブラリのテンプレートプロジェクト。
pixi による再現性の高い開発環境と、カバレッジ計測・コード品質ツールを提供する。

## 必要条件

- [pixi](https://prefix.dev/docs/pixi/overview)

pixi をインストール後、`pixi install` でコンパイラ・ツール類が自動セットアップされる。

### Linux: valgrind のインストール

conda-forge の valgrind はシステムの動的リンカ（`/lib64/ld-linux-x86-64.so.2`）を使うバイナリを
検査できないため、システムパッケージマネージャでインストールする。

```bash
sudo apt-get install valgrind   # Ubuntu/Debian
```

## クイックスタート

```bash
pixi run config   # CMake 設定（Release）
pixi run build    # ビルド
pixi run test     # テスト実行
```

## 対応プラットフォーム

| プラットフォーム              | コンパイラ        | リンカ             |
| ----------------------------- | ----------------- | ------------------ |
| Linux x86-64                  | GCC 15 / Clang 21 | mold（高速リンク） |
| macOS (Apple Silicon / Intel) | AppleClang        | system             |

## テスト・ベンチマーク用ライブラリ

| ライブラリ | バージョン | 用途                 |
| ---------- | ---------- | -------------------- |
| doctest    | 2.4.12     | テストフレームワーク |
| nanobench  | 4.3.11     | マイクロベンチマーク |

## 主要タスク一覧

```bash
# ---- 通常ワークフロー ----
pixi run config           # CMake 設定（Release）
pixi run config-debug     # CMake 設定（Debug）
pixi run build            # ビルド
pixi run test             # テスト実行
pixi run clean            # ビルド成果物をクリーン

# ---- コード品質 ----
pixi run format           # clang-format によるコード整形
pixi run lint             # clang-tidy による静的解析
pixi run run-cppcheck     # cppcheck による静的解析
pixi run fullcheck        # typos + lint + cppcheck をまとめて実行

# ---- サニタイザ（ASan + UBSan） ----
pixi run asan             # 設定 → ビルド → テストをまとめて実行（build-asan/）

# ---- カバレッジ ----
pixi run coverage         # 設定 → 計測 → HTML レポート生成（build-coverage/）

# ---- メモリチェック（Linux のみ） ----
pixi run config-debug && pixi run build
pixi run valgrind         # ctest memcheck 経由で valgrind を実行
```

詳細なビルドシステムの説明は [docs/build-system.md](docs/build-system.md) を参照。

## ビルド成果物

```text
build/
├── src/
│   ├── libtemplate_library_cpp.a      # 静的ライブラリ（デフォルト）
│   └── libtemplate_library_cpp.so     # 共有ライブラリ（BUILD_SHARED_LIBS=ON 時）
├── tests/
│   └── test_*                         # テストバイナリ
└── benches/
    └── bench_*                        # ベンチマークバイナリ
```

## ライブラリのビルド形式

`BUILD_SHARED_LIBS` CMake 変数で静的・共有ライブラリを切り替えられる。

```bash
# 静的ライブラリ（デフォルト）
cmake --preset=release
cmake --build build

# 共有ライブラリ
cmake --preset=release -DBUILD_SHARED_LIBS=ON
cmake --build build
```

## ライブラリの利用方法

### 1. ビルド済みライブラリをリンクする

ビルド後の成果物を直接使う場合：

```cmake
# CMakeLists.txt（利用側）
add_executable(my_app main.cpp)

target_include_directories(my_app PRIVATE /path/to/template_library_cpp/include)
target_link_libraries(my_app PRIVATE /path/to/template_library_cpp/build/src/libtemplate_library_cpp.a)
```

### 2. FetchContent で取得する

利用側の CMakeLists.txt に追加するだけでビルド・リンクまで自動化される。

```cmake
include(FetchContent)
FetchContent_Declare(
    template_library_cpp
    GIT_REPOSITORY https://github.com/<your-org>/template_library_cpp.git
    GIT_TAG        main  # またはタグ・コミットハッシュ
)
FetchContent_MakeAvailable(template_library_cpp)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE template_library_cpp)
```

`FetchContent_MakeAvailable` 後は `target_link_libraries` にターゲット名 `template_library_cpp` を指定するだけで
インクルードパスも自動的に設定される。

### 3. add_subdirectory で取得する

ローカルにクローンしてサブディレクトリとして追加する場合：

```cmake
add_subdirectory(third_party/template_library_cpp)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE template_library_cpp)
```

## ディレクトリ構成

- `src/` — ライブラリのソースコード
- `include/` — 公開ヘッダファイル
    - `template_library_cpp/` — ライブラリの公開インターフェース
        - `template_library_cpp.hpp` — エントリーポイントヘッダ
- `tests/` — テストコード（doctest）
    - `support/` — doctest 記述パターンサンプル
- `benches/` — ベンチマーク（nanobench）
- `cmake/` — CMake モジュール群
- `docs/` — ドキュメント

## ドキュメント

- [docs/build-system.md](docs/build-system.md) — ビルドシステム・開発ツールの詳細
