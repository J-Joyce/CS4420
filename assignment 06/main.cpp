#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

int main()
{
    int num_rounds = 0;
    int num_voters = 0;
    ///input
    do
    {
        cout << "Enter the number of rounds you would like to have: ";
        cin >> num_rounds;
        if (num_rounds <= 0)
        {
            cout << "Error: can't be negative number or zero\n";
        }
        
    } while (num_rounds <= 0);

    do 
    {
        cout << "Enter the number of voters: ";
        cin >> num_voters;
        if (num_voters < 3)
        {
            cout << "Error: not enough voters\n";
        }
        
    } while (num_voters < 3);

    int voter_id = -1;

    int pipes[num_voters][2];
    //creating all pipes for the fround
    for (int  i = 0; i < num_voters; i++)
    {
        if (pipe(pipes[i]) == -1) { perror("pipe"); }
    }
    /// creating the child process
    for (int i = 0; i < num_voters; i++)
    {
        pid_t pid = fork();
        //error handling
        if (pid < 0) { cout << "Error " << i << endl; }
        ///child process
        if (pid == 0)
        {///keeping track of voter ids
            voter_id = i;
            ///making it so each child had a different seed for rand generation
            srand(time(NULL) + voter_id);
            break;
        }
        else voter_id = -1;
    }
    ///closing non needed pipes
    if (voter_id == -1)///parent closing
    {
        for (int i = 0; i < num_voters; i++)
        { close(pipes[i][1]); }
    }
    else
    {
        for (int i = 0; i < num_voters; i++)//closing for children
        {
            close(pipes[i][0]);
            if (i != voter_id)/// not closing the write for itself
            {
                close(pipes[i][1]);
            }
            
        }
    }


    int vote = -1;
    ///voting loop
    for (int round = 0; round < num_rounds; round++)
    {
        int votes_yes = 0;
        int votes_no = 0;
        if (voter_id == -1)
        {
            cout << "\n=== Round " << round + 1 << " ===" << endl;
            for (int i = 0; i < num_voters; i++)
            {///reading all votes and counting for winner
                read(pipes[i][0], &vote, sizeof(int));
                if (vote == 0) { votes_no++;}
                else { votes_yes++;}
            }
            ///outputing victor
            if (votes_yes > votes_no)
            {
                cout << "Yes has won this round (" << votes_yes << " Yes vs " << votes_no << " No)"  << endl;
            }
            else if (votes_no > votes_yes)
            {
                cout << "No has won this round (" << votes_no << " No vs " << votes_yes << " Yes)" << endl;
            }
            else
            {
                cout << "it is a tie" << endl;
            }
            
        }
        else 
        {///creating the random number for vote
            int vote = rand() % 2;
            ///having the child print
            if (vote == 0) cout << "Voter " << voter_id << " is voting No\n"; 
            else cout << "Voter " << voter_id << " is voting Yes\n";
            ///writing back to judge
            write(pipes[voter_id][1], &vote, sizeof(int));
            ///cleaning up output 
            usleep(100000);
        }
        
    }
    ///waiting for children
    if (voter_id != -1)
    {
        close(pipes[voter_id][1]);
        exit(0);        
    }
    for (int i = 0; i < num_voters; i++)
    {
        wait(NULL);
        close(pipes[i][0]);
    }
    return 0;
}