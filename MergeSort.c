#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//Args struct
typedef struct{
    int start,end;
}Args;

//global variables
int arrayLength;
int array[100];
//read the array from the file
void readArray()
{
    FILE *f;
    int length;
    char path[30];
    //get the file name
    printf("Enter file name: ");
    fgets(path,30,stdin);
    //to make fgets work \n should be replaced with \0 as fgets reads the enter as \n i guess
    for(int i=0;i<sizeof(path);i++)
    {
        if(path[i]=='\n')
        {
            path[i]='\0';
            break;
        }
    }
    //open the file
    f=fopen(path,"r");
    //if the file name entered doesn't exist the program terminates
    if(f == NULL)
    {
        printf("File name entered doesn't exist in this directory!\n");
        exit(0);
    }
    //get the array length
    fscanf(f,"%d",&arrayLength);
    //get the array
    for(int i=0;i<arrayLength;i++)
    {
        fscanf(f,"%d",&array[i]);
    }
    fclose(f);
}
//Merge Sort Functions

void merge(int start,int mid,int end)
{
    //get the two subarrays length
    int length1 = mid - start +1;
    int length2 = end - mid;
    //initiate the two subarrays
    int left[length1];
    int right[length2];
    //copy the values into these two subarrays
    for(int i=0;i<length1;i++)
        left[i]=array[start + i];

    for(int i=0;i<length2;i++)
        right[i]=array[mid+1+i];

    //compare between the two subarrays to sort
    int i=0;      //index for subarray left[]
    int j=0;      //index fro subarrat right[]
    int k=start;  //index for the original array -yet to be the sorted array-

    //while the two subarrays are still has values
    while(i<length1 && j<length2)
    {
        //if left[i] is smaller put it in the sorted array
        if(left[i] < right[j])
        {
            array[k]=left[i];
            i++;
            k++;
        }
        //if not put right[j] in the sorted array
        else
        {
            array[k]=right[j];
            j++;
            k++;
        }
    }
    //copy the values remaining in left[] in array[] if right[] has no more values
    while(i<length1)
    {
        array[k]=left[i];
        i++;
        k++;
    }
    //copy the values remaining in right[] in array[] if left[] has no more values
    while(j<length2)
    {
        array[k]=right[j];
        j++;
        k++;
    }
}

void * mergeSort(void * arg)
{
    Args *recievedArgs = arg;
    int start=recievedArgs->start ;
    int end=recievedArgs->end;
    if(start < end)
    {
    //Array of Arguments that will be sent in the recursive call;
    Args sentArgs[2];
    //array of threads that will be working in the recursive call;
    pthread_t thread[2];
    int mid = (recievedArgs->start + recievedArgs->end) /2;
    //initiate the values of the sentArg
    sentArgs[0].start=start;
    sentArgs[0].end=mid;
    sentArgs[1].start=mid+1;
    sentArgs[1].end=end;
    //two recursive calls
    pthread_create(&thread[0],NULL,&mergeSort,&sentArgs[0]);
    pthread_create(&thread[1],NULL,&mergeSort,&sentArgs[1]);
    //joining the two threads
    for(int i=0;i<2;i++)
        pthread_join(thread[i],NULL);
    merge(start,mid,end);
    }

}

int main()
{
    //read the array
    readArray();
    //Args will be sent to the mergeSort()

    Args arg;
    arg.start=0;
    arg.end=arrayLength-1;
    //call the merge sort;
    pthread_t thread;
    pthread_create(&thread,NULL,&mergeSort,&arg);
    pthread_join(thread,NULL);
    printf("\nSorted Array\n");

    //print the sorted array
    for(int i=0;i<arrayLength;i++)
        printf("%d ",array[i]);
    printf("\n");

    return 0;
}
