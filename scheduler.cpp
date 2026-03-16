#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct Process {
    int id;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
};

int main() {
    int num;
    cout << "Enter number of processes: ";
    cin >> num;

    vector<Process> p(num);
    vector<string> gantt;

    for (int i = 0; i < num; i++) {
        cout << "\nProcess " << i + 1 << endl;
        cout << "Arrival Time: ";
        cin >> p[i].arrival;

        cout << "Burst Time: ";
        cin >> p[i].burst;

        cout << "Priority: ";
        cin >> p[i].priority;

        p[i].id = i + 1;
        p[i].remaining = p[i].burst;
        p[i].completion = 0;
        p[i].turnaround = 0;
        p[i].waiting = 0;
    }

    int time = 0, completed = 0;

    cout << "\n===== Simulation Timeline =====\n";

    while (completed < num) {
        int index = -1;
        int bestPriority = 9999;

        for (int i = 0; i < num; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                if (p[i].priority < bestPriority) {
                    bestPriority = p[i].priority;
                    index = i;
                }
            }
        }

        if (index != -1) {
            cout << "Time " << time << " -> CPU running P" << p[index].id << endl;

            cout << "Ready Queue: ";
            bool hasReady = false;
            for (int i = 0; i < num; i++) {
                if (i != index && p[i].arrival <= time && p[i].remaining > 0) {
                    cout << "P" << p[i].id << " ";
                    hasReady = true;
                }
            }
            if (!hasReady) cout << "Empty";
            cout << endl;

            gantt.push_back("P" + to_string(p[index].id));

            p[index].remaining--;

            if (p[index].remaining == 0) {
                p[index].completion = time + 1;
                p[index].turnaround = p[index].completion - p[index].arrival;
                p[index].waiting = p[index].turnaround - p[index].burst;
                completed++;
            }
        } else {
            cout << "Time " << time << " -> CPU Idle" << endl;
            cout << "Ready Queue: Empty" << endl;
            gantt.push_back("Idle");
        }

        time++;
    }

    cout << "\n===== Gantt Chart =====\n";
    for (int i = 0; i < (int)gantt.size(); i++) {
        cout << "| " << gantt[i] << " ";
    }
    cout << "|\n";

    for (int i = 0; i <= (int)gantt.size(); i++) {
        cout << setw(5) << i;
    }
    cout << "\n";

    cout << "\n===== Process Table =====\n";
    cout << left << setw(10) << "Process"
         << setw(10) << "AT"
         << setw(10) << "BT"
         << setw(10) << "Prio"
         << setw(10) << "CT"
         << setw(10) << "TAT"
         << setw(10) << "WT" << endl;

    double totalTAT = 0, totalWT = 0;

    for (int i = 0; i < num; i++) {
        cout << left << setw(10) << ("P" + to_string(p[i].id))
             << setw(10) << p[i].arrival
             << setw(10) << p[i].burst
             << setw(10) << p[i].priority
             << setw(10) << p[i].completion
             << setw(10) << p[i].turnaround
             << setw(10) << p[i].waiting << endl;

        totalTAT += p[i].turnaround;
        totalWT += p[i].waiting;
    }

    cout << "\nAverage Turnaround Time = " << totalTAT / num << endl;
    cout << "Average Waiting Time    = " << totalWT / num << endl;

    return 0;
}
