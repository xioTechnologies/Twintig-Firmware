from pathlib import Path


def duplicate(
    files: tuple[str, ...],
    keywords: tuple[str, ...],
    source_id: int,
    destination_ids: tuple[int, ...],
):
    source_files = [Path(f.replace("?", str(source_id))) for f in files]
    source_keywords = [k.replace("?", str(source_id)) for k in keywords]

    for destination_id in destination_ids:
        destination_files = [Path(f.replace("?", str(destination_id))) for f in files]
        destination_keywords = [k.replace("?", str(destination_id)) for k in keywords]

        for index, _ in enumerate(source_files):
            code = source_files[index].read_text()

            for keyword_index, _ in enumerate(source_keywords):
                code = code.replace(source_keywords[keyword_index], destination_keywords[keyword_index])

            destination_files[index].write_text(code)


duplicate(
    ("Icm?.c", "Icm?.h"),
    ("ICM?", "Icm?", "icm?"),
    1,
    (2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20),
)
