#!/usr/bin/env python3
"""Check that the curriculum lists each C++ example exactly once, in sequence."""

import re
from fnmatch import fnmatchcase
from collections import Counter
from pathlib import Path


def main():
    root = Path(__file__).resolve().parents[1]
    guide = (root / "READING_ORDER.md").read_text()
    rows = re.findall(r"^\| (\d+) \|.*?\]\(([^)]+\.cpp)\)", guide, re.MULTILINE)
    errors = []

    steps = [int(step) for step, _ in rows]
    if steps != list(range(1, len(rows) + 1)):
        errors.append("Reading steps must be sequential, starting at 1.")

    all_sources = {path.relative_to(root).as_posix() for path in root.rglob("*.cpp")}
    ignore_file = root / ".curriculumignore"
    patterns = []
    if ignore_file.exists():
        patterns = [
            line.strip() for line in ignore_file.read_text().splitlines()
            if line.strip() and not line.lstrip().startswith("#")
        ]
    excluded = {path for path in all_sources if any(fnmatchcase(path, pattern) for pattern in patterns)}
    sources = all_sources - excluded
    listed = Counter(path for _, path in rows)
    for path in sorted(sources - listed.keys()):
        errors.append(f"Missing reading-order entry: {path}")
    for path in sorted(listed.keys() - sources):
        errors.append(f"Broken source link: {path}")
    for path, count in sorted(listed.items()):
        if count != 1:
            errors.append(f"Repeated reading-order entry ({count}): {path}")

    # Each table heading advertises its actual first/last reading step.
    for section in re.split(r"(?=^### )", guide, flags=re.MULTILINE):
        if not section.startswith("### "):
            continue
        heading = section.splitlines()[0]
        numbers = [int(value) for value in re.findall(r"^\| (\d+) \|", section, re.MULTILINE)]
        if not numbers:
            continue
        expected = f"Step {numbers[0]}" if len(numbers) == 1 else f"Steps {numbers[0]}–{numbers[-1]}"
        if not heading.endswith(" · " + expected):
            errors.append(f"Incorrect step range: {heading}")

    # Check navigable links in the main learning documents, including the
    # pattern guide. External URLs and same-document anchors are not paths.
    for document in sorted(root.rglob("*.md")):
        name = document.relative_to(root)
        for target in re.findall(r"\[[^\]]*\]\(([^)]+)\)", document.read_text()):
            if "://" in target or target.startswith("#"):
                continue
            target = target.split("#", 1)[0]
            if not (document.parent / target).exists():
                errors.append(f"Broken link in {name}: {target}")

    modules = len({str(Path(source).parent) for source in sources})
    if f"**Total Reading Steps:** {len(sources)} " not in guide:
        errors.append("Repository summary has an outdated reading-step count.")
    if f"**Total Themed Modules / Folders:** {modules} Modules" not in guide:
        errors.append("Repository summary has an outdated module count.")

    if errors:
        raise SystemExit("\n".join(errors))
    print(f"Curriculum OK: {len(sources)} C++ examples, {modules} modules, sequential steps and valid links.")
    if excluded:
        print(f"Personal practice files outside the curriculum: {', '.join(sorted(excluded))}")


if __name__ == "__main__":
    main()
