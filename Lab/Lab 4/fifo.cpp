#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void readfile (int* pagenum, int* framenum, std::vector<int> &refString)
{
  std::string mix;
  std::string refstr;
  std::ifstream myReadFile;
  myReadFile.open("input.txt");
  getline(myReadFile, mix);
  getline(myReadFile, refstr);
  myReadFile.close();

  std::vector<int> firstln;
  std::vector<int> secondln;
  std::istringstream smix(mix);
  std::istringstream srefstr(refstr);

  int temp1;
  while(smix >> temp1)
    {
      firstln.push_back(temp1);
    }
  int temp2;
  while(srefstr >> temp2)
    {
      if (temp2 != -1)
       {
         secondln.push_back(temp2);
       }
    }
  *pagenum = firstln[0];
  *framenum = firstln[1];
  refString = secondln;
}

void fifo (int pagenum, int framenum, std::vector<int> &refString, std::vector<int> &fault, std::vector<int> &output)
{
  int size = framenum * refString.size();
  for(int i = 0; i < size; i++)
    {
      output.push_back(-1);
    }

  int flag = 1; //==0 exist; ==1 not exist
  int counter = -1; //count idx of refString
  for(int i = 0; i < (size-(framenum-1)); i+=framenum)
    {//1
      counter++;
      //Suspect if it already exist
      flag = 1;
      for(int j = i; j < (i+framenum); j++)
        {
          if(refString[counter] == output[j])
           {
             flag = 0;
           }
        }

      if(flag == 1)
       {
         for(int j = (framenum-1); j>=0; j--)
           {
             for(int k = (i+j); k < (size-(framenum-j-1)); k+=framenum)
              {
                if(j == 0)
                 {
                   output[k] = refString[counter];
                 }
                else
                 {
                   output[k] = output[k-1];
                 }
              }
           }
           fault.push_back(1); //1 represent p; 0 represent space
       }
      if(flag == 0)
       {
         fault.push_back(0); //1 represent p; 0 represent space
       }
    }//1
}//fifo end

void writefile (int pagenum, int framenum, std::vector<int> &refString, std::vector<int> &fault, std::vector<int> &output)
{
  std::ofstream myfile;
  myfile.open ("FIFO_out.txt");

  myfile << "Number of pages: " << pagenum << "\n";
  myfile << "Number of frames: " << framenum << "\n";
  myfile << "Size of the reference string: " << refString.size() << "\n";
  myfile << "Reference String:\n";
  for(int i = 0; i < refString.size(); i++)
    {
      myfile << refString[i] << " ";
    }
  myfile << "\n";
  myfile << " \n";

  myfile << "FIFO\n";
  for(int i = 0; i < refString.size(); i++)
    {
      myfile << refString[i] << "  ";
    }
  myfile << "\n";
  myfile << "-------------------------------------------------------------\n";

  for(int i = 0; i < framenum; i++)
    {
      for(int j = i; j < (output.size()-(framenum-i-1)); j+=framenum)
        {
          myfile << output[j] << "  ";
        }
      myfile << "\n";
    }

  int faultcounter = 0;
  for(int i = 0; i < fault.size(); i++)
    {
      if(fault[i] == 1)
        {
          faultcounter++;
          myfile << "P" << "  ";
        }
      else
       {
         myfile << " " << "  ";
       }
    }
  myfile << "\n";
  myfile << "-------------------------------------------------------------\n";
  myfile << faultcounter << " page-faults\n";
  myfile.close();
}

int main()
{
  int page;
  int frame;
  std::vector<int> refString;
  std::vector<int> fault;
  std::vector<int> output;

  readfile (&page, &frame, refString);
  fifo (page, frame, refString, fault, output);
  writefile (page, frame, refString, fault, output);
  return 0;
}
