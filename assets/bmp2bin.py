# Simple script to convert each BMP in the given
# directory to a bin array header

from PIL import Image, ImageFile
from os import sys, path, walk

header_entry_format = """
#define IMG_{0}_x ({1})
#define IMG_{0}_y ({2})
#pragma pack(push, 1)
static const uint16_t IMG_{0}_data[] = {3};
#pragma pack(pop)
"""

header_output_format = """#ifndef {0}_
#define {0}_

#include <stdint.h>
{1}

#endif // {0}_
"""


def rgb16(r: int, g: int, b: int) -> str:
    rvalue = 0x0

    rvalue += g >> 0x5
    rvalue += (g & 0x7) << 0xD
    rvalue += (r >> 0x3) << 0x8
    rvalue += (b >> 0x3) << 0x3

    return f"0x{rvalue:04x}"


def process_bmp(file_path: str) -> str:
    image: ImageFile.ImageFile = Image.open(file_path)

    if image.format != "BMP":
        print(f"Skipping {file_path} as it was not a BMP file...")

        return ""

    size = image.size
    image_name = path.splitext(path.basename(file_path))[0].replace(" ", "_")
    pixels = image.getdata()
    data_list = []

    for px in pixels:
        data_list.append(rgb16(px[0], px[1], px[2]))

    data = f"{{{', '.join(data_list)}}}"

    return header_entry_format.format(image_name, size[0], size[1], data)


def main(args: list[str]):
    if len(args) < 3:
        print(f"Incorrect usage. Usage: {args[0]} [input dir] [output file]")

        return 1

    input_dir = args[1]
    if not path.exists(input_dir):
        print(f"Unknown directory: {input_dir}")

    headers = []

    for root, dirs, files in walk(input_dir):
        for file in files:
            if not file.endswith(".bmp") and not file.endswith(".bmap"):
                continue  # Skip!

            print(f"Processing image {file}")

            data = process_bmp(path.join(root, file))
            if len(data) > 0:
                headers.append(data)

    output_file = args[2]
    with open(output_file, 'w') as header_file:
        output_file_name = path.basename(output_file).upper().replace('.', '_')

        header_file.write(header_output_format.format(
            output_file_name, '\n'.join(headers)))

    print(f"Writing to output {output_file}")


if __name__ == "__main__":
    main(sys.argv)
