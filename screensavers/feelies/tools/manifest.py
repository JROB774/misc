#!/usr/bin/env python3

import datetime

MANIFEST_FILE = "resource/resources/.manifest"

AUTHOR  = "JoshuaRobertson"
PROJECT = "Screensaver"
APPNAME = "Feelies"

MAJOR = 1
MINOR = 0
PATCH = 0
BUILD = 0

MANIFEST = f"""<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
  <assemblyIdentity type="win32" name="{AUTHOR}.{PROJECT}.{APPNAME}" version="{MAJOR}.{MINOR}.{PATCH}.{BUILD}"/>
</assembly>"""

manifest_file = open(MANIFEST_FILE, "w")
manifest_file.write(MANIFEST)
manifest_file.close()
