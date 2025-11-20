#include "histogram.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int get_bar_proportion();
int output_histogram(FILE* out);
void signal_handler(int signum);


//gets the index with the highest value in histogram
//needed to calculate the number of # for each bar in the graph so each are proportioned correctly
int get_longest_bar(){
	int longest_bar = histogram[0];//first value is used as the baseline
	
	//iterates through all elements of histogram array assuming they are of type hist_int_t 
	for(int i = 1; i<sizeof(histogram)/sizeof(hist_int_t); i++){
		if(histogram[i]>longest_bar){
			longest_bar = histogram[i];//replaces the largest value if comes across larger value
		}
	}
	return longest_bar;
}

//prints output to whatever file passed into out (will either be stdout or histo.out)
int output_histogram(FILE* out){

	int num_bars = sizeof(histogram)/sizeof(hist_int_t);
	int longest_bar = get_longest_bar();
	
	//iterates through each element of histogram array assuming an array of hist_int_t elements
	for(int i = 0; i < num_bars; i++){
		
		//count of the bar with 20 space padding
		//then the byte value in hex with 2 0 padding
		fprintf(out,"%20d 0x%02X |",histogram[i],i);
		
		//calculates the length of each bar using ratio
		//(current bar length)/(max bar length)=(num #)/(50 - max alloted #)
		//ensures each bar is porportional
		double bar_count = MAX_BAR_WIDTH * ((double)histogram[i]/longest_bar);
		
		//prints the number of # rounded down to the neared int
		for(int j = 0; j<(int)bar_count;j++){
			fprintf(out,"#");
		}
		//ensured a closing bar even if the propportion too small for # to show
		if(bar_count>0){
			fprintf(out,"|");
		}

		fprintf(out,"\n");
	}
	return 0;
}

void signal_handler(int signum){
	
	FILE* output = fopen("histo.out","w");
	output_histogram(output);
	fclose(output);
	
	//exits only if sigint or sigterm used as interupts
	if(signum==2 || signum ==15){
		exit(-1);
	}
}


int main(void) {

	signal(SIGUSR1,signal_handler);
	signal(SIGINT,signal_handler);
	signal(SIGTERM,signal_handler);
	
	//unsigned int used to handle extended ascii characters so they're not negative
	unsigned int current_byte;

	//read only returns 0 for eof and -1 for error so if >0 then it's still reading bytes
	while(read(0,&current_byte,1)>0){
		//increases the size of the histogram bar associated with the byte just read
		histogram[current_byte] += 1;
	}
	
	//if no signals encountered then once everything is read the histogram is printed to stdout
	output_histogram(stdout);
	return 0;
}
