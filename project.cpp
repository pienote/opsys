//Group: Tommy Cesard, Phillip Chang, Money Brown, My Name Jeff
//CSCI 4210
//Project 1

/*
g++ project.cpp -o a.out -lm
./a.out 1 2 0.01 256 4 0.5 128
Test 2     ./a.out 1 2 0.01 256 4 0.5 128
Test 3     ./a.out 2 2 0.01 256 4 0.5 128
Test 4     ./a.out 16 2 0.01 256 4 0.75 64
Test 5     ./a.out 8 64 0.001 4096 4 0.5 2048
*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>


struct process {
	bool done = false;
	char id; // a to z
	
	int arr_time; // determined at the beginning in preemption
	int burst_time;
	int rem_burst_time;	

	int wait_time;

	int n_bursts;
	int rem_bursts;	// remaining
	
	int tau;
	int turnaround_time;

	int cpu_index;	// which cpu burst it is on
	int io_index;
	
	std::vector<int> cpu_bursts;
	std::vector<int> io_bursts;

	bool operator<(const process& p)
	{
		if(arr_time < p.arr_time)
		{
			return true;
		}
		else if(arr_time == p.arr_time)
		{
			return id < p.id;
		}
		else
		{
			return false;
		}
	}
};

double exp_random(double lambda, int upper)
{
	double x;
	while(1)
	{
		double r = drand48();   /* uniform dist [0.00,1.00) -- also check out random() */
		x = -log(r) / lambda;
		if(x > upper)
			continue;
		else
			break;
	}
	return x;
}

void printq(std::vector<process*> q)
{
	std::cout << "[Q ";
	if(q.empty())
	{
		std::cout << "<empty>";	
	}
	else
	{
		for(int i = 0; i < q.size(); i++)
		{
			std::cout << q[i]->id;
			if(i != q.size() - 1)
				std::cout << " "; 
		}
	}
	std::cout << "]\n";
}

//Runs the FCFS CPU scheduling algorithm and returns a list containing
//[average wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<int> FCFS(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs)
{
	for(int i = 0; i < procs.size(); i++)
	{
		std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
	}
	std::cout << "time 0ms: Simulator started for FCFS [Q <empty>]\n";
	/*
		do shit
	*/
	std::vector<int> temp = {0, 0, 0, 0};
	return temp;
}

//Runs the SJF CPU scheduling algorithm and returns a list containing
//[average wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<int> SJF(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs)
{
	for(int i = 0; i < procs.size(); i++)
	{
		std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
	}
	std::cout << "time 0ms: Simulator started for SJF [Q <empty>]\n";
	/*
		do shit
	*/
	std::vector<int> temp = {0, 0, 0, 0};
	return temp;
}

//Runs the SRT CPU scheduling algorithm and returns a list containing
//[average wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<int> SRT(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs)
{
	for(int i = 0; i < procs.size(); i++)
	{
		std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
	}
	std::cout << "time 0ms: Simulator started for SRT [Q <empty>]\n";
	/*
		do shit
	*/
	std::vector<int> temp = {0, 0, 0, 0};
	return temp;
}

//Runs the RR CPU scheduling algorithm and returns a list containing
//[average wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<int> RR(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs)
{
	for(int i = 0; i < procs.size(); i++)
	{
		std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
	}
	std::cout << "time 0ms: Simulator started for RR [Q <empty>]\n";
	/*
		do shit
	*/
	std::vector<int> temp = {0, 0, 0, 0};
	return temp;
}

//Prints stats to the required output file
void print_stats(std::string alg, std::vector<int> data, FILE* out)
{
	/*
		print shit
	*/
}

void start_sim(int n_procs, int seed, double lambda, int upper, int t_cs, double alpha, int t_slice, FILE* out, std::string rr_add="END")
{
	std::map<char, process*> proc_map; // letter->process with letter
	std::vector<process*> procs; // all the processes
	srand48(seed);
	for(char c='A'; c < ((char) 'A'+n_procs); c++)
	{	
		// set up the process
		process* proc = new process();
		proc->id = c;
		proc->arr_time = floor(exp_random(lambda, upper));
		proc->tau = (int) 1 / lambda;
		proc->wait_time = 0;	
		proc->n_bursts = trunc(drand48()*100) + 1;	
		proc->rem_bursts = proc->n_bursts;
		proc->turnaround_time = 0;		
		proc->cpu_index = 0;
	    proc->io_index = 0;	
		proc->turnaround_time = 0;

		proc->cpu_bursts = std::vector<int>(proc->n_bursts, 0);
		proc->io_bursts = std::vector<int>(proc->n_bursts-1, 0);

		double sum = 0;
		for(int i = 0; i < proc->n_bursts; i++)
		{
			proc->cpu_bursts[i] = ceil(exp_random(lambda, upper));
			if(i != proc->n_bursts-1) proc->io_bursts[i] = ceil(exp_random(lambda, upper));
		}
		proc_map.insert(std::pair<char, process*>(c, proc));
		procs.push_back(proc);	
		// finish setting up the processes
	}

	std::sort(procs.begin(), procs.end());	

	std::vector<int> data;
	
	data = FCFS(proc_map, procs, t_cs);
	print_stats("FCFS", data, out);
	data = SJF(proc_map, procs, t_cs);
	print_stats("SJF", data, out);
	data = SRT(proc_map, procs, t_cs);
	print_stats("SRT", data, out);
	data = RR(proc_map, procs, t_cs);
	print_stats("RR", data, out);
}	

int main(int argc, char* argv[])
{
	setvbuf( stdout, NULL, _IONBF, 0 );
	if(argc != 8 && argc != 9)
	{
		fprintf(stderr, "ERROR: incorrect number of arguments\n");
		return EXIT_FAILURE;
	}
	// process* procs[26];
	
	FILE* out;	
	int n = atoi(argv[1]);
	int seed = atoi(argv[2]);
	double lambda = atof(argv[3]);
	int upper = atoi(argv[4]);
	unsigned int t_cs = atoi(argv[5]);
	int alpha = atoi(argv[6]);
	int t_slice = atoi(argv[7]);
	std::string rr_add;
	if(argc == 9 && strcmp(argv[8], "BEGINNING") == 0)
		rr_add = "BEGINNING";

	out = fopen("simout.txt", "w");	

	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, out);	

	return EXIT_SUCCESS;
}
