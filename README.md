# Finding the Vertex Cover of a Randomly Generated Graph

## Overview

This project is designed and implemented to find the vertex cover of a randomly generated graph using the MiniSat SAT solver. For a brief understanding of the vertex cover concept, refer to [this link](https://en.wikipedia.org/wiki/Vertex_cover#:~:text=In%20graph%20theory%2C%20a%20vertex,every%20edge%20of%20the%20graph).

In this project, a part of the [graph_shortest_path](https://github.com/SajjiKazemi/graph_shortest_path) will be incorporated to solve the minimal Vertex
Cover the problem for the input graph. The approach is based on a polynomial-time reduction to CNF-SAT and the use of an SAT solver.

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/SajjiKazemi/vertex_cover
```

## Dependencies
Create a virtual environment using the provided my_env.yml file:

```bash
conda env create -f my_env.yml
```
Activate the environment:

```bash
conda activate my_env
```

## Usage

To run the project, inside the project directory, create the necessary directory:

```bash
mkdir build
```

and execute the following command line:

```bash
cmake ../
```

and then:

```bash
make
```

Run the executable file for main.cpp:


```bash
 ./main
```

Upon providing an input graph via the ’V’ and ’E’ specifications to the code, the minimum-sized Vertex Cover will be computed and immediately output. The output will be a sequence of vertices in increasing order separated by one space each.

Assuming your executable is called main, the following is a sample run of your program:

```bash
$ ./main

V 5

E {<1,5>,<5,2>,<1,4>,<4,5>,<4,3>,<2,4>}

4 5
```

## Contact
For any questions or feedback, feel free to reach out:

Email: sajjikazemi@uwaterloo.ca

LinkedIn: www.linkedin.com/in/sajjadkazemi
