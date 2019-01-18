#!/bin/python3

from ascii_graph import Pyasciigraph
import sys

data = []

for arg in sys.argv[1:]:
    data.append((arg[0], int(arg[1:])))

graph = Pyasciigraph()
for line in  graph.graph('Letter Frequency', data):
    print(line)
