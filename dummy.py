#!/usr/bin/env python3
from sys import version
from time import ctime
print("It is now %s. This is Python %s." % (ctime(), version.replace('\n', '')))
