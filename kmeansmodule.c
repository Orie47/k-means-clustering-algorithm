kmeans#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double distance(double *p, double *q, int length) {
    double d = 0;
    int i;
    for (i = 0; i < length; i++) {
        d += (p[i] - q[i]) * (p[i] - q[i]);
    }
    return sqrt(d);
}


double **k_mean_c(int K, int iter, double epsilon, double **data, double **centroids, int n, int vec_size) {
    int i, j, count, k, argdist, change;
    double dist, temp;
    double **clusters = NULL;
    int *vec_in_cluster = NULL;

    for (count = 0; count < iter; count++) {
        clusters = calloc(K, sizeof(double *));
        if (clusters == NULL) {
            printf("An Error Has Occurred");
            for (i = 0; i < K; i++) {
                free(centroids[i]);
            }
            free(centroids);
            for (i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
            return NULL;
        }
        vec_in_cluster = calloc(K, sizeof(int));
        if (vec_in_cluster == NULL) {
            printf("An Error Has Occurred");
            for (i = 0; i < K; i++) {
                free(clusters[i]);
            }
            free(clusters);
            for (i = 0; i < K; i++) {
                free(centroids[i]);
            }
            free(centroids);
            for (i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
            return NULL;
        }

        for (i = 0; i < K; i++) {
            clusters[i] = calloc(vec_size, sizeof(double));
            if (clusters[i] == NULL) {
                printf("An Error Has Occurred");
                for (j = 0; j <= i; j++) {
                    free(clusters[j]);
                }
                free(clusters);
                free(vec_in_cluster);
                for (i = 0; i < K; i++) {
                    free(centroids[i]);
                }
                free(centroids);
                for (i = 0; i < n; i++) {
                    free(data[i]);
                }
                free(data);
                return NULL;
            }
        }

        for (i = 0; i < n; i++) {
            dist = -1;
            argdist = 0;
            for (k = 0; k < K; k++) {
                temp = distance(data[i], centroids[k], vec_size);
                if (dist == -1 || temp < dist) {
                    dist = temp;
                    argdist = k;
                }
            }
            for (j = 0; j < vec_size; j++) {
                clusters[argdist][j] += data[i][j];
            }
            vec_in_cluster[argdist]++;
        }

        for (i = 0; i < K; i++) {
            for (j = 0; j < vec_size; j++) {
                clusters[i][j] /= vec_in_cluster[i];
            }
        }

        change = 0;
        for (k = 0; k < K; k++) {
            if (distance(centroids[k], clusters[k], vec_size) >= epsilon) {
                change = 1;
            }
        }

        for (i = 0; i < K; i++) {
            free(centroids[i]);
            centroids[i] = clusters[i];
        }

        free(clusters);
        free(vec_in_cluster);

        if (!change) {
            break;
        }
    }
    for (i = 0; i < n; i++) {
        free(data[i]);
    }
    free(data);
    return centroids;
}



static PyObject *fit(PyObject *self, PyObject *args) {
    int K, iter, n, vec_size, i, j;
    double eps;
    PyObject *py_data = NULL, *py_centroids = NULL, *py_res=NULL;
    double **data = NULL, **centroids = NULL , **res = NULL;
    
    if (!PyArg_ParseTuple(args, "iiiidOO", &K, &iter, &n, &vec_size, &eps, &py_data, &py_centroids)) {
        return NULL; 

    }
    data = (double**) malloc(n* sizeof(double *));
    if (data == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }
    for (i = 0; i < n; i++) {
        data[i] = (double*) malloc(vec_size* sizeof(double));
        if (data[i] == NULL) {
            printf("An Error Has Occurred");
            for (i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
            return NULL;
        }
    }
    centroids = (double**) malloc(K * sizeof(double *));
    if (centroids == NULL) {
        printf("An Error Has Occurred");
        for (i = 0; i < n; i++) {
            free(data[i]);
        }
        free(data);
        return NULL;
    }
    for (i = 0; i < K; i++) {
        centroids[i] = (double*) malloc(vec_size* sizeof(double));
        if (centroids[i] == NULL) {
            printf("An Error Has Occurred");
            for (i = 0; i < K; i++) {
                free(centroids[i]);
            }
            free(centroids);
            for (i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
            return NULL;
        }
    }
    if (py_data == NULL || py_centroids == NULL) {
        printf("An Error Has Occurred");
        for (i = 0; i < K; i++) {
            free(centroids[i]);
        }
        free(centroids);
        for (i = 0; i < n; i++) {
            free(data[i]);
        }
        free(data);
        return NULL;
    }
    
    for (i = 0; i < n; i++) {
        PyObject *py_data_i = PyList_GetItem(py_data, i);
        if (py_data_i == NULL) {
            printf("An Error Has Occurred\n");
            for (i = 0; i < K; i++) {
            free(centroids[i]);
            }
            free(centroids);
            for (i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
            return NULL;
        }
        else {
            for (j = 0; j < vec_size; j++) {
            data[i][j] = PyFloat_AsDouble(PyList_GetItem(py_data_i,j));
        }
        }
    }
    for (i = 0; i < K; i++) {
        PyObject *py_centroid_i = PyList_GetItem(py_centroids, i);
        if (py_centroid_i == NULL) {
            printf("An Error Has Occurred\n");
            for (i = 0; i < K; i++) {
            free(centroids[i]);
            }
            free(centroids);
            for (i = 0; i < n; i++) {
                free(data[i]);
            }
            free(data);
            return NULL;
        }
        else {
        for (j = 0; j < vec_size; j++) {
            centroids[i][j] = PyFloat_AsDouble(PyList_GetItem(py_centroid_i, j));
        }}
    }
    res = k_mean_c(K, iter, eps, data, centroids, n, vec_size);
    py_res = PyList_New((Py_ssize_t)K);
    if (py_res == NULL) {
        printf("An Error Has Occurred\n");
        for (i = 0; i < K; i++) {
            free(centroids[i]);
        }
        free(centroids);
        return NULL;
    }
    for (i = 0; i < K; i++) {
        PyObject *py_res_i = PyList_New(vec_size);
        if (py_res_i == NULL) {
            printf("An Error Has Occurred\n");
            for (i = 0; i < K; i++) {
                free(centroids[i]);
            }
            free(centroids);
            return NULL;
        }
        for (j = 0; j < vec_size; j++) {
            PyList_SetItem(py_res_i, j, PyFloat_FromDouble(res[i][j]));
        }
        free(centroids[i]);
        PyList_SetItem(py_res, i, py_res_i);
    }
    free(centroids);
    return py_res;
}

static PyMethodDef mykmeanssp_methods[] = {
    {"fit",(PyCFunction) fit, METH_VARARGS,PyDoc_STR("K-means algorithm_plus_plus")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mykmeansspmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    NULL,
    -1,
    mykmeanssp_methods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    PyObject *m;
    m = PyModule_Create(&mykmeansspmodule);
    if (!m) {
        printf("An Error Has Occurred\n");
        return NULL;
    }
    return m;
}


