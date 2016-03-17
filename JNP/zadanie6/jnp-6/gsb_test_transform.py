#!/usr/bin/env python3

import sys


replacements = []
line = ""

while True:
    line = sys.stdin.readline().strip().split()
    if len(line) == 0: break
    lhs, rhs = line[0], "".join(line[2:])

    assert rhs != ""
    replacements.append((rhs, "[[" + lhs + "]]"))


for nextline in sys.stdin:
    for replacement in replacements:
        nextline = nextline.replace(replacement[0], replacement[1])

    print(nextline.strip())
