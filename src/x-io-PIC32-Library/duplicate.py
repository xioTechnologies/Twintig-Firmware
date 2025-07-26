def duplicate(files, keywords, source_id, destination_ids):
    source_files = [f.replace("?", str(source_id)) for f in files]
    source_keywords = [k.replace("?", str(source_id)) for k in keywords]

    for id in destination_ids:
        destination_files = [f.replace("?", str(id)) for f in files]
        destination_keywords = [k.replace("?", str(id)) for k in keywords]

        for index, _ in enumerate(source_files):
            with open(source_files[index]) as file:
                code = file.read()

            for keyword_index, _ in enumerate(source_keywords):
                code = code.replace(source_keywords[keyword_index], destination_keywords[keyword_index])

            with open(destination_files[index], "w") as file:
                file.write(code)


duplicate(
    ("Spi/SpiBus?.c", "Spi/SpiBus?.h"),
    ("SpiBus?", "SPI_BUS_?"),
    1,
    (2, 3, 4, 5, 6),
)
