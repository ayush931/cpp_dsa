#!/usr/bin/env bash
#
# Compile and run a C++ file without leaving a binary behind.
#
# Usage:
#   ./run_cpp.sh <file.cpp> [program arguments...]
#
# Examples:
#   ./run_cpp.sh "CPP-Basics/Fundamentals/helloWorld.cpp"
#   ./run_cpp.sh "CPP-Basics/Functions/Functions.cpp"
#   CXX_STANDARD=c++20 ./run_cpp.sh "Advanced-CPP/Modern-CPP17-20/concepts.cpp"
#
# Environment:
#   CXX           compiler to use (default: g++)
#   CXX_STANDARD  language standard (default: c++17; retries c++20 automatically)
#   CXXFLAGS      extra compiler flags (default: -O2 -pthread)
#
# The program is built inside a temporary directory that is removed when the
# program exits, so no .exe or other binary is created in the repository.

set -euo pipefail

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <file.cpp> [program arguments...]" >&2
  exit 1
fi

source_file=$1
shift

if [ ! -f "$source_file" ]; then
  echo "error: source file not found: $source_file" >&2
  exit 1
fi

compiler=${CXX:-g++}
standard=${CXX_STANDARD:-c++17}
extra_flags=${CXXFLAGS:--O2 -pthread}

if ! command -v "$compiler" >/dev/null 2>&1; then
  echo "error: compiler not found: $compiler" >&2
  exit 1
fi

work_dir=$(mktemp -d "${TMPDIR:-/tmp}/cpp-run.XXXXXX")
binary="$work_dir/program"

cleanup() {
  rm -rf "$work_dir"
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

# Apple clang does not ship <bits/stdc++.h>. Put a compatibility header in the
# temporary include directory so files written for GCC still compile. The
# repository itself is never modified.
mkdir -p "$work_dir/include/bits"
cat > "$work_dir/include/bits/stdc++.h" <<'HEADER'
#ifndef CPP_RUNNER_BITS_STDCPP_H
#define CPP_RUNNER_BITS_STDCPP_H

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <deque>
#include <exception>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>

#if __cplusplus >= 202002L
#include <bit>
#include <compare>
#include <concepts>
#include <numbers>
#include <ranges>
#include <source_location>
#include <span>
#endif

#endif
HEADER

compile() {
  # shellcheck disable=SC2086
  "$compiler" -std="$1" -I "$work_dir/include" $extra_flags \
    -o "$binary" "$source_file"
}

if ! compile "$standard" 2>"$work_dir/errors.txt"; then
  # If the caller did not pin a standard, retry with C++20. Several files in
  # this repository use concepts, ranges, or three-way comparison.
  if [ "$standard" = "c++17" ] && [ -z "${CXX_STANDARD:-}" ]; then
    if compile "c++20" 2>"$work_dir/errors20.txt"; then
      standard="c++20"
    else
      echo "error: compilation failed with both C++17 and C++20" >&2
      echo "C++17 diagnostics:" >&2
      cat "$work_dir/errors.txt" >&2
      echo "C++20 diagnostics:" >&2
      cat "$work_dir/errors20.txt" >&2
      exit 1
    fi
  else
    cat "$work_dir/errors.txt" >&2
    exit 1
  fi
fi

"$binary" "$@"
