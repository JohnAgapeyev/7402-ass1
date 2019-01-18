#!/bin/python3

from ascii_graph import Pyasciigraph
import sys

data = []

for arg in sys.argv[1:]:
    data.append((arg[0], float(arg[1:]) * 100))

graph = Pyasciigraph(
    float_format='{0:,.4f}'
)

for line in  graph.graph('Letter Frequency', data):
    print(line)
