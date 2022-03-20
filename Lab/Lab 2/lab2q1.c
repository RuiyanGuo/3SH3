#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/sem.h>
# include <math.h>

//global variables

int a = 6;

//static variables
int fix = 1;


void *addOne(void* arg){
    //define static variable
    static int fix;
    
    //define local variable in this thread
    int var;
    printf("Initial global variables in thread before being modified\n");
    //The initial value is 0 which is default
    printf("thread: the value of a is: %d\n", a);
    printf("Initial static variables in thread before being modified\n");
    //The initial value is 0 which is default
    printf("thread: the value of fix is: %d\n", fix);
    printf("Initial local variables in thread before being modified\n");
    //The initial value is 0 which is default
    printf("thread: the value of var is: %d\n", var);

    //Modify the local variable and global variables
    a += 1;
    var += 1;
    fix += 1;
    
    printf("Global variables in thread\n");
    //print the global variables and they are modified compared to the initial values defined before
    printf("thread: the value of a is: %d\n", a);

    printf("Static variables in thread\n");
    //print the global variables and they are modified compared to the initial values defined before in this thread
    printf("thread: the value of fix is: %d\n", fix);

    printf("Modified Local variables in thread\n");
    //print the local variable and it is modified compare to the initial 0
    printf("thread: the value of var is: %d\n", var);
    printf("\n");
    return 0;

}


int main(void)
{ 
    //Try to print local varibales here but error happen, which proves local varibales can't
    //be accessed outside the threads
    /*
    printf("Local variables\n");
    printf("the value of var is: %d\n", var);*/

    printf("Initial Global variables\n");
    //print the initial values of gloal variables
    printf("the value of a is: %d\n", a);

    printf("Initial Static variables\n");
    //print the initial values of static variable
    printf("the value of fix is: %d\n", fix);
    printf("\n");
    
    //Create 5 threads
    pthread_t pth1, pth2, pth3, pth4, pth5;  

    //Make the threads run the functions above seperately   
    pthread_create(&pth1,NULL,addOne, NULL);
    pthread_create(&pth2,NULL,addOne, NULL);
    pthread_create(&pth3,NULL,addOne, NULL);
    pthread_create(&pth4,NULL,addOne, NULL);
    pthread_create(&pth5,NULL,addOne, NULL);

    //add the threads to main
    pthread_join(pth1,NULL);
    pthread_join(pth2,NULL);
    pthread_join(pth3,NULL);
    pthread_join(pth4,NULL);
    pthread_join(pth5,NULL);

    printf("Global variables\n");
    //The global variables are modified in threads compared to the initial values
    printf("the value of a is: %d\n", a);


    printf("Static variables\n");
    //The static variable remains its value compared to the initial values
    printf("the value of fix is: %d\n", fix); 


    return 0;
}