#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <tgmath.h>
#include <iostream>
#include <fstream>
#include <algorithm>

void insert(int* arr, int pos_ini, int counter)
{
  for(int i = pos_ini; i < (pos_ini + counter); i++)
   {
     //std::cout << "Filling position " << i << "\n";
     arr[i] = 8;
   }
}
//**************************** Begin First*********************************//
void allocate_first(int* arr, std::vector<int> &list_allocation)
{
  std::cout << "Entering allocate_first\n";
  int lower = 1024;
  int upper = 25600;
  int each_time = 0;

  int counter = 0;
  int check = 0;

  std::vector<int> pos_initial;

  while (check == 0)
   {
     check = 1;
     each_time = (rand() % (upper - lower + 1)) + lower;//each_time = random number between 4KB and 100KB

     for(int i = 0; i < 262144; i++)
      {
        if (arr[i] == 0)
         {
           pos_initial.push_back(i); //Mark the index where first hole show up
           counter++; // Keep counting the size of the hole
           if(each_time <= counter)
            {
              list_allocation.push_back(pos_initial[0]);
              list_allocation.push_back(pos_initial[0]+each_time-1);
              insert(arr, pos_initial[0], each_time);
              pos_initial.clear();
              pos_initial.resize(0);
              counter = 0;
              check = 0;
              break;//break for
            }
         }
        else
         {
           pos_initial.clear();
           pos_initial.resize(0);
          counter = 0;
         }
       }//end for
   }//end while
   std::cout << "Exiting allocate_first\n";
}
//**************************** End First*********************************//
//**************************** Begin Best*********************************//
void allocate_best(int* arr, std::vector<int> &list_allocation)
{
  std::cout << "Entering allocate_best\n";
  int lower = 1024;
  int upper = 25600;
  int each_time = 0;

  int check = 0;
  int counter = 0;
  std::vector<int> pos_initial;
  std::vector<int> temp;//To save the scan result

  while (check == 0)
   {
     check = 1;
     each_time = (rand() % (upper - lower + 1)) + lower;
     //Start scanning, store all the holes in temp
     for(int i = 0; i < 262144; i++)
      {
        if(arr[i] == 0)
         {
           pos_initial.push_back(i);
           counter++;
         }
        else
         {
           if(counter != 0)
            {
              temp.push_back(pos_initial[0]);
              temp.push_back(pos_initial[0]+counter-1);
              temp.push_back(counter);
              pos_initial.clear();
              pos_initial.resize(0);
              counter = 0;
              continue;
            }
         }
        if((arr[i] == 0)&&(i == 262143))
         {
           temp.push_back(pos_initial[0]);
           temp.push_back(pos_initial[0]+counter-1);
           temp.push_back(counter);
         }
      }//end for
      pos_initial.clear();
      pos_initial.resize(0);
      counter = 0;
      //Start search for the closest fit
      int temp_size = temp.size();
      int bestfit = 262144;
      int bestidx = 0;
      for(int i = 2; i <= (temp_size - 1); i+=3)
       {
         if((each_time <= temp[i]) && (temp[i] <= bestfit))
          {
            bestfit = temp[i];
            bestidx = i;
            check = 0;
          }
       }
      //Start inserting
      if(check == 0)
       {
         list_allocation.push_back(temp[bestidx-2]);//store start
         list_allocation.push_back((temp[bestidx-2]+each_time-1));//store end
         insert(arr, temp[bestidx-2], each_time);
       }
       temp.clear();
       temp.resize(0);
   }//end while
   std::cout << "Exiting allocate_best\n";
}
//**************************** End Best*********************************//
//**************************** Begin Worst*********************************//
void allocate_worst(int* arr, std::vector<int> &list_allocation)
{
  std::cout << "Entering allocate_worst\n";
  int lower = 1024;
  int upper = 25600;
  int each_time = 0;

  int check = 0;
  int counter = 0;
  std::vector<int> pos_initial;
  std::vector<int> temp;//To save the scan result

  while (check == 0)
   {
     check = 1;
     each_time = (rand() % (upper - lower + 1)) + lower;
     //Start scanning, store all the holes in temp
     for(int i = 0; i < 262144; i++)
      {
        if(arr[i] == 0)
         {
           pos_initial.push_back(i);
           counter++;
         }
        else
         {
           if(counter != 0)
            {
              temp.push_back(pos_initial[0]);
              temp.push_back(pos_initial[0]+counter-1);
              temp.push_back(counter);
              pos_initial.clear();
              pos_initial.resize(0);
              counter = 0;
              continue;
            }
         }
        if((arr[i] == 0)&&(i == 262143))
         {
           temp.push_back(pos_initial[0]);
           temp.push_back(pos_initial[0]+counter-1);
           temp.push_back(counter);
         }
      }//end for
      pos_initial.clear();
      pos_initial.resize(0);
      counter = 0;
      //Start search for the largest fit
      int temp_size = temp.size();
      int worstfit = 0;
      int worstidx = 0;
      for(int i = 2; i <= (temp_size - 1); i+=3)
        {
          if((each_time <= temp[i])&&(worstfit <= temp[i]))
           {
             worstfit = temp[i];
             worstidx = i;
             check = 0;
           }
        }
      //Start inserting
      if(check == 0)
       {
         list_allocation.push_back(temp[worstidx-2]);//store start
         list_allocation.push_back((temp[worstidx-2]+each_time-1));//store end
         insert(arr, temp[worstidx-2], each_time);
       }
       temp.clear();
       temp.resize(0);
   }//end while
   std::cout << "Exiting allocate_worst\n";
}
//**************************** End Worst*********************************//
void release(int* arr, std::vector<int> &list_allocation)
{
  std::cout << "Entering release\n";
  float totalprocess = (list_allocation.size())/2; //Total processes allocated
  float processtoremove = round(totalprocess/10); //number of processes to remove

  int lower = 1;
  int upper = totalprocess;
  int number = 0;
  int start = 0;
  int end = 0;

  std::vector<int> avoid_dup;
  avoid_dup.push_back(0);
  std::vector<int>::iterator temp;

  std::cout << "Total processes = " << totalprocess << "\n";
  std::cout << "Total process to remove = " << processtoremove << "\n";
  for(int i = 0; i < processtoremove; i++)
   {
     while(1)
      {
        number = (rand() % (upper - lower + 1)) + lower; //which process to remove (it's a random number)
        temp = find (avoid_dup.begin(), avoid_dup.end(), number);
        if(temp == avoid_dup.end())
          {avoid_dup.push_back(number); break;}
      }
     std::cout << "Removing process " << number << "\n";
     start = number*2-2;
     end = number*2-1;
     std::cout << "Removing from poistion " << list_allocation[start] << " to position "<< list_allocation[end] << "\n";
     for(int j = list_allocation[start]; j <= list_allocation[end]; j++)
      {
        arr[j] = 0;
      }
   }
   std::cout << "Exiting release\n";
}

void compaction(int* arr, std::vector<int> &list_allocation)
{
  int arr_size = 262144;
  int count_fill = 0;
  //Count the total numbers of filled locations
  for(int i = 0; i <= (list_allocation.size() - 2); i+=2)
    {
      count_fill = (count_fill + (list_allocation[i+1] - list_allocation[i] + 1));
    }
  //Compaction
  for(int i = 0; i < 262144; i++)
    {
      if(i < count_fill)
        {arr[i] = 8;}
      else
        {arr[i] = 0;}
    }
}

void status(int* arr, std::vector<int> &holes,  std::vector<int> &filled)
{
  std::vector<int> hole_initial;
  int hole_counter = 0;

  std::vector<int> fill_initial;
  int fill_counter = 0;
  int total_fill = 0;
  //Search and store hole & filled location
  for(int i = 0; i < 262144; i++)
   {
     if(arr[i] == 0)
      {
        hole_initial.push_back(i);
        hole_counter++;
        if(fill_counter != 0)
         {
           filled.push_back(fill_initial[0]);
           filled.push_back(fill_initial[0]+fill_counter-1);
           fill_counter = 0;
           fill_initial.clear();
           fill_initial.resize(0);
         }
      }
     else
      {
        fill_initial.push_back(i);
        fill_counter++;

        if(hole_counter != 0)
         {
           holes.push_back(hole_initial[0]);
           holes.push_back(hole_initial[0]+hole_counter-1);
           hole_counter = 0;
           hole_initial.clear();
           hole_initial.resize(0);
         }
      }
    }//end for

    //Print holes status
    int hole_arr_size = holes.size();
    for(int i = 0; i <= (hole_arr_size-2); i+=2)
     {
       std::cout << "Hole from position " << holes[i] << " to position " << holes[i+1] << "\n";
     }

    //Print filled status
    int fill_arr_size = filled.size();
    for(int i = 0; i <= (fill_arr_size-2); i+=2)
     {
       total_fill = total_fill + (filled[i+1] - filled[i] + 1);
       std::cout << "Filled space from position " << filled[i] << " to position " << filled[i+1] << "\n";
     }

     int convert_byte = total_fill*4;
     std::cout << "There are  " << total_fill << " / " << "262144" << " space filled\n";
     std::cout << "Represent in Bytes, there are  " << convert_byte << "Bytes / " << "1048576Bytes(1MB)" << " space filled\n";
}


int main()
{
  int n = 262144; // 1048576/4 = 262144

  // First Fit
  std::cout << "------------------- Printing First Fit -------------------\n";
  int* memo1 = (int*) calloc(n, sizeof(int)); //initialize 1MB memory
  std::cout << "******* Initial filling *******\n";
  std::vector<int> list_allocation;
  std::vector<int> holes;
  std::vector<int> filled;
  allocate_first(memo1, list_allocation);
  release(memo1, list_allocation);
  status(memo1, holes, filled);
  int* memo1_dup = (int*) calloc(n, sizeof(int));
  std::vector<int> list_allocation_dup;
  for(int i = 0; i < n; i++)
    {memo1_dup[i] = memo1[i];}
  for(int i = 0; i < list_allocation.size(); i++)
    {
      list_allocation_dup.push_back(list_allocation[i]);
    }
  //For non-compaction
  std::cout << "******* Non-compaction *******\n";
  list_allocation.clear();
  list_allocation.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  allocate_first(memo1, list_allocation);
  status(memo1, holes, filled);
  //For compaction
  std::cout << "******* Compaction *******\n";
  compaction(memo1_dup, list_allocation_dup);
  list_allocation.clear();
  list_allocation.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  allocate_first(memo1_dup, list_allocation_dup);
  status(memo1_dup, holes, filled);
  list_allocation.clear();
  list_allocation.resize(0);
  list_allocation_dup.clear();
  list_allocation_dup.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  free(memo1);
  free(memo1_dup);

  std::cout << "------------------- Printing Best Fit -------------------\n";
  int* memo2 = (int*) calloc(n, sizeof(int)); //initialize 1MB memory
  // Best Fit
  std::cout << "******* Initial filling *******\n";
  allocate_best(memo2, list_allocation);
  release(memo2, list_allocation);
  status(memo2, holes, filled);
  int* memo2_dup = (int*) calloc(n, sizeof(int));
  for(int i = 0; i < n; i++)
    {memo2_dup[i] = memo2[i];}
  for(int i = 0; i < list_allocation.size(); i++)
    {
      list_allocation_dup.push_back(list_allocation[i]);
    }
  //For non-compaction
  std::cout << "******* Non-compaction *******\n";
  list_allocation.clear();
  list_allocation.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  allocate_best(memo2, list_allocation);
  status(memo2, holes, filled);
  //For compaction
  std::cout << "******* Compaction *******\n";
  compaction(memo2_dup, list_allocation_dup);
  list_allocation.clear();
  list_allocation.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  allocate_best(memo2_dup, list_allocation_dup);
  status(memo2_dup, holes, filled);
  list_allocation.clear();
  list_allocation.resize(0);
  list_allocation_dup.clear();
  list_allocation_dup.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  free(memo2);
  free(memo2_dup);

  std::cout << "------------------- Printing Worst Fit -------------------\n";
  int* memo3 = (int*) calloc(n, sizeof(int)); //initialize 1MB memory
  // Worst Fit
  std::cout << "******* Initial filling *******\n";
  allocate_worst(memo3, list_allocation);
  release(memo3, list_allocation);
  status(memo3, holes, filled);
  int* memo3_dup = (int*) calloc(n, sizeof(int));
  for(int i = 0; i < n; i++)
    {memo3_dup[i] = memo3[i];}
  for(int i = 0; i < list_allocation.size(); i++)
    {
      list_allocation_dup.push_back(list_allocation[i]);
    }
  //For non-compaction
  std::cout << "******* Non-compaction *******\n";
  list_allocation.clear();
  list_allocation.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  allocate_worst(memo3, list_allocation);
  status(memo3, holes, filled);
  //For compaction
  std::cout << "******* Compaction *******\n";
  compaction(memo3_dup, list_allocation_dup);
  list_allocation.clear();
  list_allocation.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  allocate_worst(memo3_dup, list_allocation_dup);
  status(memo3_dup, holes, filled);
  list_allocation.clear();
  list_allocation.resize(0);
  list_allocation_dup.clear();
  list_allocation_dup.resize(0);
  holes.clear();
  holes.resize(0);
  filled.clear();
  filled.resize(0);
  free(memo3);
  free(memo3_dup);

  return 0;
}
