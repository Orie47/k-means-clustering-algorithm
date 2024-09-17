# k-means-clustering-algorithm
This repository contains the implementation of the K-means++ algorithm and its integration with an existing K-means algorithm written in C. The project aims to optimize the centroid initialization for K-means clustering using K-means++ in Python, while utilizing a C extension to perform the actual clustering process.

Features
K-means++ Initialization: Implemented in Python to provide better initial centroids for the K-means algorithm, which helps avoid poor clustering results.
C Extension for K-means: The K-means algorithm is implemented in C and linked to Python via a C extension for faster computation.
NumPy and Pandas Integration: Used for efficient numerical computation and data manipulation.
Custom Python Interface: A command-line interface allows users to specify the number of clusters, maximum iterations, epsilon for convergence, and input data files.
Files
kmeans_pp.py: Implements the K-means++ initialization and handles user input, data preparation, and integration with the C extension.
kmeansmodule.c: A C extension that runs the K-means algorithm, excluding the initialization step (which is handled by K-means++).
setup.py: Builds the C extension and allows for integration with the Python code.
