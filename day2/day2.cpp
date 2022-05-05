#include <iostream>
#include <fstream>
#include <string>

// DIVE!


// solution for part 1:
// int main(int argc, char *argv[]){
//   if ((argc < 2)){
//     std::cerr << "err: either too many, or to few arguments" << std::endl;
//   }
//   bool verbose = false;
//   if (argc > 2){
//     std::string val = argv[2];
//       if(val == "-v"){
//     verbose = true;
//     }
//   }
//   int horiz = 0;
//   int verti = 0;
  
//   std::string command;
//   std::string intensity;
//   std::ifstream inputfile (argv[1]);
//   while(getline(inputfile, command, ' ')){
//     getline(inputfile, intensity);
//     if(verbose){
//       std::cout << command << std::endl << intensity << std::endl;
//     }
//     if(command ==  "forward"){
//       horiz += stoi(intensity);
//       if(verbose){
// 	std::cout << "hor: " << horiz << std::endl;
//       }
//     }
//     else if(command == "down"){
//       verti += stoi(intensity);
//       if(verbose){
// 	std::cout << "ver: " << intensity << std::endl;
//       }
//     }
//     else if(command ==  "up"){
//       verti -= stoi(intensity);
//       if(verbose){
// 	std::cout << "ver: " << intensity << std::endl;
//       }
//     }
//     else{
//       std::cerr << "err: command not found: " << command << std::endl;
//       return -1;
//     }
//     command == "";
//   }
//   inputfile.close();
//   std::cout << horiz * verti << std::endl;
// }

int main(int argc, char *argv[]){
  if ((argc < 2)){
    std::cerr << "err: either too many, or to few arguments" << std::endl;
  }
  bool verbose = false;
  if (argc > 2){
    std::string val = argv[2];
      if(val == "-v"){
    verbose = true;
    }
  }
  int horiz = 0;
  int depth = 0;
  int aim = 0;
  
  std::string command;
  std::string intensity;
  std::ifstream inputfile (argv[1]);
  while(getline(inputfile, command, ' ')){
    getline(inputfile, intensity);
    if(verbose){
      std::cout << command << std::endl << intensity << std::endl;
    }
    if(command ==  "forward"){
      horiz += stoi(intensity);
      int temp = aim * stoi(intensity);
      depth += temp;
      if(verbose){
	std::cout << "hor: " << horiz << std::endl;
	std::cout << "ver: " << depth << std::endl;
      }
    }
    else if(command == "down"){
     aim += stoi(intensity);
      if(verbose){
	std::cout << "aim " << intensity << std::endl;
      }
    }
    else if(command ==  "up"){
     aim -= stoi(intensity);
      if(verbose){
	std::cout << "aim " << intensity << std::endl;
      }
    }
    else{
      std::cerr << "err: command not found: " << command << std::endl;
      return -1;
    }
    command == "";
  }
  inputfile.close();
  std::cout << horiz * depth << std::endl;
}
