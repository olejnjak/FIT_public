//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "functions.h"

//------------------------------------------------------------------------------------------------------------------
//! \brief Function shortens given string.
//! \param str String to be shortened.
//! \param index Index where to put terminating 0.
void shortenString(string& str, unsigned index) {
	if (index >= str.length()) return;

	str[index] = 0;
	str = str.c_str();
}