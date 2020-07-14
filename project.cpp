// g++ project.cpp -o a.out -lm
// ./a.out 1 2 0.01 200 4 0.5 120

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

typedef const std::string& alg_type;

alg_type FCFS = "FCFS";
alg_type SJF = "SJF";
alg_type SRT = "SRT";
alg_type RR = "RR";

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
	int turnaround;

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
			std::cout << q[i];
			if(i != q.size() - 1)
				std::cout << " "; 
		}
	}
	std::cout << "]\n";
}

void start_sim(int n_procs, int seed, double lambda, int upper, int t_cs, double alpha, int t_slice, alg_type at, FILE* out, std::string rr_add="END")
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
		proc->turnaround = 0;		

		proc->cpu_bursts = std::vector<int>(proc->n_bursts, 0);
		proc->io_bursts = std::vector<int>(proc->n_bursts-1, 0);

		for(int i = 0; i < proc->n_bursts; i++)
		{
			proc->cpu_bursts[i] = ceil(exp_random(lambda, upper));
			if(i != proc->n_bursts-1) proc->io_bursts[i] = ceil(exp_random(lambda, upper));
		}

		// for(int i = 0; i < proc->cpu_bursts.size(); i++)
		//	std::cout << proc->cpu_bursts[i] << " ";		
		//std::cout << std::endl;
		//for(int i = 0; i < proc->io_bursts.size(); i++)
		//	std::cout << proc->io_bursts[i] << " ";		

		std::cout << "Process " << proc->id << " [NEW] (arrival time " << proc->arr_time << " ms) " << proc->n_bursts << " CPU bursts\n";
		proc_map.insert(std::pair<char, process*>(c, proc));
		procs.push_back(proc);	
		// finish setting up the processes
	}

	std::sort(procs.begin(), procs.end());	

	// probably need more variables here
	process* cpu_burst_proc = NULL; // in current use
	process* io_burst_proc = NULL;

	std::vector<process*> ready_queue; // the ready queue
	int num_con_switches = 0;
	int num_preempts = 0;
	bool done = false;
	int timer = 0;
	int finished = 0; // number of procs finished

	// more stuff here
	std::cout << "time " << timer << "ms: Simulator started for " + at + " ";
	printq(ready_queue);	
	while(finished < n_procs)
	{
		// logic here
		switch(at_type)
		{
			case FCFS:
				break;
			case SJF:
				break;
			case SRT:
				break;
			case RR:
				break;
			default:
				break;	
		}
		timer++;
	}

	// file write here

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
	// RUN FCFS
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, FCFS, out);	
	// RUN SJF
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, SJF, out);	
	// RUN SRT
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, SRT, out);	
	// RUN RR
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, RR, out, rr_add);	
	return EXIT_SUCCESS;
}
