from setuptools import setup, Extension

module = Extension('mykmeanssp', sources=['kmeansmodule.c'])
setup(name='mykmeanssp', version='1.0',description='python wrapper for kmeans module', ext_modules=[module])
