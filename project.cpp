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
	
	bool preempt = false;

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
	std::vector<int> turnaround_times;

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

struct CPU {
	process * curr_proc = NULL;     //current process CPU is being occupied by
	int rem_burst_time = 0;			//If process is being run, how much longer until command switch completes
	int switch_in_time = 0;			//If process is switching into the CPU, how much longer until CPU burst can be excuted
	int switch_out_time = 0;		//If process is switching out of the CPU, how much longer until another process can switch in
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
		for(unsigned int i = 0; i < q.size(); i++)
		{
			std::cout << q[i]->id;
			if(i != q.size() - 1)
				std::cout << " "; 
		}
	}
	std::cout << "]\n";
}


//Runs the SJF CPU scheduling algorithm and returns a list containing
//[average wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<double> SJF(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs)
{
	for(unsigned int i = 0; i < procs.size(); i++)
	{
		std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
	}
	std::cout << "time 0ms: Simulator started for SJF [Q <empty>]\n";
	/*
		do shit
	*/
	std::vector<double> temp = {0, 0, 0, 0};
	return temp;
}

//Runs the SRT CPU scheduling algorithm and returns a list containing
//[average wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<double> SRT(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs)
{
	for(unsigned int i = 0; i < procs.size(); i++)
	{
		std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
	}
	std::cout << "time 0ms: Simulator started for SRT [Q <empty>]\n";
	/*
		do shit
	*/
	std::vector<double> temp = {0, 0, 0, 0};
	return temp;
}

//Runs the RR CPU scheduling algorithm and returns a list containing
//[wait time time, average turnaround time, # of context switches, # of preemptions]
std::vector<double> RR(std::map<char, process*> proc_map, std::vector<process*> procs, int t_cs, int t_slice, std::string rr_add, std::string alg)
{
/*##################################################################*/
	bool full_output = false;
/*##################################################################*/
	for(unsigned int i = 0; i < procs.size(); i++)
	{
		if(procs[i]->n_bursts > 1)
			std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU bursts\n";
		else
			std::cout << "Process " << procs[i]->id << " [NEW] (arrival time " << procs[i]->arr_time << " ms) " << procs[i]->n_bursts << " CPU burst\n";
	}
	std::cout << "time 0ms: Simulator started for " << alg << " [Q <empty>]\n";
	std::vector<double> data = {0, 0, 0, 0};
	int time = 0;
	int rem_slice_time;
	int total_cs = 0;
	int total_preemptions = 0;
	int procs_completed = 0;
	std::vector<process*> readyQ;
	CPU* cpu = new CPU();
	while((unsigned)procs_completed < procs.size())
	{
		//Decrement rem_burst_time/switch_in_time/switch_out_time
		if(cpu->curr_proc != NULL)
		{
			//If CPU burst is running
			if(cpu->rem_burst_time != 0)
			{
				cpu->rem_burst_time -= 1;
				//Check if process finished burst
				if(cpu->rem_burst_time == 0)
				{
					bool terminated = false;
					cpu->curr_proc->rem_bursts -= 1;
					//Check if process terminated
					if(cpu->curr_proc->rem_bursts == 0)
					{
						std::cout << "time " << time << "ms: Process " << cpu->curr_proc->id << " terminated ";
						printq(readyQ);
						terminated = true;
						procs_completed++;
					}
					else
					{
						if(full_output || time < 1000)
						{
							std::cout << "time " << time << "ms: Process " << cpu->curr_proc->id << " completed a CPU burst; " << cpu->curr_proc->rem_bursts << " bursts to go ";
							printq(readyQ);
						}
					}

					//If process was not terminate, calculate time until I/O burst completion
					if(!terminated)
					{
						if(full_output || time < 1000)
						{
							std::cout << "time " << time << "ms: Process " << cpu->curr_proc->id << " switching out of CPU; will block on I/O until time "
							<< time + t_cs/2 + cpu->curr_proc->io_bursts[cpu->curr_proc->io_index] << "ms ";
							printq(readyQ);
						}
						cpu->curr_proc->rem_burst_time = t_cs/2 + cpu->curr_proc->io_bursts[cpu->curr_proc->io_index] + 1;
						cpu->curr_proc->io_index++;
					}
					//std::cout << "TURNAROUND TIME: Process " << cpu->curr_proc->id << " started at " << cpu->curr_proc->turnaround_times[cpu->curr_proc->cpu_index]
					//<< "ms ended at " << time << "ms\n";
					int start_time = cpu->curr_proc->turnaround_times[cpu->curr_proc->cpu_index];
					cpu->curr_proc->turnaround_times[cpu->curr_proc->cpu_index] = time + t_cs/2 - start_time;
					cpu->curr_proc->cpu_index++;
					cpu->rem_burst_time = 0;
					cpu->switch_in_time = 0;
					cpu->switch_out_time = t_cs/2;
				}
				rem_slice_time -= 1;
				//Check if slice time ran out
				if(rem_slice_time == 0 && cpu->rem_burst_time > 0)
				{
					//If there is a process to actuall preempt the CPU process
					if(readyQ.size() > 0)
					{
						cpu->curr_proc->preempt = true;
						if(full_output || time < 1000)
						{
							std::cout << "time " << time << "ms: Time slice expired; process " << cpu->curr_proc->id << " preempted with "
							<< cpu->rem_burst_time << "ms to go ";
							printq(readyQ);
						}
						total_preemptions += 1;
						cpu->curr_proc->cpu_bursts[cpu->curr_proc->cpu_index] = cpu->rem_burst_time;
						cpu->rem_burst_time = 0;
						cpu->switch_in_time = 0;
						cpu->switch_out_time = t_cs/2;
					}
					else
					{
						if(full_output || time < 1000)
						{
							std::cout << "time " << time << "ms: Time slice expired; no preemption because ready queue is empty ";
							printq(readyQ);
						}
					}
				}
			}
			//If a process is currently being switched in to the CPU
			else if(cpu->switch_in_time != 0)
			{
				cpu->switch_in_time -= 1;
				//New process begins CPU burst
				if(cpu->switch_in_time == 0)
				{
					if(full_output || time < 1000)
					{
						if(cpu->curr_proc->preempt)
						{
							std::cout << "time " << time << "ms: Process " << cpu->curr_proc->id << " started using the CPU with "
							<< cpu->curr_proc->cpu_bursts[cpu->curr_proc->cpu_index] << "ms burst remaining ";
							printq(readyQ);
						}
						else
						{
							std::cout << "time " << time << "ms: Process " << cpu->curr_proc->id << " started using the CPU for "
							<< cpu->curr_proc->cpu_bursts[cpu->curr_proc->cpu_index] << "ms burst ";
							printq(readyQ);
						}
					}
					cpu->curr_proc->preempt = false;
					total_cs += 1;
					cpu->rem_burst_time = cpu->curr_proc->cpu_bursts[cpu->curr_proc->cpu_index];
					rem_slice_time = t_slice;
					/*
					if(cpu->curr_proc->turnaround_times[cpu->curr_proc->cpu_index] == 0)
					{
						cpu->curr_proc->turnaround_times[cpu->curr_proc->cpu_index] = time;
					}
					*/
				}
			}
			//If a process is currently being switched out of the CPU
			else if(cpu->switch_out_time != 0)
			{
				cpu->switch_out_time -= 1;
				if(cpu->switch_out_time == 0)
				{
					if(cpu->curr_proc->preempt)
					{
						if(!rr_add.compare("BEGINNING"))
						{
							readyQ.insert(readyQ.begin(), cpu->curr_proc);
						}
						else
						{
							readyQ.push_back(cpu->curr_proc);
						}
					}
					cpu->curr_proc = NULL;
				}
			}
			else
			{
				std::cerr << "ERROR: CPU occupied by dead process\n";
				return data;
			}
		}

		//Check if a process arrives
		for(unsigned int i = 0; i < procs.size(); i++)
		{
			if(procs[i]->arr_time == time)
			{
				//add process to the readyQ
				if(!rr_add.compare("BEGINNING"))
				{
					readyQ.insert(readyQ.begin(), procs[i]);
				}
				else
				{
					readyQ.push_back(procs[i]);
				}
				//print out readyQ
				if(full_output || time < 1000)
				{
					std::cout << "time " << time << "ms: Process " << procs[i]->id  << " arrived; added to ready queue ";
					printq(readyQ);
				}
				if(procs[i]->turnaround_times[procs[i]->cpu_index] == 0)
				{
					procs[i]->turnaround_times[procs[i]->cpu_index] = time;
				}
			}
		}

		//Check if a process finishes performing I/O
		for(unsigned int i = 0; i < procs.size(); i++)
		{
			if(procs[i]->rem_burst_time > 0)
			{
				procs[i]->rem_burst_time -= 1;
				if(procs[i]->rem_burst_time == 0)
				{
					if(!rr_add.compare("BEGINNING"))
					{
						readyQ.insert(readyQ.begin(), procs[i]);
					}
					else
					{
						readyQ.push_back(procs[i]);
					}
					if(procs[i]->turnaround_times[procs[i]->cpu_index] == 0)
					{
						procs[i]->turnaround_times[procs[i]->cpu_index] = time;
					}
					if(full_output || time < 1000)
					{
						std::cout << "time " << time << "ms: Process " << procs[i]->id  << " completed I/O; added to ready queue ";
						printq(readyQ);
					}
					//procs[i]->wait_time += t_cs/2;
				}
			}
		}

		//Check if a process starts using the CPU
		if(cpu->curr_proc == NULL && readyQ.size() > 0)
		{
			cpu->curr_proc = readyQ[0];
			readyQ.erase(readyQ.begin());
			cpu->switch_in_time = t_cs/2;
		}

		//Incremet wait time for every process in the readyQ
		for(unsigned int i = 0; i < readyQ.size(); i++)
		{
			readyQ[i]->wait_time += 1;
		}
		time++;
	}
	time++;
	std::cout << "time " << time << "ms: Simulator ended for " << alg << " [Q <empty>]\n";
	int total_wait_time = 0;
	int num_cpu_bursts = 0;
	int total_turnaround_time = 0;
	for(unsigned int i = 0; i < procs.size(); i++)
	{
		total_wait_time += procs[i]->wait_time;
		num_cpu_bursts += procs[i]->cpu_bursts.size();
		for(unsigned int j = 0; j < procs[i]->cpu_bursts.size(); j++)
		{
			total_turnaround_time += procs[i]->turnaround_times[j];
		}
	}
	data[0] = (double)total_wait_time/num_cpu_bursts;
	data[1] = (double)total_turnaround_time/num_cpu_bursts;
	data[2] = (double)total_cs;
	data[3] = (double)total_preemptions;	
	return data;
}

//Takes in vector containing [average wait time time, average turnaround time, # of context switches, # of preemptions]
//and prints it to the out FILE in the desired format
void print_stats(std::string alg, double ave_cpu_bt, std::vector<double> data, FILE* out)
{
	if(alg == "FCFS")
	{
		fprintf(out, "Algorithm FCFS\n");
		//data[1] += data[0]; 
	}
	else if(alg == "SJF")
		fprintf(out, "Algorithm SJF\n");
	else if(alg == "SRT")
		fprintf(out, "Algorithm SRT\n");
	else
		fprintf(out, "Algorithm RR\n");
	fprintf(out, "-- average CPU burst time: %.3f ms\n", ave_cpu_bt);
	fprintf(out, "-- average wait time: %.3f ms\n", data[0]);
	fprintf(out, "-- average turnaround time: %.3f ms\n", data[1]);
	fprintf(out, "-- total number of context switches: %d\n", (int)data[2]);
	fprintf(out, "-- total number of preemptions: %d\n", (int)data[3]);
}

void start_sim(int n_procs, int seed, double lambda, int upper, int t_cs, double alpha, int t_slice, FILE* out, std::string rr_add, std::string algorithm)
{
	std::map<char, process*> proc_map; // letter->process with letter
	std::vector<process*> procs; // all the processes
	int sum_cpu_bt = 0;
	int num_cpu_b = 0;
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
		proc->turnaround_times = std::vector<int>(proc->n_bursts, 0);

		for(int i = 0; i < proc->n_bursts; i++)
		{
			proc->cpu_bursts[i] = ceil(exp_random(lambda, upper));
			sum_cpu_bt += proc->cpu_bursts[i];
			num_cpu_b++;
			if(i != proc->n_bursts-1) proc->io_bursts[i] = ceil(exp_random(lambda, upper));
		}
		proc_map.insert(std::pair<char, process*>(c, proc));
		procs.push_back(proc);	
		// finish setting up the processes
	}

	std::sort(procs.begin(), procs.end());	

	//Calculates average CPU burst time
	double ave_cpu_bt = (double)sum_cpu_bt/num_cpu_b;
	std::vector<double> data;
	
	if(algorithm == "FCFS")
	{
		data = RR(proc_map, procs, t_cs, 999999999, "END", "FCFS");
		print_stats("FCFS", ave_cpu_bt, data, out);
		std::cout << "\n";
	}
	else if(algorithm == "SJF")
	{
		data = SJF(proc_map, procs, t_cs);
		print_stats("SJF", ave_cpu_bt, data, out);
		std::cout << "\n";
	}
	else if(algorithm == "SRT")
	{
		data = SRT(proc_map, procs, t_cs);
		print_stats("SRT", ave_cpu_bt, data, out);
		std::cout << "\n";
	}
	else
	{
		data = RR(proc_map, procs, t_cs, t_slice, rr_add, "RR");
		print_stats("RR", ave_cpu_bt, data, out);
	}
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
	out = fopen("simout.txt", "w");
	if(out == NULL)
	{
		std::cerr << "ERROR: could not open file\n";
		return EXIT_FAILURE;
	}
	std::string rr_add = "END";
	if(argc == 9 && strcmp(argv[8], "BEGINNING") == 0)
		rr_add = "BEGINNING";
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, out, rr_add, "FCFS");
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, out, rr_add, "SJF");
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, out, rr_add, "SRT");
	start_sim(n, seed, lambda, upper, t_cs, alpha, t_slice, out, rr_add, "RR");

	fclose(out);
	return EXIT_SUCCESS;
}
