def replace(path, keywords, old_ids, new_ids):
    with open(path) as file:
        contents = file.read()

    for old_id, new_id in zip(old_ids, new_ids):
        old_keywords = [k.replace("?", str(old_id)) for k in keywords]
        new_keywords = [k.replace("?", str(new_id)) for k in keywords]

        for old_keyword, new_keyword in zip(old_keywords, new_keywords):
            contents = contents.replace(old_keyword, new_keyword)

    with open(path, "w") as file:
        file.write(contents)


keywords = ("DCH?", "Dma?", "DMA?")

replace("Spi/Spi1DmaTX.c", keywords, (0,), (0,))

replace("Spi/Spi3Dma.c", keywords, (0, 1), (2, 3))

replace("Spi/Spi4Dma.c", keywords, (0, 1), (4, 5))

replace("Spi/Spi6Dma.c", keywords, (0, 1), (6, 7))
