#include "../kmeans_util.h"

int main(int argc, char *argv[]) {
    struct timeval start, stop;
    string file = argv[1];

    float *data = new float[DATA * ATTR];
    float *cent = new float[CENT * ATTR];
    float *temp = new float[CENT * (ATTR + 1)];
    init(file, data, cent, temp);
    int *assign = new int[TILE];

    test(temp);
    gettimeofday(&start, NULL);
    for (int i = 0; i < DATA / TILE; i++) {
        for (int j = 0; j < TILE; j++) {
            int dataOff = i * TILE * ATTR + j * ATTR;

            //distance
            float dist[CENT] = {0.0f};
            for (int k = 0; k < CENT; k++) {
                int centOff = k * ATTR;
                for (int l = 0; l < ATTR; l++) {
                    float d = cent[centOff + l] - data[dataOff + l];
                    dist[k] += d * d;
                }
                dist[k] = sqrtf(dist[k]);
            }

            //minimum
            assign[j] = 0;
            float min = dist[0];
            for (int k = 1; k < CENT; k++)
                if (dist[k] < min) {
                    min = dist[k];
                    assign[j] = k;
                }
        }

        for (int j = 0; j < TILE; j++) {
            int dataOff = i * TILE * ATTR + j * ATTR;

            //reassign
            int tempOff = assign[j] * (ATTR + 1);
            for (int k = 0; k < ATTR; k++)
                temp[tempOff + k] += data[dataOff + k];
            temp[tempOff + ATTR]++;
        }
    }
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("kmeans", start, stop);

    delete[] data;
    delete[] cent;
    delete[] temp;
    delete[] assign;

    return 0;
}
