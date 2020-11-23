#!/usr/bin/env python3

# ==============================================================================
# Simple Python script to convert binary files to C/C++ source files.
# Available Under Public Domain or MIT License
# Released 22-07-2020
# Version 1.0.3
# Authored by Joshua Robertson
# ==============================================================================

import os
import sys

import datetime

from argparse import ArgumentParser
from pathlib import Path

parser = ArgumentParser(description="Convert binary files to C/C++ source files.")
parser.add_argument("paths", nargs="+", help="list of binary dirs/files to convert")
parser.add_argument("--version", action="version", version="%(prog)s v1.0.3")
parser.add_argument("-r", action="store_true", help="recursively go through directories", dest="recursive", default=False)
parser.add_argument("-o", metavar="output", nargs=1, help="output directory for the source files", dest="output")
args = parser.parse_args()

def process_file (infilename):
    try:
        filenamenoext = os.path.splitext(infilename)[0]
        outfilename = filenamenoext  + ".hpp"
        if args.output:
            outfilename = os.path.basename(outfilename)
            outfilename = os.path.join(args.output[0], outfilename)
        with open(infilename,'rb') as infile:
            data = infile.read()
            with open(outfilename, 'w') as outfile:
                arrayname = os.path.basename(filenamenoext).replace(" ", "_").upper()
                arraydata = ""
                for i,byte in enumerate(data):
                    if i != 0 and i % 16 == 0:
                        arraydata += "\n"
                    arraydata += "0x{:02X}".format(byte)
                    if i != len(data)-1:
                        arraydata += ","
                outfile.write(f"""\
// File auto-generated using {os.path.basename(sys.argv[0])} on {datetime.datetime.today().strftime("%c")}.

#ifndef __{arrayname}_HPP__
#define __{arrayname}_HPP__

static const uint8_t {arrayname}[] =
{{

{arraydata}

}};

#endif // __{arrayname}_HPP__""")
                print("[SUCCESS] " + infilename)
    except OSError as e:
        print("[FAILED] " + infilename + ": " + e.strerror)

def process_dir (indirectory):
    paths = os.listdir(indirectory)
    for path in paths:
        path = os.path.join(indirectory, path)
        if os.path.isdir(path):
            if args.recursive:
                process_dir(path)
        else:
            process_file(path)

# Make the output directory if it doesn't exist.
if args.output:
    Path(args.output[0]).mkdir(parents=True, exist_ok=True)

# Process each binary file into a C/C++ source file.
for path in args.paths:
    if os.path.isdir(path):
        process_dir(path)
    else:
        process_file(path)
