import re


def dma_Select(path, new_channels):
    with open(path) as file:
        code = file.read()

    keywords = ("DCH?", "Dma?", "DMA?")

    pattern = "|".join(k.replace("?", r"([0-7])") for k in keywords)

    matches = re.findall(pattern, code)

    old_channels = sorted({c for m in matches for c in m if c})

    for old_channel, new_channel in zip(old_channels, new_channels):
        old_keywords = [k.replace("?", old_channel) for k in keywords]
        new_keywords = [k.replace("?", f"${new_channel}$") for k in keywords]

        for old_keyword, new_keyword in zip(old_keywords, new_keywords):
            code = code.replace(old_keyword, new_keyword)

    code = code.replace("$", "")

    with open(path, "w") as file:
        file.write(code)


dma_Select("Spi/Spi1DmaTx.c", (0,))

dma_Select("Spi/Spi3Dma.c", (2, 3))

dma_Select("Spi/Spi4Dma.c", (4, 5))

dma_Select("Spi/Spi6Dma.c", (6, 7))

dma_Select("Uart/Uart1DmaTx.c", (1,))
