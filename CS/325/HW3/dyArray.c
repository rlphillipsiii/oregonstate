/*
 * dyArray.c
 *
 *  Created on: Jan 17, 2011
 *  Modified on: March 9, 2014
 *      Author: Robert Phillips III
 */
#include <assert.h>
#include <stdlib.h>
#include "dyArray.h"

void _dyArrayDoubleCap(struct dyArray *da){
	struct city **oldbuffer = da->data;
	int oldsize = da->size;
	int i;
	dyArrayInit (da, 2 * da->capacity);

	for (i = 0; i < oldsize; i++)
		da->data[i] = oldbuffer[i];

	da->size = oldsize;
	free(oldbuffer);
}//end of the method to double the capacity of the dynamic array

void dyArrayInit(struct dyArray *da, int initialCapacity){
	da->data = (struct city **)(malloc(initialCapacity * sizeof(struct city *)));
	da->size = 0;
	da->capacity = initialCapacity;
}//end of initialization method

void dyArrayFree (struct dyArray *da){
	free(da->data);
}

void dyArrayInsert (struct dyArray *da, int index, struct city *e){
	assert(index >= 0 && index <= da->size);
	
	if(da->size == da->capacity)
		_dyArrayDoubleCap(da);

	int i;
	for(i = da->size; i > index; i--)
		da->data[i] = da->data[i - 1];

	da->data[index] = e;
	da->size++;
}

/* stack operations interface */
void dyArrayPush(struct dyArray *da, struct city *e){
	dyArrayAdd(da, e);
}//end of stack push method

void dyArrayPop(struct dyArray *da){
	assert(da-> size > 0);
	da->size--;
}//end of stack pop method

struct city *dyArrayTop(struct dyArray *da){
	assert(da->size >0);
	return da->data[da->size-1];
}//end of top of stack method

int dyArrayIsEmpty(struct dyArray *da){
	if(da->size > 0)
		return 0;

	return 1;
}//end of array emptiness check

/* bag operations interface */
void dyArrayAdd (struct dyArray *da, struct city *d){
	if (da->size == da->capacity)
		_dyArrayDoubleCap(da);


	da->data[da->size] = d;
	da->size++;
}//end of array add method

int dyArraySize (struct dyArray *da){
	return da->size;
}//end of array size method

void dyArrayRemove (struct dyArray *da, struct city *d){
	assert(!dyArrayIsEmpty(da));

	int i;
	for(i = 0; i < da->size; i++){
		if(da->data[i]->id == d->id)
			da->data[i] = da->data[--da->size];
	}
}//end of the element removal method

int dyArrayContains (struct dyArray *da, struct city *e){
	assert(!dyArrayIsEmpty(da));

	int i;
	for(i = 0; i < da->size; i++){
		if(da->data[i]->id == e->id)
			return 1;
	}

	return 0;
}//end of check to see if the element is in the array

/* array operations interface */
struct city *dyArrayGet (struct dyArray *da, int index){
	assert(index >= 0 && index < da->size);
	return da->data[index];
}//end of the get method

void dyArraySet (struct dyArray *da, int index, struct city *newValue){
	assert(index >= 0 && index < da->size);
	da->data[index] = newValue;
}//end of the set method

/* sorted array operations */
void sortedArrayAdd (struct dyArray *da, struct city *newValue){
	int i = 0;
	while(da->data[i] < newValue && i != da->size)
		i++;

	dyArrayInsert(da, i, newValue);
}//end of the method that adds to an array so it stays sorted

int sortedArrayContains (struct dyArray *da, struct city *testValue){
	if(dyArrayContains(da, testValue)) return 1;

	return 0;
}//end of check to see of a sorted array contains the specified value

void sortedArrayRemove (struct dyArray *da, struct city *testValue){
	int i = 0;
	while(da->data[i] != testValue){
		i++;

		if(i == da->size)
			return;
	}

	for(i = i; i < da->size - 1; i++)
		da->data[i] = da->data[i + 1];

	da->size--;
}//end of the method to remove from the sorted array
