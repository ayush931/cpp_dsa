# C++ and Data Structures & Algorithms

A learning repository covering C++ fundamentals,
advanced C++ language features, data structures, algorithms, system design
DSA, and low-level design (LLD).

## 📚 Curriculum & Reading Order

For the complete step-by-step learning progression from Beginner to Master,
see **[`READING_ORDER.md`](READING_ORDER.md)**.

Use **[`PATTERNS.md`](PATTERNS.md)** to recognize problem-solving patterns and
choose practice questions. **[`CONCEPTS.md`](CONCEPTS.md)** maps the topic coverage.
Specialist algorithms and systems examples are educational demonstrations;
their input assumptions and complexity notes are part of each lesson.

## System design

Start with [the system-design learning path](System-Design/README.md): foundations,
48 executable models, eight high-level design walkthroughs, and an explicit
[coverage/extension inventory](System-Design/COVERAGE.md). Run its examples and
failure regressions with `python3 scripts/check_system_design.py`.

## Running a file

Compile and run any `.cpp` file without leaving a binary in the repository:

```bash
./run_cpp.sh "CPP-Basics/Fundamentals/helloWorld.cpp"
```

Pass program arguments after the file name. The script builds inside a
temporary directory and removes it when the program exits, so no `.exe`
or `a.out` is created.

C++20 files are detected automatically when `CXX_STANDARD` is not set:

```bash
./run_cpp.sh "Advanced-CPP/Modern-CPP17-20/concepts.cpp"
```

To pin the standard or switch compilers:

```bash
CXX_STANDARD=c++20 CXX=clang++ ./run_cpp.sh path/to/file.cpp
```

Files that include `<bits/stdc++.h>` also work on macOS: the script supplies
a temporary compatibility header without changing the repository.

## Keeping examples consistent

Keep each lesson in its topic folder as a standalone `.cpp` file with a runnable
`main()`. Explain the problem or concept, the algorithm's invariant, time and
space complexity, input assumptions, and representative edge cases. Use
descriptive names, four-space indentation, and braced control flow. The
`.clang-format` file captures the formatting used for new and edited lessons.

To format all C/C++ source and header files, run this from the repository root
with `rg` (ripgrep) and `clang-format` installed:

```bash
rg --files -0 -g '*.{c,cc,cpp,cxx,h,hpp,hxx}' |
  xargs -0 clang-format -i --style=file
```

This edits files in place using `.clang-format` and respects `.gitignore`.

After adding a lesson, put it after its prerequisites in `READING_ORDER.md`,
update step numbers and module ranges, and check navigation with:

```bash
python3 scripts/check_curriculum.py
```

Personal scratch files such as root-level `learning.cpp` can be listed in
`.curriculumignore`; they remain runnable but are excluded from numbered
curriculum entries and lesson counts.

For an additional runtime check on a lesson:

```bash
CXX=clang++ CXXFLAGS='-O1 -g -pthread -fsanitize=address,undefined' \
  ./run_cpp.sh Techniques/Sliding-Window/subarrays_with_k_distinct.cpp
```
