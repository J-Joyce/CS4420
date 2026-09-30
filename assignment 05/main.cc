#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

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
    bool is_complete;

    process() : pid(0), arrival_time(0), burst_time(0),
                start_time(0), end_time(0), wait_time(0),
                is_complete(false) {}

    void print()
    {
        cout << "pid: " << pid << ", arrival_time: " << arrival_time << ", burst_time: " << burst_time << endl;
        cout << "start_time: " << start_time << ", end time: " << end_time << ", wait time: " << wait_time << ", is completed: " << is_complete << endl;
    }
};

void fcfs(process processes[], int size)
{
    int total_time = 0;
    for (int i = 0; i < size; i++)
    {
        processes[i].start_time = total_time;
        total_time += processes[i].burst_time;
        processes[i].wait_time = processes[i].start_time - processes[i].arrival_time;
        processes[i].end_time = total_time;
        processes[i].is_complete = true;
    }
    
}
void rr(process processes[], int size, int quantum_time)
{

}

void sjf(process processes[], int size)
{

}

int main(int argc, char const *argv[])
{
    ///checking if the correct number of arguments are given through the command line
    if (argc != 4) { cout << "Not enough arguments\n"; exit(-1); }
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
        pos = temp.find(',');
        temp = temp.substr(0, pos);
        processes[i].pid = stoi(temp);

        temp = input.substr(pos + 1);
        pos = temp.find(',');
        processes[i].arrival_time = stoi(temp.substr(0, pos));

        temp = temp.substr(pos + 1);
        processes[i].burst_time = stoi(temp);

    }
    
    sort(processes, processes + num_process, []
        (const process &a, const process &b) 
    {return a.arrival_time < b.arrival_time; });

    string choice;
    ///making formating consistent in lowercase
    choice = argv[2];
    for (char &c : choice) { c = tolower(static_cast<unsigned char>(c)); }
    
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

    ///printing
    float total_wait_time = 0;
    for (int i = 0; i < num_process; i++)
    {
        processes[i].print();
        cout << endl;
        total_wait_time += processes[i].wait_time;
    }
    
    cout << "Average wait time: " << total_wait_time / num_process << endl;

    return 0;
}
