# Python Random Polygon Generator

This package provides Python bindings for the [Random Polygon Generator (RPG)](https://github.com/cgalab/genpoly-rpg) library, developed by the Computational Geometry and Applications Lab at the University of Salzburg. The RPG library implements various algorithms for generating simple (non-self-intersecting) polygons from a given set of points.

## Features

- Generate simple polygons using various algorithms including:
  - 2-opt based methods (2opt, 2opt_ii, 2opt_iii)
  - Space partitioning (space_part)
  - Growing algorithms (growing, growing_ii)
  - Star-shaped polygons (star)
- Support for both random and clustered point distributions
- NumPy integration for efficient data handling

## Installation

### Prerequisites

- Python 3.6 or higher
- CMake 2.8.12 or higher
- C compiler (gcc recommended)
- NumPy
- Git (for cloning with submodules)

### Installation Steps

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/yourusername/python-rpg.git
cd python-rpg
```

2. Install using pip:
```bash
pip install .
```

## Usage

### Basic Example

```python
import genpoly_rpg
import numpy as np

# Generate a single polygon with 10 vertices
polygon = genpoly_rpg.generate_polygons(vertices=10)
# Result shape: (1, 11, 2) - One polygon, 11 points (first point repeated), 2 coordinates (x,y)

# Plot the polygon
import matplotlib.pyplot as plt
plt.plot(polygon[0,:,0], polygon[0,:,1], '-o')
plt.axis('equal')
plt.show()
```

### Advanced Usage

```python
# Generate multiple polygons with custom settings
polygons = genpoly_rpg.generate_polygons(
    vertices=20,          # Number of vertices per polygon
    num_polygons=5,       # Number of polygons to generate
    seed=42,             # Random seed for reproducibility
    cluster=True,        # Use clustered point distribution
    algorithm='2opt_ii'  # Choose generation algorithm
)

# Available algorithms:
# - '2opt'        (Default 2-opt based algorithm)
# - '2opt_ii'     (Alternative 2-opt implementation)
# - '2opt_iii'    (Another 2-opt variant)
# - 'space'       (Space partitioning algorithm)
# - 'growth'      (Growing algorithm)
# - 'growth_ii'   (Alternative growing implementation)
# - 'star'        (Star-shaped polygon generation)
# - 'fast_star'   (Fast star-shaped polygon generation)
# - 'xmono'       (xmonotone polygon generation)
```

## Output Format

The output is a NumPy array with shape `(num_polygons, vertices+1, 2)` where:
- First dimension: Number of generated polygons
- Second dimension: Number of vertices + 1 (first vertex repeated at end to close polygon)
- Third dimension: X and Y coordinates

## Citation

If you use this software, please cite the following publication that describes the original algorithms:

   Thomas Auer and Martin Held.
   Heuristics for the generation of random polygons.
   Proc. 8th Canadian Conference on Computational Geometry (CCCG'96), pages 38-43, 1996.
   http://www.cccg.ca/proceedings/1996/cccg1996_0007.pdf

## Copyright/License

The original RPG library:
Copyright T. Auer, M. Gschwandtner, M. Heimlich, M. Held 1994-2020

The library includes an old version of qhull for computing convex hulls:
Copyright C.B. Barber

Please see the original repository's COPYING.txt and COPYING_Qhull.txt for detailed license information.