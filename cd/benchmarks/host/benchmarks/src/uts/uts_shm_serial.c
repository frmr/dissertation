#include "uts.h"

#include <stdio.h>
#include <stdint.h>

#include "../timer.h"

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
    if (numChildren > 0) {
        for (int i = 0; i < numChildren; i++) {
            spawnedNodes += spawnChildNode(parent, child, i);
        }
    }
    return spawnedNodes + 1;
}

int main(int argc, char *argv[]) {
    // parse parameter
    uts_parseParams(argc, argv);

    // show parameter
    uts_printParams();

    // init the root node of our work-tree
    Node root;
    uts_initRoot(&root);

    Node child;
    initNode(&child);

    // start our benchmark
    int64_t spawnedNodes;
    timer t;

    startTimer(&t);
    spawnedNodes = genChildren(&root, child);
    stopTimer(&t);

    printf("%d,%d,%d,%lf\n", getComputeGran(), 1, spawnedNodes, t.ms);

    return 0;
}
