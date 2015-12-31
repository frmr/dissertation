#include "uts.h"

#include <stdio.h>
#include <stdint.h>

#include <omp.h>

#include "../timer.h"

extern int nworkers;

int64_t sumArray(int64_t* values, int size) {
    int64_t sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += values[i];
    }
    return sum;
}

static int64_t genChildren(Node * parent, const Node child);

static int64_t spawnChildNode(Node * parent, const Node child, const int i) {
    int parentHeight = parent->height;
    int childType = uts_childType(parent);
    Node newChild = child;
    newChild.type = childType;
    newChild.height = parentHeight + 1;
    for (int j = 0; j < getComputeGran(); j++) {
        rng_spawn(parent->state.state, newChild.state.state, i);
    }

    return genChildren(&newChild, child);
}

static int64_t genChildren(Node * parent, const Node child) {
    // record number of children in parent
    const int numChildren = uts_numChildren(parent);
    parent->numChildren = numChildren;

    int64_t spawnedNodes = 0;
    int64_t* spawnedNodesArray = malloc(sizeof(int64_t)*numChildren);
    // construct children and push in our data structure
    if (numChildren > 0) {
        for (int i = 0; i < numChildren; i++) {
            #pragma omp task shared(parent, child)
            spawnedNodesArray[i] = spawnChildNode(parent, child, i);
        }
        #pragma omp taskwait
    }
    int64_t sum = sumArray(spawnedNodesArray, numChildren);
    free(spawnedNodesArray);
    return sum + 1;
}

int main(int argc, char *argv[]) {
    // parse parameter
    uts_parseParams(argc, argv);

    // show parameter
    uts_printParams();

    omp_set_num_threads(nworkers);

    // init the root node of our work-tree
    Node root;
    uts_initRoot(&root);

    Node child;
    initNode(&child);

    // start our benchmark
    int64_t spawnedNodes;
    timer t;

    startTimer(&t);
    #pragma omp parallel shared(root, child)
    {
        #pragma omp single nowait
        {
            spawnedNodes = genChildren(&root, child);
        }
    }
    stopTimer(&t);

    printf("%d,%d,%d,%lf\n", getComputeGran(), nworkers, spawnedNodes, t.ms);

    return 0;
}
