#pragma once

#define QUEUE_SZ 300

int queue[2 * QUEUE_SZ];
int queue_l, queue_r;

void queue_clear() {
    queue_l = QUEUE_SZ;
    queue_r = QUEUE_SZ - 1;
}

bool queue_empty() {
    return queue_l > queue_r;
}

int queue_pop() {
    assert(!queue_empty());
    return queue[queue_l++];
}

void queue_pf(int x) {
    assert(queue_l > 0);
    queue[--queue_l] = x;
}

void queue_pb(int x) {
    assert(queue_r < 2 * QUEUE_SZ - 1);
    queue[++queue_r] = x;
}