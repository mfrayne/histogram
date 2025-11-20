#include "histogram.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>


int get_bar_proportion();
int output_histogram(FILE* out);
int signal_handler(int signum);

// Note: It is bad practice to use STDIO in signal handlers. 
// For this assignment however, I am guaranteeing that multiple signals will **not** be sent at or near the same time, thus it is __okay__ to use STDIO in your signal handlers.
// That is to say, feel free to use printf, fprintf, etc. in your signal handlers.
// Challenge: For the astute, try to implement the histogram program where your signal handlers have only 1 line of code (setting a global variable/flag). Even better if you use sigaction instead of signal.

int get_bar_proportion(){
	int longest_bar = histogram[0];
	
	for(int i = 1; i<sizeof(histogram)/sizeof(hist_int_t); i++){
		if(histogram[i]>longest_bar){
			longest_bar = histogram[i];
		}
	}
	if(longest_bar == 0){
		return 0;
	}
	return MAX_BAR_WIDTH/longest_bar;
}

int output_histogram(FILE* out){
	int num_bars = sizeof(histogram)/sizeof(hist_int_t);

	int bar_proportion = get_bar_proportion();

	for(int i = 0; i < num_bars; i++){
		fprintf(out,"%20d 0x%02X |",histogram[i],i);
		//do math to determine # count
		int bar_count = bar_proportion * histogram[i];
		for(int j = 0; j<bar_count;j++){
			fprintf(out,"#");
		}
		
		fprintf(out,"|\n");
	}
	return 0;
}

int signal_handler(int signum){
	//output current state of histogram
	//
	//if signum = signal for int or term then exit
	return 0;
}


int main(void) {
	//signal(SIGUSR1,)
	//signal(SIGINT,)
	//signal(SIGTERM,)
	
	//ssize_t bytes_read;
	char current_char;
	//might need to change for error handling of read
	while(read(0,&current_char,1)>0){
		histogram[(unsigned int)current_char] += 1;
	}
	
	/*	
	do{
		bytes_read read()
		if(bytes_read=-1){
			//print error with errno
		}
		else if(bytes_read !=0){
			
		}
	}while(bytes_read != 0);
	*/

	output_histogram(stdout);
	return 0;
}
