#pragma once

const int queueSz = 300;

int queue[2 * queueSz];
int queueL, queueR;

void clearQueue() {
    queueL = queueSz;
    queueR = queueSz - 1;
}

bool queueEmpty() {
    return queueL > queueR;
}

int queuePop() {
    assert(!queueEmpty());
    return queue[queueL++];
}

void pushFront(int x) {
    assert(queueL > 0);
    queue[--queueL] = x;
}

void pushBack(int x) {
    assert(queueR < 2 * queueSz - 1);
    queue[++queueR] = x;
}