#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]){
  int depths = 0;
  bool first_pass = true;
  string value = "empty";
  int last_num = 0;
  //int endval = argc;
  int endval = argc - 2;
  for(int itr = 1; itr < endval; itr++){
    int curr_num = atoi(argv[itr]);
    curr_num += atoi(argv[itr + 1]) + atoi(argv[itr + 2]);
    if(first_pass){
      value = "N/A - no previous measurement";
      first_pass = false;
    }
    else{
      if(last_num < curr_num){
	value = "increased";
	depths +=1;
	  }
      else if(last_num > curr_num){
	value = "decreased";
      }
      else{
	value = "equal";
      }
    }
    
    // cout << curr_num << "("<< value << ")" << endl;
    last_num = curr_num;
  }
  cerr << depths << endl;
  return 0;
}
