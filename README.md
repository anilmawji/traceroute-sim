# Traceroute Simulation

Program for finding the shortest path between two nodes, meant to imitate traceroute. Uses Dijkstra’s shortest path algorithm.

The input file should be a text file describing a graph, where each line represents a single edge given by specifying the name of the first point followed by the name of the second point and the distance between them. See input.txt for an example.

Compile with `g++ paths.cpp -o paths` and run with `./paths input.txt`.
