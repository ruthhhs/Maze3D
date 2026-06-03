// ========================================================
// SPIKE
// ========================================================
#ifndef SPIKE_H
#define SPIKE_H

#define MAX_SPIKES 100

typedef struct {

    float x, z;

} Spike;

extern Spike spikes[MAX_SPIKES];
extern int spikeCount;

void drawSpike(
    float x,
    float z
);

void resetSpikes();

int checkSpikeCollision(
    float px,
    float py,
    float pz
);

#endif
