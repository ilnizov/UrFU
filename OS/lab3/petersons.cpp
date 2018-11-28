#include "petersons.h"

int n;
int *last;
int *stage;

void enter_region(int process) {
    for (int i = 1; i <= n - 1; i++) {
        stage[process] = i;
        last[i] = process;

        for (int j = 0; j < n; j++) {
            if (j == process)
                continue;

            while (stage[j] >= i && last[i] == process);
        }
    }
}

void leave_region(int process) {
    stage[process] = 0;
}