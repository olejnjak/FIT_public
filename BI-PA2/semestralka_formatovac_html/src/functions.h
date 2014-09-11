//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
using namespace std;

template <class T>
void enlargeArray(T *& array, int& sizeMax, int constant = 2);

//------------------------------------------------------------------------------------------------------------------
//! Function handles array enlarging.
//! \param array Array to be resized.
//! \param sizeMax Number of items in given array.
//! \param constant Number to multiply sizeMax.
template <class T>
void enlargeArray(T *& array, int& sizeMax, int constant = 2) {
	int newMax = (sizeMax > 0 ? sizeMax*constant : 10); // if sizeMax is 0 or less, sizeMax will be 10

	T * newArray = new T[newMax]; // allocate new array
	for (int i = 0; i < sizeMax; i++) newArray [i] = array[i]; // copy items

	sizeMax = newMax; // set new sizeMax
	delete [] array; // delete old array
	array = newArray; // change pointer
}

void shortenString(string& str, unsigned index);
#endif
