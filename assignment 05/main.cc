#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <climits>
#include <iomanip>


using namespace std;

///creating the structure for a process
struct process
{
    public:

    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int end_time;
    int wait_time;
    int time_left;
    int last_run_time;
    bool is_complete;

    process() : pid(0), arrival_time(0), burst_time(0),
                start_time(-1), end_time(0), wait_time(0), time_left(0),
                last_run_time(0), is_complete(false) {}

    void print_all()
    {
        cout << "pid: " << pid << ", arrival_time: " << arrival_time << ", burst_time: " << burst_time << endl;
        cout << "start_time: " << start_time << ", end time: " << end_time << ", wait time: " << wait_time << endl;
        cout << "time left: " << time_left << " last run time: " << last_run_time << ", is completed: " << is_complete << endl;
    }
    void print()
    {
        cout << setw(2) << pid << setw(13) << arrival_time << setw(13) << start_time << setw(12) << end_time <<
        setw(13) << burst_time << setw(15) << wait_time << endl;
    }
};

void fcfs(process processes[], int size)
{
    int total_time = 0;
    for (int i = 0; i < size; i++)
    {
        if (processes[i].arrival_time > total_time)///checking for if there is idel time
        {
            total_time = processes[i].arrival_time;
            cout << processes[i].pid << " is idle until " << total_time << "ms\n";
        }

        cout << "Process: " << processes[i].pid << " started at " << total_time << "ms\n";\
        ///calculating the simulation times
        processes[i].start_time = total_time;
        total_time += processes[i].burst_time;
        processes[i].wait_time = processes[i].start_time - processes[i].arrival_time;
        processes[i].end_time = total_time;
        processes[i].is_complete = true;
        cout << "Process: " << processes[i].pid << " ended at " << total_time << "ms\n";
    }
    
}
void rr(process processes[], int size, int quantum_time)
{
    int total_time = 0;
    int remaining = size;
    while (remaining > 0)
    {
        for (int i = 0; i < size; i++)
        {///don't need to do anything is something isn't in the queue yet or already completed
            if (processes[i].arrival_time > total_time) { cout << "Idle at " << total_time << "ms\n"; total_time++; }
            if (processes[i].is_complete) { continue; }

            if (processes[i].start_time == -1)
            {
                cout << "Process: " << processes[i].pid << " started at " << total_time << "ms\n";\
                processes[i].start_time = total_time;
            }///outputting the continue message and keeping track of wait time correctly
            else { cout << "process: " << processes[i].pid << " is continued at " << total_time << "ms\n"; processes[i].wait_time += total_time - processes[i].last_run_time; }

            ///checking if need to go again
            if (processes[i].time_left > quantum_time)
            {
                total_time += quantum_time;
                processes[i].time_left -= quantum_time;
                processes[i].last_run_time = total_time;
                cout << "process: " << processes[i].pid << " is passed at " << total_time << "ms\n";
            }
            else
            {
                total_time += processes[i].time_left;
                processes[i].time_left = 0;
                processes[i].end_time = total_time;
                processes[i].is_complete = true;
                ///checking if the process is finished on it's first go through
                if (processes[i].wait_time == 0) {processes[i].wait_time = processes[i].start_time - processes[i].arrival_time;}

                cout << "Process: " << processes[i].pid << " ended at " << total_time << "ms\n";
                remaining --;
            }
        }
    }
}

void sjf(process processes[], int size)
{
    int remaining = size;
    int total_time = 0;

    while (remaining > 0)
    {   ///checking for the smallest job
        int smallest = INT_MAX;
        int smallest_location = -1;
        for (int i = 0; i < size; i++)
        {
            if (processes[i].burst_time < smallest && processes[i].arrival_time <= total_time && !processes[i].is_complete)
            { smallest = processes[i].burst_time; smallest_location = i;}
        }
        ///checking for idle
        if (smallest_location == -1) { total_time ++; continue;}
        
        cout << "Process: " << processes[smallest_location].pid << " started at " << total_time << "ms\n";\
        ///calculating the simulation times
        processes[smallest_location].start_time = total_time;
        total_time += processes[smallest_location].burst_time;
        processes[smallest_location].wait_time = processes[smallest_location].start_time - processes[smallest_location].arrival_time;
        processes[smallest_location].end_time = total_time;
        processes[smallest_location].is_complete = true;
        cout << "Process: " << processes[smallest_location].pid << " ended at " << total_time << "ms\n";
        remaining --;
    }
}

int main(int argc, char const *argv[])
{
    string choice;
    ///making formating consistent in lowercase
    choice = argv[2];
    for (char &c : choice) { c = tolower(static_cast<unsigned char>(c)); }
    ///checking if the correct number of arguments are given through the command line
    if ((argc != 4 && choice == "rr") || (argc != 3 && choice != "rr")) { cout << "Not enough arguments\n"; exit(-1); }
    ///loading input file
    fstream input_file(argv[1]);
    if (!input_file.is_open()) { cout << "File could not be opened\n"; exit(-2);}
    ///getting the number of process there are
    int num_process = 0;
    string input = "";
    getline(input_file, input);
    num_process = stoi(input);
    process processes[num_process];

    string temp;
    int pos;
    ///loading the file
    for (int i = 0; i < num_process; i++)
    {
        getline(input_file, input);

        temp = input;
        pos = temp.find(' ');
        temp = temp.substr(0, pos);
        processes[i].pid = stoi(temp);

        temp = input.substr(pos + 1);
        pos = temp.find(' ');
        processes[i].arrival_time = stoi(temp.substr(0, pos));

        temp = temp.substr(pos + 1);
        processes[i].burst_time = stoi(temp);
        processes[i].time_left = stoi(temp);

    }
    
    sort(processes, processes + num_process, []
        (const process &a, const process &b) 
    {return a.arrival_time < b.arrival_time; });

    
    
    if (choice == "fcfs")
    {
        fcfs(processes, num_process);
    }
    else if (choice == "rr")
    {
        int quantum_time = stoi(argv[3]);
        rr(processes, num_process, quantum_time);
    }
    else if (choice == "sjf")
    {
        sjf(processes, num_process);
    }
    else
    {
        cout << "No real selection.\n";
        exit(-2);
    }


    sort(processes, processes + num_process, []
        (const process &a, const process &b) 
    {return a.pid < b.pid; });

    ///printing
    float total_wait_time = 0;

    cout << "PID    Arrival Time    Start Time  End Time    Running Time    Waiting Time\n";
    for (int i = 0; i < num_process; i++)
    {
        processes[i].print();
        total_wait_time += processes[i].wait_time;
    }
    
    cout << "Average wait time: " << total_wait_time / num_process << endl;

    return 0;
}
