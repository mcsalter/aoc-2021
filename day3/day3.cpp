#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>


// day 3 -- binary diagnostics

#define numsize 12

int readfile(char* argv, std::string *fileMemory) {
  std::ifstream file(argv, std::ios::in|std::ios::binary|std::ios::ate);
  if(file.is_open()){

    // taken from cplusplus.com, as I have not messed with binary files before:
    std::streampos size = file.tellg();
    fileMemory->resize(size, '0');
    file.seekg(0, std::ios::beg);
    file.read(fileMemory->data(), size);
    file.close();
    // if(verbose){
    // 	std::cout << "read file" << std::endl;
    //      }
  } 
  else{
    std::cerr << "err: file cannot be opened: " << argv[1] << std::endl;
    return -1;
    }
  return 0;
}



int get_power(std::string fileMemory, bool verbose){
  // just store how many of each there are... no need to store the entire list.
  int ones[numsize] = {0};
  int zeros[numsize] = {0};
  // adding up the ones/zeros for each col
    int itr = 0;
    for(char val : fileMemory){
      if(itr == numsize){
	itr = 0;
	//if(verbose){std::cout << std::endl;}
      }
      if(val == '0'){
	zeros[itr] ++;
	//	if(verbose){std::cout << "0";}
	} else if(val == '1'){
	ones[itr] ++;
	//if(verbose){std::cout << "1";}
      }else if(val == '\n'){
	continue;
      }
      else
	{
	std::cerr << "err: unknown character: " << val << std::endl;
	return -1;
      }
      itr ++;
    }

    // calculate gama
    std::bitset<numsize> gama = 0;
    for(itr = 0; itr < numsize; itr++){
      if(ones[itr] > zeros[itr]){
	gama[numsize - itr - 1] = 1;
      }
      else if(ones[itr] < zeros[itr]){
	gama[numsize - itr - 1] = 0;
      }

      else{
	return -1;
      }
      if(verbose){std::cout << ones[itr] << " " << zeros[itr] << std::endl;}
    }


    //calculate epsilon
    std::bitset<numsize> epsilon = 0;
    for(itr = 0; itr < numsize; itr++){
      if(gama[itr] == 0){ epsilon[itr] = 1;}
      else if(gama[itr] == 1){ epsilon[itr] = 0;}
      else{
	return -1;
      }
    }
    
    std::cout << gama << std::endl << epsilon << std::endl;
    ulong power = gama.to_ulong() * epsilon.to_ulong();
    std::cout << power << std::endl;

    
    return 0;
}

int get_most_common_for_bit_offset(std::vector<std::bitset<numsize>> list, int offset,
				   std::vector<std::bitset<numsize>> &most_common,
				   bool verbose, bool most_or_least){
  
  std::vector<std::bitset<numsize>> zeros;
  std::vector<std::bitset<numsize>> ones;
  //first pool the vector values into two pools and calculate the size of each
  for (std::bitset<numsize> itr : list){
    if(itr[offset] == 0){
      zeros.push_back(itr);
    }else if (itr[offset] == 1){
      ones.push_back(itr);
    }else {
      std::cerr << "err: character unknown: " << itr[offset] << std::endl;
      return -1;
    }
  }

  //defining most value as true, least as false
  if(most_or_least == true){
    // time to sum up how many there are, and return the indicated value
    if(zeros.size() > ones.size()){
      most_common = zeros;
      if(verbose){std::cout << "zero"<< std::endl;}
    } else if(zeros.size() <= ones.size()){
      most_common = ones;
      if(verbose){std::cout << "one"<< std::endl;}
    } else {
      std::cerr << "err: get_most_common_for_bit_offset failure" << std::endl;
      return -1;
    }
  } else if(most_or_least == false){
    // time to sum up how many there are, and return the indicated value
    if(zeros.size() > ones.size()){
      most_common = ones;
      if(verbose){std::cout << "zero"<< std::endl;}
    } else if(zeros.size() <= ones.size()){
      most_common = zeros;
      if(verbose){std::cout << "one"<< std::endl;}
    } else {
      std::cerr << "err: get_most_common_for_bit_offset failure" << std::endl;
      return -1;
    }
  }
  return 1;
    
}

int get_lifesupport(std::string fileMemory, bool verbose){    
  std::vector<std::bitset<numsize>> args;
  
  std::bitset<numsize> number;
  std::stringstream iss(fileMemory);
  while(iss >> number){
    args.push_back(number);
    if(verbose){std::cout << "read: " << number << std::endl;}
  }

  std::vector<std::bitset<numsize>> holder;
  holder = args;
  for(int offset = numsize - 1; offset > -1; offset--){
    int retval = get_most_common_for_bit_offset(holder, offset, holder, false , true);
    if(retval == -1){
      return -1;
    }
  }
  long o2_gen = holder[0].to_ulong();

  holder = args;
  for(int offset = numsize - 1; offset > -1; offset--){
    int retval = get_most_common_for_bit_offset(holder, offset, holder, verbose, false);
    if(retval == -1){
      return -1;
    }
  }
  long o2_scrubber = holder[0].to_ulong();
  
  std::cout << o2_gen << " " << o2_scrubber << std::endl;
 
  
  return 1;
}


int main(int argc, char *argv[]){
  std::string fileMemory;
  
  if(argc < 2){
    std::cerr << "err: no input file" << std::endl;
    return -1;
  }

  else {
    bool verbose = false;

    if(argc >= 3)
      {
	std::string temp = argv[2];
	if(temp == "-v"){
	  verbose = true;
	}
      }
    int retval = readfile(argv[1], &fileMemory);
    if(retval == -1){
      return -1;
    }

    //int power = get_power(fileMemory, verbose);
    //if(power == -1){
    //  return -1;
    //}
    
    int life = get_lifesupport(fileMemory, verbose);
    if(life == -1){
      return -1;
    }

    return 0;
  }
}
