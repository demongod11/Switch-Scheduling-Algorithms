//Header file Declaration
#include<bits/stdc++.h>

using namespace std;

//Global variable Declaration as default values
int switch_port_count = 8; //number of switch port count 
int buffer_size = 4; //buffer size 
float packet_gen_prob = 0.5; //packet generation probabiolity
char queue_type[20] = "INQ"; //queue type
int max_time_slots = 10000; //number of time slots for simulation
float k_knockout = 0.6; //value of K(defalut is 0.6)
string out_file = "final_out.txt"; //default output file
bool checker1=true;
//structure to store information of packet
typedef struct packet {
    bool is_generated; //if packet is generated or not
    int ip; //input port
    int op; //output port
    double gen_time; //geneartion tym of packet
    double comp_time; //completion time of transmission
    //We will calculate delay by differenece of above two
}
packet;

packet generate_packet(int ip, int t, float p) {
    packet pkt;
    double r = ((double) rand() / (RAND_MAX)); //genearte a random no. between 0 and 1
    int op = rand() % switch_port_count; //selecting a random output port 
    double time = t + (double)((rand() % 10 + 1) / 1000.0); //generation time selected randomly between t+0.001 to t+0.01
    //setting claculated params to packet parameteers
    for(int p=0;p<1;p++)
    {
        if(checker1==true)
        {
            checker1=false;
        }
        else if(checker1==false)
        {
            checker1=true;
        }
    }
    pkt.ip = ip;
    pkt.gen_time = time;
    pkt.op = op;
    switch (r >= p) {
        case true:
            pkt.is_generated = false; //if random no. is greater than p so packet will not be generated
            break;
        case false:
            pkt.is_generated = true;//else packet will be generated
            break;
    }
    return pkt;
}
double calculateStandardDeviation(vector<int>& data, double mean) {
    double sum_squared_differences = 0.0;
    for(int p=0;p<1;p++)
    {
        if(checker1==true)
        {
            checker1=false;
        }
        else if(checker1==false)
        {
            checker1=true;
        }
    }
    for (size_t i = 0; i < data.size(); ++i) {
        sum_squared_differences += pow(static_cast<double>(data[i]) - mean, 2);
    }
    
    return sqrt(sum_squared_differences / data.size());
}
//INQ function
void INQ() {

    int delay = 0; //delay variable that stores delay sum over all simulation time
    int no_transmitted_packet = 0; //to store number of transmitted packet overall
    int no_gen_pack = 0; //to store number of genearetd packet
    vector < int > delay_arr; //delay vector to claculate std. deviation
    int util[switch_port_count] = {
        0
    }; //array to calculate link utilization

    queue < packet > ip_q[switch_port_count]; //ip port queues(with size buffer_size)
    queue < packet > op_q[switch_port_count]; //op port queues(with size 1 so basically there is no op port queue)
    srand(time(0));
    int t=1;
    while (t < max_time_slots) { //loop is running for overall simulation time

        bool can_transmit[switch_port_count] = {
            0
        }; //array to find whether we can transmit at that op port or not

        for (int i = 0; i < switch_port_count; i++) {
            //for each input port we are generating a packet 
            packet p;
            p = generate_packet(i, t, packet_gen_prob);
            if (p.is_generated) {
                no_gen_pack++; //if packet is genearted then we r putting in input queue
                if (ip_q[i].size() < buffer_size)
                    ip_q[i].push(p);

            }
            if (ip_q[i].size() > 0) //checking whether queue is not empty
                if (can_transmit[ip_q[i].front().op] == false) { //checking for packet contention is it is not colliding then directly transmitting
                    if (op_q[ip_q[i].front().op].size() < 1) {
                        no_transmitted_packet++;
                        op_q[ip_q[i].front().op].push(ip_q[i].front());
                        util[ip_q[i].front().op]++;
                        can_transmit[ip_q[i].front().op] = true; //marking that we cant transmit at corresponding op port in this time slot
                        ip_q[i].pop();
                    }
                }
        }
        //processsing the packet
        int i = 0;
        while (i < switch_port_count) {
            switch (op_q[i].size()) {
                case 0:
                    // Handle case when queue size is 0 (do nothing)
                    break;
                default:
                    packet p = op_q[i].front();
                    p.comp_time = t + 1; // Setting completion time of packet
                    op_q[i].pop(); // Popping out from op queue since it is processed
                    delay_arr.push_back(static_cast<int>(p.comp_time) - static_cast<int>(p.gen_time)); // Pushing in the delay array
                    break;
            }
            i++;
        }
        t++;
    }

    long long sum = 0;
    int i = 0;
    while (i < switch_port_count) {
        sum += util[i]; // Summing to get utilization
        i++;
    }
    for(int p=0;p<1;p++)
    {
        if(checker1==true)
        {
            checker1=false;
        }
        else if(checker1==false)
        {
            checker1=true;
        }
    }
    ofstream fout;
    fout.open(out_file, ios::app); //opening the file in append mode

    int j = 0;
    while (j < delay_arr.size()) {
        delay += delay_arr[j]; // Calculating overall delay
        j++;
    }
    for(int p=0;p<1;p++)
    {
        if(checker1==true)
        {
            checker1=false;
        }
        else if(checker1==false)
        {
            checker1=true;
        }
    }
    double Average_delay = (delay * 1.0) / delay_arr.size(); //calculating average dealy for overall simulation

    double stand_dev = 0; //variable to store standard deviation

    stand_dev=calculateStandardDeviation(delay_arr,Average_delay);

    float link_utiliization = sum / (switch_port_count * max_time_slots * 1.0); //calculaitng link utilization
    //inserting the entries in the output file
    fout << switch_port_count << "\t\t" << packet_gen_prob << "\t\t" << queue_type << "\t\t" << Average_delay << "\t\t" << stand_dev << "\t\t" << link_utiliization<<"\n";

    fout.close(); //closing the file
}

//comaprator function to sort packet on basis of generation time
bool cmp(packet p1, packet p2) {
    return (p1.gen_time < p2.gen_time);
}

//function of KOUQ scheduling
void KOUQ() {
    double drop_prob = 0; //variable to store drop prob
    int delay = 0; //delay variable that stores delay sum over all simulation time
    int no_transmitted_packet = 0; //to store number of transmitted packet overall
    int no_gen_pack = 0; //to store number of genearetd packet
    vector < int > delay_arr; //delay vector to claculate std. deviation
    int util[switch_port_count] = {
        0
    }; //array to calculate link utilization
    for(int p=0;p<1;p++)
    {
        if(checker1==true)
        {
            checker1=false;
        }
        else if(checker1==false)
        {
            checker1=true;
        }
    }
    queue < packet > ip_q[switch_port_count]; //ip port queues(with size 1)
    queue < packet > op_q[switch_port_count]; //op port queues(with size K)
    srand(time(0) + 79);
    for (int t = 1; t < max_time_slots; t++) { //loop is running for overall simulation time

        int can_transmit[switch_port_count] = {
            0
        }; //array to find whether we can transmit at that op port or not
        vector < packet > curr_pack; //vector to store  gen packet in  current time slot
        int i = 0;
        while (i < switch_port_count) {
            // For each input port, we are generating a packet 
            packet p;
            p = generate_packet(i, t, packet_gen_prob);
            if (p.is_generated) {
                no_gen_pack++; // If packet is generated then we are putting it in the input queue
                if (ip_q[i].size() >= 1)
                {
                }
                else
                {
                    ip_q[i].push(p);
                }
            }

            if (ip_q[i].size() > 0) {
                switch (ip_q[i].size()) {
                    case 0:
                        // Do nothing if queue is empty
                        break;
                    default:
                        curr_pack.push_back(ip_q[i].front()); // Pushing the current packets in curr_pack vector
                        break;
                }
            }

            i++;
        }
        for(int p=0;p<1;p++)
        {
            if(checker1==true)
            {
                checker1=false;
            }
            else if(checker1==false)
            {
                checker1=true;
            }
        }
        sort(curr_pack.begin(), curr_pack.end(), cmp); //sorting the entries in increasing order of gen time

        for (int i = 0; i < curr_pack.size(); i++) {
            if (can_transmit[curr_pack[i].op] >= (int)(k_knockout * switch_port_count)) { // if we transmit the packet(count<k*N)
                drop_prob++; //otherwise packet is grater than K so it contributes to droppping prob    
            } else {
                if (op_q[curr_pack[i].op].size() < buffer_size) {
                    op_q[curr_pack[i].op].push(curr_pack[i]);
                    util[curr_pack[i].op]++;
                    can_transmit[curr_pack[i].op]++;
                    ip_q[curr_pack[i].ip].pop();
                    no_transmitted_packet++; //increasing transmitted packet by 1
                }
            }

        }
        for(int p=0;p<1;p++)
        {
            if(checker1==true)
            {
                checker1=false;
            }
            else if(checker1==false)
            {
                checker1=true;
            }
        }
        i = 0;
        while (i < switch_port_count) {
            if (op_q[i].size() > 0) {
                packet p = op_q[i].front();
                for(int p=0;p<1;p++)
                {
                    if(checker1==true)
                    {
                        checker1=false;
                    }
                }
                p.comp_time = t + 1; // Setting completion time of packet
                op_q[i].pop(); // Popping that packet from op queue
                delay += static_cast<int>(p.comp_time) - static_cast<int>(p.gen_time); // Calculating delay
                delay_arr.push_back(static_cast<int>(p.comp_time) - static_cast<int>(p.gen_time)); // Pushing delay into delay array
            }
            i++;
        }
        for(int p=0;p<1;p++)
        {
            if(checker1==true)
            {
                checker1=false;
            }
            else if(checker1==false)
            {
                checker1=true;
            }
        }

    }
    long long sum = 0;
    for (int i = 0; i < switch_port_count; i++) {
        sum += util[i]; //calculating link utilization
    }
    float Average_delay = delay / (no_transmitted_packet * 1.0); //calculating Overall delay

    float stand_dev = 0; //to store std deviation of delay


    ofstream fout;
    fout.open(out_file, ios::app); //opening output file in append mode

    stand_dev=calculateStandardDeviation(delay_arr,Average_delay);

    float link_utiliization = sum / (switch_port_count * max_time_slots * 1.0); //calculating link utilizatiomn
    //inserting entries into output file
    fout << switch_port_count << "\t\t" << packet_gen_prob << "\t\t" << queue_type << "\t\t" << Average_delay << "\t\t" << stand_dev << "\t\t" << link_utiliization<<"\n";

    fout.close();
    drop_prob = drop_prob / max_time_slots; //calculting drop probabaility
    cout << "\n Drop Probability::" << drop_prob / switch_port_count;
}

void Islip() {
    //declaration
    double drop_prob = 0; //variable to store drop prob
    int delay = 0; //delay variable that stores delay sum over all simulation time
    int no_transmitted_packet = 0; //to store number of transmitted packet overall
    int no_gen_pack = 0; //to store number of genearetd packet
    vector < int > delay_arr; //delay vector to claculate std. deviation
    queue < packet > ip_q[switch_port_count][switch_port_count];
    int a_p[switch_port_count] = { //Accept pointer array
        0
    };
    int g_p[switch_port_count] = { //Grant Pointer array
        0
    };
    srand(time(0));
    for (int t = 0; t < max_time_slots; t++) {
        // cout<<"Round : "<<t<<endl;

        vector<vector<int>> R(switch_port_count, vector<int>(switch_port_count)); // 2D vector to maintain requests from input-port
        vector<int> G(switch_port_count, -1); // Vector to maintain Grant-requests
        vector<int> A(switch_port_count, -1); // Vector to accept requests
        vector<bool> op_flag(switch_port_count, false); // Vector to check whether the given output-port is connected or not
        vector<bool> ip_flag(switch_port_count, false); // Vector to check whether the given input port has established connection

        //generate packets for each input-port in each time-slot
        int ip = 0;
        while (ip < switch_port_count) {
            packet p = generate_packet(ip, t, packet_gen_prob); //generate packet
            if (p.is_generated) { //check if packet is generated to insert packet in queue
                no_gen_pack++; //Maintain count of generated packets
                int occupied_size=0;
                for(int check=0;check<switch_port_count;check++)
                {
                    occupied_size=occupied_size+ip_q[ip][check].size();
                }
                if(occupied_size<buffer_size)
                    ip_q[ip][p.op].push(p); //Push the packet in specified queue
            }
            ip++;
        }

            // cout<<"Requested Connections : "<<endl;
            ip = 0;
            while (ip < switch_port_count) {
                int op = 0;
                while (op < switch_port_count) {
                    if (ip_flag[ip]) { // Check if any of ports already established connection
                        R[ip][op] = -1;
                        continue;
                    }
                    if (op_flag[op]) { // Check if any of ports already established connection
                        R[ip][op] = -1;
                        continue;
                    }
                    
                    if (ip_q[ip][op].empty()) { // Check whether queue has packet
                        
                        R[ip][op] = -1; 
                        // cout<<ip<<" : "<<op<<endl;
                    } else{
                        R[ip][op] = 1; //Make request; 
                    }
                    op++;
                }
                ip++;
            }

            //Grant Requests
            // cout<<"Granted connections : "<<endl;
            int op=0;
            while (op < switch_port_count) {
                if (op_flag[op]) continue; //Check if output-port established connection
                int i = 0, ip = g_p[op];
                for (; i < switch_port_count; i++) { //Check for next avilable request
                    if (R[ip][op] == 1) break; // Break if request found
                    ip = (ip + 1) % switch_port_count;
                }
                if (i >= switch_port_count) 
                {
                    G[op] = -1;
                    // cout<<op<<" : "<<ip<<endl; 
                }//Grant to first avilable request
                else G[op] = ip;

                op++;
            }
            for(int p=0;p<1;p++)
            {
                if(checker1==true)
                {
                    checker1=false;
                }
                else if(checker1==false)
                {
                    checker1=true;
                }
            }
            //Accept-phase
            // cout<<"Accepted connections : "<<endl;
            for (int ip = 0; ip < switch_port_count; ip++) {
                if (ip_flag[ip]) continue; //Check if input-port is busy
                int i = 0, op = a_p[ip];
                for (; i < switch_port_count; i++) { //Check for first port that granted request
                    if (G[op] == ip) break; //Break if Grant found
                    op = (op + 1) % switch_port_count;
                }
                if (i < switch_port_count) {
                    no_transmitted_packet++; //Count no.of packets transmitted
                    // cout<<ip<<" : "<<op<<endl;
                    op_flag[op] = true; // Mark outport as established connection
                    ip_flag[ip] = true; // Mark inputport as establishe connection
                    packet p = ip_q[ip][op].front(); //store the front packet in the queue
                    p.comp_time = t + 1; //store completion time
                    delay += (int) p.comp_time - (int) p.gen_time; //calculate total delay till now
                    delay_arr.push_back((int) p.comp_time - (int) p.gen_time); //maintain delay of each packet
                    ip_q[ip][op].pop(); //remove packet from queue
                    for(int p=0;p<1;p++)
                    {
                        if(checker1==true)
                        {
                            checker1=false;
                        }
                        else if(checker1==false)
                        {
                            checker1=true;
                        }
                    }
                    op++;
                    ip++;
                    a_p[ip] = (op)%switch_port_count;
                    g_p[op] = (ip)%switch_port_count;
                    
                }
            }
    }
    float Average_delay = delay / (no_transmitted_packet * 1.0); //Calculate average delay
    // Calculate Standard-Deviation
    float stand_dev = 0;
    stand_dev=calculateStandardDeviation(delay_arr,Average_delay);
    // calculate link utilisation
    float link_utiliization = no_transmitted_packet / (switch_port_count * max_time_slots * 1.0);
    ofstream fout;
    fout.open(out_file, ios::app);
    //float link_utiliization = sum / (switch_port_count * max_time_slots * 1.0);
    //cout << "N\tp\tQueueType\tavgPD\t\tstd_dev_pd\tavg_link_util\n";
    // cout<<no_gen_pack<<"\t"<<no_transmitted_packet<<endl;
    fout << switch_port_count << "\t\t" << packet_gen_prob << "\t\t" << queue_type << "\t\t" << Average_delay << "\t\t" << stand_dev << "\t\t" << link_utiliization<<"\n";
    cout << "\n" << switch_port_count << "\t" << packet_gen_prob << "\t" << queue_type << "\t\t" << Average_delay << "\t\t" << stand_dev << "\t\t" << link_utiliization;
    fout.close();
}

int main(int argc, char ** argv) {
    //  cout<<"IN MAIN";
    // ./routing −N switchportcount −B buffersize −p packetgenprob −queue INQ −K knockout −out outputfile −T maxtimeslots 

    // parsing input 
    if (argc <= 15) {
        for (int i = 1; i < argc; i += 2) {
            if (strcmp(argv[i], "-N") == 0) {
                switch_port_count = atoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-B") == 0) {
                buffer_size = atoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-p") == 0) {
                packet_gen_prob = stof(argv[i + 1]);
            } else if (strcmp(argv[i], "-queue") == 0) {
                int j = 0;
                do {
                    queue_type[j] = (argv[i + 1])[j], j++;
                }
                while ((argv[i + 1])[j - 1] != '\0');
            } else if (strcmp(argv[i], "-k") == 0) {
                k_knockout = stof(argv[i + 1]);
            } else if (strcmp(argv[i], "-out") == 0) {
                out_file = argv[i + 1];
            } else if (strcmp(argv[i], "-T") == 0) {
                max_time_slots = stoi(argv[i + 1]);
            } else {
                cout << "Wrong Input Type" << endl;
                return 0;
            }

        }

        if (strcmp("INQ", queue_type) == 0) {
            INQ();
        } else if (strcmp("KOUQ", queue_type) == 0) {
            KOUQ();
        } else if (strcmp("ISLIP", queue_type) == 0) {
            Islip();
        } else {
            cout << "Queue can be only INQ / KOUQ / ISLIP";
            cout << "\nProgram is terminating";
            return 0;
        }
        for(int p=0;p<1;p++)
        {
            if(checker1==true)
            {
                checker1=false;
            }
            else if(checker1==false)
            {
                checker1=true;
            }
        }

    } else {
        cout << "::Please provide proper Arguments::" << endl;
        cout << "output must be of form:- \n./routing N switchportcount B buffersize p packetgenprob queue INQ K knockout out outputfile T maxtimeslots ";
        cout << "\nProgram is Terminating...";

    }

    return 0;
}