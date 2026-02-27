#include <iostream>
using namespace std;

const int MAXP = 50;

// Input arrays
int pidArr[MAXP], arrivalArr[MAXP], burstArr[MAXP], priorityArr[MAXP];

// Result arrays
int waitArr[MAXP], tatArr[MAXP], remArr[MAXP];

int n; // number of processes

// Utility: compute & show average metrics
void showResults(const char *title) {
    cout << "\n=========== " << title << " ===========\n";
    cout << "PID\tAT\tBT\tPR\tWT\tTAT\n";
    float sumWT=0, sumTAT=0;
    for (int i = 0; i < n; i++) {
        cout << pidArr[i] << "\t" << arrivalArr[i] << "\t"
             << burstArr[i] << "\t" << priorityArr[i] << "\t"
             << waitArr[i] << "\t" << tatArr[i] << "\n";
        sumWT += waitArr[i];
        sumTAT += tatArr[i];
    }
    cout << "Avg WT: " << sumWT/n << "   Avg TAT: " << sumTAT/n << "\n";
}

// FCFS Scheduling
void scheduleFCFS() {
    int time = 0;
    // Copy bursts into remaining to schedule
    for (int i = 0; i < n; i++) {
        remArr[i] = burstArr[i];
    }
    for (int i = 0; i < n; i++) {
        if (arrivalArr[i] > time) time = arrivalArr[i];
        waitArr[i] = time - arrivalArr[i];
        tatArr[i] = waitArr[i] + burstArr[i];
        time += burstArr[i];
    }
    showResults("FCFS");
}

// SJF (Non-Preemptive)
void scheduleSJF() {
    // Create local copies
    int order[MAXP], done[MAXP] = {0};
    for (int i = 0; i < n; i++) order[i] = i;

    int time = 0, completed = 0;
    while (completed < n) {
        int idx = -1, mn = 99999;
        for (int i = 0; i < n; i++) {
            if (!done[i] && arrivalArr[i] <= time && burstArr[i] < mn) {
                mn = burstArr[i];
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        waitArr[idx] = time - arrivalArr[idx];
        time += burstArr[idx];
        tatArr[idx] = waitArr[idx] + burstArr[idx];
        done[idx] = 1;
        completed++;
    }
    showResults("SJF (Non-Preemptive)");
}

// Priority Scheduling (Non-Preemptive)
void schedulePriority() {
    int done[MAXP] = {0};
    int time = 0, completed = 0;

    while (completed < n) {
        int idx = -1, bestPri = 99999;
        for (int i = 0; i < n; i++) {
            if (!done[i] && arrivalArr[i] <= time && priorityArr[i] < bestPri) {
                bestPri = priorityArr[i];
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        waitArr[idx] = time - arrivalArr[idx];
        time += burstArr[idx];
        tatArr[idx] = waitArr[idx] + burstArr[idx];
        done[idx] = 1;
        completed++;
    }
    showResults("Priority (Non-Preemptive)");
}

// Round Robin
void scheduleRR(int quantum) {
    for (int i = 0; i < n; i++) remArr[i] = burstArr[i];
    int time = 0, doneCount = 0;

    while (doneCount < n) {
        bool didRun = false;
        for (int i = 0; i < n; i++) {
            if (arrivalArr[i] <= time && remArr[i] > 0) {
                didRun = true;
                if (remArr[i] > quantum) {
                    remArr[i] -= quantum;
                    time += quantum;
                } else {
                    time += remArr[i];
                    waitArr[i] = time - arrivalArr[i] - burstArr[i];
                    tatArr[i] = time - arrivalArr[i];
                    remArr[i] = 0;
                    doneCount++;
                }
            }
        }
        // If nothing ran (waiting for arrivals), jump time forward
        if (!didRun) time++;
    }
    showResults("Round Robin");
}

int main() {
    cout << "Number of processes: ";
    cin >> n;
    cout << "Enter PID, Arrival Time, Burst Time, Priority:\n";
    for (int i = 0; i < n; i++) {
        cin >> pidArr[i] >> arrivalArr[i] >> burstArr[i] >> priorityArr[i];
        waitArr[i] = tatArr[i] = 0;
    }

    scheduleFCFS();        // First-Come First-Serve :contentReference[oaicite:1]{index=1}
    scheduleSJF();         // Shortest Job First (non-preemptive) :contentReference[oaicite:2]{index=2}
    schedulePriority();    // Priority Scheduling :contentReference[oaicite:3]{index=3}
    scheduleRR(4);         // Round Robin with quantum = 4 :contentReference[oaicite:4]{index=4}

    return 0;
}
