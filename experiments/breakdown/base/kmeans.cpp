#include "../kmeans_util.h"

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
        //distance
        float dist[CENT] = {0.0f};
        for (int j = 0; j < CENT; j++) {
            int centOff = j * ATTR;
            for (int k = 0; k < ATTR; k++) {
                float d = cent[centOff + k] - data[i + k];
                dist[j] += d * d;
            }
            dist[j] = sqrtf(dist[j]);
        }

        //minimum
        int assign = 0;
        float min = dist[0];
        for (int j = 1; j < CENT; j++)
            if (dist[j] <= min) {
                min = dist[j];
                assign = j;
            }

        //reassign
        int tempOff = assign * (ATTR + 1);
        for (int j = 0; j < ATTR; j++)
            temp[tempOff + j] += data[i + j];
        temp[tempOff + ATTR]++;
    }
    gettimeofday(&stop, NULL);
    test(temp);
    printTime("kmeans", start, stop);

    delete[] data;
    delete[] cent;
    delete[] temp;

    return 0;
}
