#!/bin/bash

# A script to create trace files for all sorts of memory
# march tests, using the light_march script.

# SAF:  Stuck at Fault
#       -> The logic value of a cell or a line is always 0 or 1.
# TF:   Transition Fault
#       -> A cell or a line that fails to undergo a 0->1 or a 1->0 transition.
# CF:   Coupling Fault
#       -> A write operation to one cell changes the content of a second cell.
# NPSF: Neighborhood Pattern Sensitive Fault
#       -> The content of a cell, or the ability to change its content, is 
#           influenced by the contents of some other cells in the memory.
# AF:   Address Decoding Fault
#       -> Any of the following:
#          + With a certain address, no cell is accessed.
#          + A certain cell is never accessed.
#          + With a certain address, multiple cells are accessed simultaneously.
#          + A certain cell can be accessed by multiple addresses.

gcc -o light_march light_march.c

# MATS Algorithm
# Test length: 4N
# Fault coverage: Some AFs, SAFs
./light_march mats.trace aw,arw,ar

# MATS+ Algorithm
# Test length: 5N
# Fault coverage: AFs, SAFs
./light_march matsp.trace aw,urw,arw

# Marching 1/0 Algorithm
# Test length: 14N
# Fault coverage: AFs, SAFs, TFs
./light_march marching10.trace aw,urwr,drwr,aw,urwr,drwr

# MATS++ Algorithm
# Test length: 6N
# Fault coverage: AFs, SAFs, TFs
./light_march matspp.trace aw,urw,arwr

# March X Algorithm
# Test length: 6N
# Fault coverage: AFs, SAFs, TFs, Some CFs
./light_march marchx.trace aw,urw,drw,ar

# March C Algorithm
# Test length: 10N
# Fault coverage: AFs, SAFs, TFs, Some CFs
./light_march marchc.trace aw,urw,urw,ar,drw,drw,ar

# March A Algorithm
# Test length: 15N
# Fault coverage: AFs, SAFs, TFs, Some CFs
./light_march marcha.trace aw,urwww,urww,drwww,drww

# March Y Algorithm
# Test length: 8N
# Fault coverage: AFs, SAFs, TFs, Some CFs
./light_march marchy.trace aw,urwr,drwr,ar

# March B Algorithm
# Test length: 17N
# Fault coverage: AFs, SAFs, TFs, Some CFs
./light_march marchb.trace aw,urwrwrw,urww,drwww,drww


