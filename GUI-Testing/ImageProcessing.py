from PIL import Image
import glob
from fractions import Fraction
import argparse

parser = argparse.ArgumentParser()

parser.add_argument("-path", "--path", type=str,
                    help="Input the data's path, if not specified=D:\\\\combined/*.jpg",
                    default="D:\\\\combined/*.jpg")

args = parser.parse_args()

unique = set()
for filename in glob.glob(args.path):
    with Image.open(filename) as im:
        h, w = im.size[0], im.size[1]
        unique.add(f"{h}x{w}")
        tmp = Fraction(h, w)
        unique.add(f"{tmp}")

print(unique)
