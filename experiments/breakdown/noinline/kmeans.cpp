#include "../kmeans_util.h"

__attribute__((noinline)) void distance(float *data, float *cent, float *dist) {
    for (int i = 0; i < CENT; i++) {
        int centOff = i * ATTR;
        for (int j = 0; j < ATTR; j++) {
            float d = cent[centOff + j] - data[j];
            dist[i] += d * d;
        }
        dist[i] = sqrtf(dist[i]);
    }
}

__attribute__((noinline)) void minimum(float *dist, int *assign) {
    *assign = 0;
    float min = dist[0];
    for (int i = 1; i < CENT; i++)
        if (dist[i] < min) {
            min = dist[i];
            *assign = i;
        }
}

__attribute__((noinline)) void reassign(float *data, float *temp) {
    for (int i = 0; i < ATTR; i++)
        temp[i] += data[i];
    temp[ATTR]++;
}

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *data = new float[DATA * ATTR];
    float *cent = new float[CENT * ATTR];
    float *temp = new float[CENT * (ATTR + 1)];
    init(file, data, cent, temp);

    test(temp);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA * ATTR; i += ATTR) {
        float dist[CENT] = {0.0f};
        distance(data + i, cent, dist);

        int assign;
        minimum(dist, &assign);

        float *tempOff = temp + assign * (ATTR + 1);
        reassign(data + i, tempOff);
    }
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("kmeans", start, stop);

    delete[] data;
    delete[] cent;
    delete[] temp;

    return 0;
}
