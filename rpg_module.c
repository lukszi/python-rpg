// rpg_module.c
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "pointOps.h"
#include "polyOps.h"
#include "calcPoly.h"
#include "basicInits.h"
#include "eralloc.h"

static PyObject* generate_polygons(PyObject* self, PyObject* args, PyObject* kwargs) {
    int vertices;
    int num_polygons = 1;
    int seed = -1;
    int cluster = 0;
    char* algorithm = "2opt";
    
    static char* kwlist[] = {"vertices", "num_polygons", "seed", "cluster", "algorithm", NULL};
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i|iiis", kwlist,
                                    &vertices, &num_polygons, &seed, &cluster, &algorithm)) {
        return NULL;
    }
    
    // Initialize RPG subsystems
    BIinit();
    erinit();
    POinit();
    YOinit();
    
    // Set random seed
    if (seed >= 0) {
        srand48(seed);
    }
    else {
        srand48(time(NULL));
    }
    
    // Create NumPy array to store results
    npy_intp dims[3] = {num_polygons, vertices + 1, 2}; // +1 because we repeat first vertex at end
    PyObject* arr = PyArray_SimpleNew(3, dims, NPY_DOUBLE);
    if (!arr) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to create output array");
        return NULL;
    }
    double* data = (double*)PyArray_DATA((PyArrayObject*)arr);
    
    // For each requested polygon
    for (int p = 0; p < num_polygons; p++) {
        // Clear any previous points/polygons
        POclearPoints();
        YOclearPoly();
        
        // Generate points
        if (cluster) {
            POgenerateClusterPoints(vertices);
        } else {
            POgenerateRandomPoints(vertices);
        }
        
        // Set algorithm
        enum t_calcType algo = CP_2OPT_II;
        if (strcmp(algorithm, "2opt") == 0) algo = CP_2OPT;
        else if (strcmp(algorithm, "2opt_ii") == 0) algo = CP_2OPT_II;
        else if (strcmp(algorithm, "2opt_iii") == 0) algo = CP_2OPT_III;
        else if (strcmp(algorithm, "space") == 0) algo = CP_SPACE_PART;
        else if (strcmp(algorithm, "growth") == 0) algo = CP_GROWING;
        else if (strcmp(algorithm, "growth_ii") == 0) algo = CP_GROWING_II;
        else if (strcmp(algorithm, "star") == 0) algo = CP_STAR;
        else if (strcmp(algorithm, "fast_star") == 0) algo = CP_FAST_STAR;
        else if (strcmp(algorithm, "xmono") == 0) algo = CP_XMONO;
        
        // Generate polygon
        CPsetState(algo, RPG_UNDEFINED);
        CPdoCalc();
        
        // Get polygon data
        t_polygon* polygon;
        t_pointArray* points;
        YOgetPoly(&polygon, &points);
        
        // Copy vertices to NumPy array
        for (int i = 0; i < vertices + 1; i++) {
            int idx = i % vertices + 1;  // Wrap around to first vertex
            t_point pt = PAgetPoint(points, BPgetPIndex(polygon, idx));
            data[(p * (vertices + 1) + i) * 2] = pt.x;
            data[(p * (vertices + 1) + i) * 2 + 1] = pt.y;
        }
    }
    
    // Cleanup
    POfree();
    YOfree();
    erend();
    
    return arr;
}

static PyMethodDef RPGMethods[] = {
    {"generate_polygons", (PyCFunction)generate_polygons, METH_VARARGS | METH_KEYWORDS,
     "Generate random simple polygons.\n"
     "Args:\n"
     "    vertices (int): Number of vertices per polygon\n"
     "    num_polygons (int, optional): Number of polygons to generate. Default 1\n"
     "    seed (int, optional): Random seed. Default -1 (random)\n"
     "    cluster (int, optional): Use clustered point distribution. Default 0\n"
     "    algorithm (str, optional): Algorithm to use. Default '2opt'\n"
     "Returns:\n"
     "    ndarray: Array of shape (num_polygons, vertices+1, 2) containing polygon coordinates"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef rpgmodule = {
    PyModuleDef_HEAD_INIT,
    "genpoly_rpg",
    "Python interface for Random Polygon Generator (RPG)",
    -1,
    RPGMethods
};

PyMODINIT_FUNC PyInit_genpoly_rpg(void) {
    import_array();  // Initialize NumPy
    return PyModule_Create(&rpgmodule);
}