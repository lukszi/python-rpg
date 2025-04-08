import os
import sys
import ctypes
from pathlib import Path
from typing import Optional
import numpy as np
from numpy.typing import NDArray

# Load the shared library first
package_dir = Path(__file__).parent.absolute()
lib_path = package_dir / "librpg.so"

if os.path.exists(lib_path):
    try:
        ctypes.CDLL(str(lib_path))
    except Exception as e:
        print(f"Warning: Failed to load librpg.so: {e}", file=sys.stderr)

# Import the actual function from the C extension
from ._genpoly_rpg import generate_polygons as _generate_polygons

# Create a typed wrapper around the C function
def generate_polygons(
    vertices: int, 
    num_polygons: int = 1, 
    seed: Optional[int] = -1, 
    cluster: bool = False, 
    algorithm: str = "2opt"
) -> NDArray[np.float64]:
    """
    Generate random simple polygons.
    
    Args:
        vertices: Number of vertices per polygon
        num_polygons: Number of polygons to generate
        seed: Random seed (-1 for random)
        cluster: Use clustered point distribution if True
        algorithm: Algorithm to use. Options include:
                  "2opt", "2opt_ii", "2opt_iii", "space", "growth",
                  "growth_ii", "star", "fast_star", "xmono"
                  
    Returns:
        NumPy array with shape (num_polygons, vertices+1, 2) containing polygon coordinates
    """
    return _generate_polygons(vertices, num_polygons, seed, cluster, algorithm)

__all__ = ["generate_polygons"]
__version__ = "1.0.0"
