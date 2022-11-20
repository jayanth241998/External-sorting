#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include<string.h>
#define USE "./mysort <input file> <output file> <number of threads>"
#define BUFFER_SIZE 100

struct minheapnode {
char string[101];
int index;
};

struct MinHeap{

struct minheapnode **nodes;
int heapsize;
 
};

int left(int i){ return 2*i + 1;}
int right(int i){ return 2*i + 2;}

void swap(struct minheapnode *node1,struct minheapnode *node2)
{
 struct minheapnode temp= *node1;
 *node1 = *node2;
 *node2 = temp;
}

void removenode(struct MinHeap *minheap,int index)
{

  *minheap->nodes[index] = *minheap->nodes[minheap->heapsize-1];
  minheap->heapsize--;
  minHeapify(minheap,0);
}

void minHeapify(struct MinHeap *minheap,int index)
{
  int leftchild = left(index);
  int rightchild = right(index);
  char* leftstring =(char*)malloc(sizeof(char)*10);
  char* rightstring =(char*)malloc(sizeof(char)*10);  
  char* currentstring =(char*)malloc(sizeof(char)*10); 
  
  
  int smallest = index; 
  currentstring = strncpy(currentstring,minheap->nodes[smallest]->string,10);   
   
  if(leftchild < minheap->heapsize)
  {
    leftstring = strncpy(leftstring,minheap->nodes[leftchild]->string,10);
    if(strcmp(leftstring,currentstring) < 0){
    smallest = leftchild;}
  }
  
  if(rightchild < minheap->heapsize)
  {
     rightstring = strncpy(rightstring,minheap->nodes[rightchild]->string,10);
     currentstring = strncpy(currentstring,minheap->nodes[smallest]->string,10); 
     if(strcmp(rightstring,currentstring) < 0){
    smallest = rightchild;}
     
  }
  
  if(smallest != index)
  {
    swap(minheap->nodes[index],minheap->nodes[smallest]);
    minHeapify(minheap,smallest); 
  }
  free(leftstring);
  free(rightstring);
  free(currentstring);
}

void createminheap(struct MinHeap *minheap)
{
    int index = (minheap->heapsize/2) - 1;
    
     while(index >= 0)
     {
      minHeapify(minheap,index);
      index--;
     }
     
}

struct minheapnode* getroot(struct MinHeap *minheap)
{
  
 return minheap->nodes[0];
}




void mergefiles(int filenumber,char* outputFile){
   int i=0;
   FILE *sortedfiles[filenumber];
   char filename[100]; 
   FILE* fout;
   
   struct minheapnode **nodes = (struct minheapnode**)malloc(sizeof(struct minheapnode *)* filenumber);
   
    
   for(i=0;i< filenumber; i++)
   {
      snprintf(filename,sizeof(filename),"data%d.txt",i);
      sortedfiles[i] = fopen(filename,"r");
      nodes[i] = (struct minheapnode*)malloc(sizeof(struct minheapnode));
      //nodes[i]->string = (char *)malloc(sizeof(char)*101);
       if (sortedfiles[i] == NULL) {
        fprintf(stderr, "fopen(%s) failed\n", filename);
        continue;
        } 
      fgets(nodes[i]->string, 101, sortedfiles[i]);
      nodes[i]->index = i;
   }
   
   struct MinHeap *minheap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
   minheap->nodes = nodes;
   minheap->heapsize = filenumber;

   
   createminheap(minheap);
   
   int filecount = 0;
   
   fout = fopen(outputFile, "w");
    if (fout == NULL) 
    {
        fprintf(stderr, "fopen(%s) failed", outputFile);
        return;
    }
    
    struct minheapnode *root; 
    
    
   
   while(filecount != filenumber )
   {
    root= getroot(minheap);
    fprintf(fout,"%s",root->string);
    if(fgets(nodes[0]->string, 101, sortedfiles[root->index]) == NULL)
    {
      removenode(minheap,0);
      filecount++;
    }
    minHeapify(minheap,0);
   } 
   

   for(i=0;i< filenumber; i++)
   {
   snprintf(filename,sizeof(filename),"data%d.txt",i); 
   fclose(sortedfiles[i]);
   remove(filename);
   }
   fclose(fout);
  for(i=0;i< filenumber; i++)
   {
    if(nodes[i] != NULL){
    free(nodes[i]);}
   }
   free(minheap);
}

void merge(char **strings,int left,int middle,int right){
    int i,j,k;
   int leftcount = (middle - left) + 1;
   int rightcount = (right - middle);
  
   char **firsthalf = (char**)malloc(sizeof(char*)*(leftcount));
   char **secondhalf = (char**)malloc(sizeof(char*)*(rightcount));
     
   char* first =(char*)malloc(sizeof(char)*10);
   char* second =(char*)malloc(sizeof(char)*10);  
   
   for(i=0;i<leftcount;i++)
   {
     firsthalf[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
     strcpy(firsthalf[i],strings[left+i]);

   }
   
   for(i=0;i<rightcount;i++)
   {
     secondhalf[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
     strcpy(secondhalf[i],strings[middle+i+1]);
   }
   
   k = left;
   for(i=0,j=0; i< leftcount && j < rightcount ; k++)
   {
     first = strncpy(first,firsthalf[i],10);
     second = strncpy(second,secondhalf[j],10);
     if(strcmp(first,second) < 0)
     {
      strcpy(strings[k],firsthalf[i]);
      i++; 
     }
     else{
      strcpy(strings[k],secondhalf[j]);
      j++; 
     }
   
   }
  
   while( i < leftcount){
    strcpy(strings[k],firsthalf[i]);
    i++;
    k++;
   }
   
    while( j < rightcount){
        strcpy(strings[k],secondhalf[j]);
    j++;
    k++;
   }
 
   
    free(first);
    free(second);
    for(int i=0;i<leftcount;i++)
    free(firsthalf[i]);
    for(int i=0;i<rightcount;i++)
    free(secondhalf[i]);
    free(firsthalf);
    free(secondhalf);
}


void mergesort(char **strings,int left,int right) {

if(left < right)
{
  int middle = left + (right - left)/2;
  mergesort(strings,left,middle);
  mergesort(strings,middle+1,right);
  merge(strings,left,middle,right);
}


}



// TODO implement external sort
void mysort(char* inputFile, char* outputFile, int numThreads)
{
    char* buffer;
    FILE* fin;
    FILE* fout;
    FILE* foutsub;
    char** eachline =(char**)malloc(sizeof(char*)*1000000);
    int i,j;
    char filename[100];
    // Open input file
    fin = fopen(inputFile, "r");
    if (fin == NULL) {
        fprintf(stderr, "fopen(%s) failed", inputFile);
        return;
    }

    /* Open output file
    fout = fopen(outputFile, "w");
    if (fout == NULL) {
        fprintf(stderr, "fopen(%s) failed", outputFile);
        return;
    } */
  
    // Allocate memory for the buffer
    buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE*1000000);

    // Read the entries line by line
    // TODO sort the entries
    int filenumber = 0;
    long unsigned int elementsread;
    long unsigned int filechunk =  BUFFER_SIZE*1000000;
    elementsread = fread(&buffer[0], sizeof(char),filechunk, fin);
    
    while ( elementsread != 0) {
          buffer[elementsread] = '\0';
         char* line = strtok(buffer,"\n");
         i = 0;
         while(line != NULL)
         {
            eachline[i] = (char*) malloc(sizeof(char) * BUFFER_SIZE);
            eachline[i] = line;
            i++;
            line = strtok(NULL,"\n");
            
         }
        printf(" file %d  and i -1 = %d\n",filenumber,i-1);
        mergesort(eachline,0,i-1);
        snprintf(filename,sizeof(filename),"data%d.txt",filenumber);
        filenumber++;
        
        foutsub = fopen(filename,"w");
        
         if (foutsub == NULL) {
        fprintf(stderr, "fopen(%s) failed", filename);
        continue;
        } 
        
         
        for(j = 0; j < i ; j++)
        {
        
         fprintf(foutsub,"%s\n",eachline[j]);
        }
        
        fclose(foutsub);
        elementsread = fread(&buffer[0], sizeof(char), BUFFER_SIZE*1000000, fin);
    }
    
    mergefiles(filenumber,outputFile);


    // Clear buffer and close files

    free(eachline);
    free(buffer);
    fclose(fin);
}

int main(int argc, char** argv) {
    char* inputFile;
    char* outputFile;
    int numThreads;
    struct timeval start, end;
    double executionTime;

    if (argc != 4) {
        fprintf(stderr, USE);
        return 1;
    }

    // Read arguments
    inputFile = argv[1];
    outputFile = argv[2];
    numThreads = atoi(argv[3]);

    // Execute sort and measure execution time
    gettimeofday(&start, NULL);
    mysort(inputFile, outputFile, numThreads);
    gettimeofday(&end, NULL);
    executionTime = ((double) end.tv_sec - (double) start.tv_sec)
            + ((double) end.tv_usec - (double) start.tv_usec) / 10000000.0;
            
 
    
    printf("input file: %s\n", inputFile);
    printf("output file: %s\n", outputFile);
    printf("number of threads: %d\n", numThreads);
    printf("execution time: %lf\n", executionTime);

    return 0;
}
