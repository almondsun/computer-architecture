from __future__ import annotations

import csv
from pathlib import Path


def validate_csv(csv_path: Path) -> None:
    with csv_path.open(newline="", encoding="ascii") as csv_file:
        reader = csv.DictReader(csv_file)
        expected_columns = ["letter", "representation"]
        if reader.fieldnames != expected_columns:
            raise ValueError(
                'ascii.csv must define the columns "letter" and "representation" '
                "in that order"
            )

        for row in reader:
            letter = row["letter"]

            if len(letter) != 1:
                raise ValueError(
                    "each letter value must contain exactly one character; "
                    f"got {letter!r}"
                )

            if ord(letter) >= 128:
                raise ValueError(
                    f"only 7-bit ASCII letters are supported; got {letter!r}"
                )


def to_c_string_literal(text: str) -> str:
    normalized = text.replace("\r\n", "\n").replace("\r", "\n")
    escaped = (
        normalized.replace("\\", "\\\\")
        .replace('"', '\\"')
        .replace("\n", "\\n\"\n    \"")
    )
    return f'    "{escaped}";\n'


def render_header(csv_text: str, csv_name: str) -> str:
    return (
        "#ifndef GENERATED_ASCII_ASSET_H\n"
        "#define GENERATED_ASCII_ASSET_H\n\n"
        f"/* Auto-generated from {csv_name}. Do not edit by hand. */\n"
        "static const char k_scrolling_sign_ascii_csv[] =\n"
        f"{to_c_string_literal(csv_text)}\n"
        "#endif // GENERATED_ASCII_ASSET_H\n"
    )


def main() -> None:
    project_dir = Path(__file__).resolve().parent.parent
    csv_path = project_dir / "ascii.csv"
    header_path = (
        project_dir
        / "lib"
        / "scrolling_sign_core"
        / "src"
        / "generated_ascii_asset.h"
    )

    validate_csv(csv_path)
    csv_text = csv_path.read_text(encoding="ascii")
    header_text = render_header(csv_text, csv_path.name)
    current_text = header_path.read_text(encoding="ascii") if header_path.exists() else ""
    if current_text != header_text:
        header_path.write_text(header_text, encoding="ascii")


if __name__ == "__main__":
    main()
