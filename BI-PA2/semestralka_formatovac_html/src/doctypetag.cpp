//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "doctypetag.h"

//-------------------------------------------------------------------------------------------------------
//! \brief Initializes instance with content.
//! \param content Comment content.
DoctypeTag::DoctypeTag (string content) : m_Content (content) {
}

//-------------------------------------------------------------------------------------------------------
/*void DoctypeTag::print (ostream& os, string prefix, string suffix) const {
	os << prefix << "<!DOCTYPE " << m_Content << ">" << endl;
}*/

//-------------------------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void DoctypeTag::printString(ostream& os, string& toPrint, string prefix) const {
	toPrint += "<!DOCTYPE " + m_Content + ">";
}

//-------------------------------------------------------------------------------------------------------
//! Here is object only linked to its parentNode.
//! \brief Method pushes object to stack and adds it to its parent node.
void DoctypeTag::PushToStack (Stack<TagNode*>& stack) {
	Node::PushToStack(stack);
}