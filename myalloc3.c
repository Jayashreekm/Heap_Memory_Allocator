Heap Memory Manager(Language : C)	
	Implemented a heap memory manager as part of data structures course 
	In this program there is alloc function and yfreee function which behaves same as malloc and free in c. 
	Any program that uses malloc and free can be substututed with alloc and freee.
	I have tried to minimize the Space needed to store metadata about allocated memory and free memory.	And somehow tried to solve the problem of internal fragmentation by merging the contigous free blocks .
	

/*at one time min of 8 bytes can be accessible ,8 bytes-decided just randomly  
  but it should be > 5 as one allocation takes 5 bytes
	if we increase the size of addressable block efficiency will increase and some amt of memory wastage also will there if request is of that type
 */

#include<stdio.h>
#include<stdlib.h>

#define HEAP_MEMORY 520 //Assumed 520 bytes randomly

void *startMetaData,*startActualData,*ptr;

struct node
{
		int noOfBytes;//storage 4 bytes
		char statusAU;//storage 1 bytes
};//storage for 1 node 5(=4+1) byte

struct node *temp,*temp1;

void init(){
		int bytesNeededForStorageOfMetadata;
		startMetaData=malloc(HEAP_MEMORY);
		ptr=startMetaData;
		bytesNeededForStorageOfMetadata=(HEAP_MEMORY/(8+5))*5;// bytesNeededForStorageOfMetadata number of bytes for metadata here it is 200 bytes ...
		startActualData=ptr+bytesNeededForStorageOfMetadata;
		temp=startMetaData;
		temp->statusAU='u';
		temp->noOfBytes=HEAP_MEMORY - bytesNeededForStorageOfMetadata; //remaining 320 bytes cab be used for users data 
}

//if two consecutive stuct nodes with statusAU is 'u' then it merges
void merge(){
		temp=startMetaData;
		while((long int)temp < (long int)startActualData){
				if(temp->statusAU == 'u'){
						ptr=temp;
						temp1=ptr+(temp->noOfBytes/8)*5;
						if(temp1->statusAU=='u'){
								temp->noOfBytes+=temp1->noOfBytes;
						}
				}
				ptr=temp;
				temp=ptr+(temp->noOfBytes/8)*5;
		}//while
}//merge

//fnds the struct node that points to pointer p and then makes it unallocated making status 'u'
void freee(void *p){
		void *toFree=startActualData;
		temp=startMetaData;
		while((long int)temp < (long int)startActualData){
				if(p == toFree){
						if(temp->statusAU=='a'){
								//printf("\n In freee %p\t%p\n",p,toFree);
								temp->statusAU='u';
								merge();
								return;
						}
						else
								printf("\nGarbage collection failed...!"); return;	
				}
				toFree=toFree+temp->noOfBytes;
				ptr=temp;
				temp=ptr+(temp->noOfBytes/8)*5;
		}//while
		printf("\nGarbage collection failed...!");
}//freee

int getInMultiple(int n){
	if(n%8==0)
		return (n/8);
	return (n/8)+1;
}//getInMultiple

//find the block which is large enough and alloates it 
void * alloc(int no){
		void *toReturn=startActualData;
		no=getInMultiple(no);
		temp=startMetaData;
		toReturn=startActualData;
		while((long int)temp < (long int)startActualData){
				if(temp->statusAU=='u'){
						if(temp->noOfBytes >= (no*8)){
								if(temp->noOfBytes > (no*8)){
										ptr=temp;
										temp1=ptr+no*5;
										printf("\ntemp1=%p\ttemp=%p\tno=%d\n",temp1,temp,no);
										temp1->statusAU='u';
										temp1->noOfBytes=temp->noOfBytes-(no*8);
										temp->statusAU='a';
										temp->noOfBytes=no*8;
										return toReturn;
								}
								else{
										temp->statusAU='a';
										temp->noOfBytes=no*8;///no need
										return toReturn;
								}

						}//inner if
				}//outer if
				toReturn=toReturn+temp->noOfBytes;
				ptr=temp;
				temp=ptr+(temp->noOfBytes/8)*5;
		}//while	
		printf("Sorry allocation failed : Insufficient Amount of Memory ...!");
		return NULL;
}//alloc

int main(){
		init();
		int i;
		//...checking the algorithm 
		for(i=0;i<40;i++) //maximum no of allocations can be 40 only because 40*8=320 
				alloc(1); 
		printf("\nSuccessfully 40(MAX) allocations done ...!");
		//void *ptr1=alloc(2),*ptr2=alloc(5),*ptr3=alloc(2);
		//printf("%p\t%p\t%p",ptr1,ptr2,ptr3);
		//freee(ptr1);freee(ptr2);freee(ptr3);
}
