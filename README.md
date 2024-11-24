This project aims to understand the performance of queuing in a packet switch.

To compile the program, use following command in terminal: ```g++ assign1.cpp -o a.exe```
					
To run the program, use the following commands:   
  FOR INQ:	```./a.exe -N 32 -B 8 -p 0.5 -queue INQ -k 0.8 -out output.txt -T 1000```  
  FOR KOUQ:	```./a.exe -N 32 -B 8 -p 0.5 -queue KOUQ -k 0.8 -out output.txt -T 1000```  
  FOR ISLIP:	```./a.exe -N 32 -B 8 -p 0.5 -queue ISLIP -k 0.8 -out output.txt -T 1000```  

Note: 
  1. If any parameters are not provided in the command line, that parameter will take the default value.
  2. If you want to keep any parameter as default, remove the corresponding tag, e.g. if you want to provide all 
     parameters apart from the probability of packet generation, then the instruction will be: ./a.out -N 32 -B 8 -queue 
     INQ -K 0.8 -out output.txt -T 1000

Output:   
  The output of the scheduling algorithm can be found in the provided output file in the same folder where the code
  file resides.	As well as, output will be displayed on the console.
