/* to compile

   g++ rowCut.cpp -lz -O3 -o rowCut


*/

//version 6
#include <iostream>
#include <fstream> //used for ifstream
#include <cstdio> //used for printf
#include <cstring> //used for strtok strcmp
#include <map>
#include <deque>
#include <utility> //used for make_pair
#include <cstdlib> //used atoi
#include <zlib.h>

// INSPIRED BY getliner from:
// https://github.com/aalbrechtsen/getliner

size_t LENS = 1000000; //buffer length


std::deque<int> getIndexes(char *input){
  std::deque<int> ret;
  char* tmp = strtok(input,",");
  ret.push_back(atoi(tmp));
  int tmpint = atoi(tmp);
  tmp = strtok(NULL,",");
  
  while(tmp!=NULL){
    ret.push_back(atoi(tmp));
    if(tmpint>=atoi(tmp)){
      fprintf(stderr,"rows have to be in increasing order\n");
      exit(1);
    }    
    tmp = strtok(NULL,",");
  }
  fprintf(stderr,"number of rows to extract=%lu\n",ret.size());
  return ret;
}





int main(int argc, char *argv[]){
  if(argc==1){
    fprintf(stderr,"\tNeeded: -d delimter -r rows  -f infile \n\n");
    fprintf(stderr,"\toptional: -b bufferSize (for long lines)  \n\n");
    fprintf(stderr,"\texample1: to extract row 1,2,3 in file: \'infile\'\n");
    fprintf(stderr,"\t\t./getliners  -r 1,2,3 -f infile \n");
    
    return 0;
  }

  int argPos = 1;
  const char *delims = " \t\n\r";
  char* rows = NULL;
  const char* datafile = NULL;
  int doCompl =0;
  while(argPos <argc){
    if(strcmp(argv[argPos],"-d")==0)
      delims = strdup(argv[argPos+1]);
    else if(strcmp(argv[argPos],"-r")==0)
      rows = argv[argPos+1];
    else if(strcmp(argv[argPos],"-f")==0)
      datafile = argv[argPos+1];
   else if(strcmp(argv[argPos],"-b")==0)
     LENS = atoi(argv[argPos+1]);
  
   else {
      printf("\tUnknown arguments: %s\n",argv[argPos]);
      printf("USE -d delimter -r rows  -f infile\n");
      return 0;
    }
    argPos+=2;
  }

  std::deque<int> vec;//used for linenumbers

  vec = getIndexes(rows);    
  //printMap(asso);

  gzFile gz = Z_NULL;
  gz = gzopen(datafile,"r");
  if(gz==Z_NULL){
    fprintf(stderr,"Problems opening file:%s\n",datafile);
    return 0;
  }
  char buffer[LENS];
  char *original = new char[LENS];

  //fprintf(stderr,"Using lines file for extracting lines\n");


  int curLine = 1;
  int nextLine = vec.front();
  vec.pop_front();

  while(gzgets(gz,buffer,LENS)){   
    if(nextLine==curLine){      
      char *tok = strtok(buffer,"\r\t\n ");
      fprintf(stdout,"%s",tok);
      while((tok = strtok(NULL,"\r\t\n "))){
	fprintf(stdout," %s",tok);
      }
      fprintf(stdout,"\n");                  
      nextLine = vec.front();
      vec.pop_front();
    } 
    curLine++;
  }
  
  
  return 0;
}
