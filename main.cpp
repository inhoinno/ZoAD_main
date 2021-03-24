/*  main.cpp */

#include <iostream>
#include <string>
#include "zoad.h"
#include <signal.h>
//void* newthread_operation(){}


//std::vector<std::vector <float>>


int main(int argc, char** argv[]){

	std::cout << "ZoAD init" <<std::endl;
	std::cout << "Custom Workload for ZoAD" << std::endl;

	zoad_init();

	int i = 0;
	int w_it=0;
	int ret=0;
	int test_lp_count = 5;
	unsigned int t = 0;
	unsigned int uid[test_lp_count] = {0, 1, 2, 3, 4};		//5 uid 
	unsigned int iolevel[test_lp_count] = {4, 4, 8, 8, 8};	//32 iolevel 
	unsigned int new_uid = 4;
	unsigned int new_iolevel = 4;
	

	struct timeval start ={};
	struct timeval end ={};
	double time = 0.0;
	double perf = 0.0;
	double totalperf= 0.0;
	std::vector <double> time_logs;
	std::vector <std::string> lp_names;
	std::vector <unsigned int> lp_ids;
	std::string lp_name = "inhoinno_qos_a";
	char ch = 'b';

	
	/*	
	//mklp
	ret=z_mklp(999, "dev/nvme0n1", "iolevel1_Test", QOS_PARTITION, 1);
	if(ret<=0)
		std::cout<<"z_mklp fail uid : 999"<< std::endl;
	else{
		std::cout<< "made uid : " << "999" << " partition id: "<<ret<<std::endl;
	}
	int test_id = ret;
	//open
	ret = z_openlp(999, "iolevel1_Test", Z_ASYNC);
	if (ret < 0)
		std::cout<< "z_open fail" <<std::endl;
	else 
		std::cout<< "open" << std::endl;

	unsigned int test_size = 800*1024*1024; // 800MB
	char * test_buf = (char*)malloc(test_size);
	memset(test_buf, 'T', test_size);
	//flush
	//raise(SIGTRAP);
	
	gettimeofday(&start, NULL);
	for(w_it = 0; w_it <1 ; w_it++){
//		gettimeofday(&start,NULL);
		z_flush(999, test_id, "dummy", test_buf, test_size, Z_APPEND); 
		z_sync(999, test_id, "dummy");
//		gettimeofday(&end,NULL);
//		time += (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*0.000001);
//		perf = test_size / time*1024*1024; // MB/s
//		std::cout<< "p : " << perf << "(MB/s)"<<std::endl;
	}
	gettimeofday(&end, NULL);
	time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)*0.000001);	
	totalperf = (test_size*w_it) /(time*1024*1024);
	std::cout << "tp : "<< totalperf << "(MB/s)"<< w_it <<std::endl;

	//close
	z_closelp(999, test_id);
//	time = (eend.tv_sec - sstart.tv_sec) + ((eend.tv_usec - sstart.tv_usec)*0.000001);
//	totalperf = (test_size * w_it) / (time*1024*1024); //(MB/s)
//	std::cout << "tp : " << totalperf<< "(MB/s)"<<std::endl; 
	//rmlp
	z_rmlp(999,"dev/nvme0n1",test_id);

	std::cout << "IOlevel:1 Fin" <<std::endl;
		
	
	*/

	
	// * z_mklp() * 4 
	for (i =0; i < test_lp_count; i++){
		lp_names.push_back(lp_name);
		ret = z_mklp(uid[i],"dev/nvme0n1", lp_name, QOS_PARTITION, iolevel[i]);
		lp_name.pop_back(); 
		lp_name.push_back(ch);
		ch++;
		//lp_name.append(ch);	ch++;

		if (ret <= 0)
			std::cout << "z_mklp fail uid : " << uid[i] << std::endl;
		else{ 
			std::cout << "made uid : " << uid[i] << "partition id : "<< ret << std::endl; 
			lp_ids.push_back(ret);
		} 
	}

	for(i =0; i< test_lp_count; i++){
		std::cout<< "lp_name : " << lp_names.at(i) << std::endl;
	}
	std::cout << "z_mklp fin" <<std::endl;

	// z_openlp() * 4 
	for (i =0; i<test_lp_count; i++){
		ret = z_openlp(uid[i], lp_names.at(i), Z_ASYNC);//SYNC OPEN -> GET PERF
		if (ret < 0)
			std::cout << "z_open fail" << std::endl;
		else 
			std::cout << "open " << ret << std::endl;	//partition id
			if( lp_ids[i] != ret)
				std::cout << "LP id doesnt match" << std::endl; 
	}
	
	unsigned int write_size= 1024*1024*1024; //72MB*20 = 1440MB 
	char * buf = (char*)malloc(write_size);
	char * buf2 = (char*)malloc(write_size);
	memset(buf, 'a', write_size);
	memset(buf2, 'B', write_size);

	std::cout << "z_openlp fin" << std::endl;

	// z_flush() *4 + new thread

	//raise(SIGTRAP);

	for(w_it =0; w_it < 1; w_it++){
		//if(j != 0){
			//re-open
			//for(int k=0; k< test_lp_count; k++){
				//raise(SIGTRAP);
				//ret = z_openlp(uid[k], lp_names.at(k), Z_ASYNC);
				//if (ret < 0)
					//std::cout << "z_open fail" << std::endl;
				//else 
					//std::cout << "open " << ret << std::endl;	//partition id
				//if(lp_ids[k] != ret)
					//std::cout << "LP id doesnt match" << std::endl; 					
			//}
			
			
		//}
			
		for (int i = 0 ; i < test_lp_count; i++){
			//START
			gettimeofday(&start,NULL);
			z_flush(uid[i], lp_ids[i], "dummy", buf, write_size, Z_APPEND); 
			z_sync(uid[i], lp_ids[i], "dummy");
			gettimeofday(&end, NULL);
			// END  
			time = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec)* 0.000001);
			std::cout << "time : " <<time<<std::endl;
			time_logs.push_back(time);
			//std::cout<< buf2[0]<<time << std::endl;
					
			//std::cout << "flush "<< std::endl;	
			//if(w_it % 300 == 0){
				// ret = z_mklp(new_uid)
				// ret = z_openlp(new_uid)
				// if mklp or z_openlp failed , continue;
				// New User LP came to write
				// START 
				//z_flush(new_uid, new_lp_id, "dummy", buf2, (write_size*384*8), Z_APPEND);
				// END 
				//new_uid++;
			//}
		}	//for End
	
		
		//re-close
		//for(int k=0; k < test_lp_count; k++){
			//ret = z_closelp(uid[k], lp_ids[k]);
		//}
	}//for End
	std::cout<< "flush fin" <<std::endl;
	
	for(int it=0; it< test_lp_count; it++){
		z_closelp(uid[it], lp_ids[it]);
	}
	
	for(int it=0; it< test_lp_count; it++)
		z_rmlp(uid[it],"/dev/nvme0n1",lp_ids[it]);
	
	delete buf;
	delete buf2;
	
	std::cout<< "Custom FIN" <<std::endl;
	int time_logs_size = time_logs.size();
	for(int i = 0; i<time_logs_size; i++){
		totalperf= (double)(write_size)/((time_logs.at(i))*1024*1024);
		std::cout<<"iolevel : "<<iolevel[i]<<" time "<< time_logs.at(i) <<" pf : "<< totalperf<<"(MB/s)"<<w_it << std::endl;
	}


}
