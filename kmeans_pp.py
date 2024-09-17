import math
import sys
import numpy as np
import pandas as pd
from ctypes import *
import mykmeanssp

def d(p, q):
    d = np.sum((p-q)**2)
    return np.sqrt(d)

def k_mean(K, iter, eps, input_data1, input_data2):
    K =int(K)
    iter = int(iter)
    f1 = pd.read_csv(input_data1, header=None).sort_values(by=0)
    f2 = pd.read_csv(input_data2, header=None).sort_values(by=0)
    f_res = f1.set_index(0).join(f2.set_index(0), lsuffix='_1', rsuffix='_2')
    data = f_res.to_numpy()

    # n = len(data)
    # vec_size = len(data[0])  # if n!=0
    n , vec_size = data.shape
    
    if K >= n or K <= 1:
        print("Invalid number of clusters!")
        sys.exit(1)
    
    if iter >= 1000 or iter <= 1:
        print("Invalid maximum iteration!")
        sys.exit(1)
    
    id_centroids = np.zeros(K)

    centroids = np.zeros((K, vec_size))
    np.random.seed(1234)
    random_index_choice = np.random.choice(n)
    id_centroids[0] = random_index_choice
    centroids[0] = np.copy(data[random_index_choice])
    for centers in range(1,K):
        distances = np.array([min([np.linalg.norm(np.array(x) - np.array(c)) for c in centroids[0:centers]]) for x in data])
        p = distances/np.sum(distances)
        random_index_choice = np.random.choice(n, p=p)
        centroids[centers] = np.copy(data[random_index_choice])
        id_centroids[centers] = random_index_choice
    distances = np.array([min([np.linalg.norm(np.array(x) - np.array(c)) for c in centroids]) for x in data])
    s =""
    for i in range(K):
        s += str(int(id_centroids[i])) + ","
    s= s[:-1]
    print(s)
    res = mykmeanssp.fit(K, iter, n, vec_size, eps, data.tolist(), centroids.tolist())
    for vector in res:
        for i in range(vec_size):
            vector[i] = str('%.4f' % vector[i])
    
    for vector in res:
        result =""
        for x in vector:
            result += x+','
        result =result[:-1]
        print(result.replace(" ",""))


if __name__ == '__main__': 
        if len(sys.argv) == 5:
            K = float(sys.argv[1])
            if K != math.floor(K):
                print("Invalid number of clusters!")
                sys.exit(1)
            eps = float(sys.argv[2])
            if eps < 0:
                print("Invalid epsilon!")
                sys.exit(1)
            iter = 300
            input_file1 =sys.argv[3]
            input_file2 =sys.argv[4]
        elif len(sys.argv) != 6:
            print("An Error Has Occurred")
            sys.exit(1)
        else:
            K = float(sys.argv[1])
            if K != math.floor(K):
                print("Invalid number of clusters!")
                sys.exit(1)
            iter = float(sys.argv[2])
            if iter != math.floor(iter):
                print("Invalid maximum iteration!")
                sys.exit(1)
            eps = float(sys.argv[3])
            if eps < 0:
                print("Invalid epsilon!")
                sys.exit(1)
            input_file1 = sys.argv[4]
            input_file2 = sys.argv[5]
        
        k_mean(K, iter,eps, input_file1, input_file2)
        

