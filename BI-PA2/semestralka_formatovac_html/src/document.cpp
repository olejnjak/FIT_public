//Author: Jakub Olejnik
//jakub.olejnik@fit.cvut.cz

#include "document.h"

//------------------------------------------------------------------------------------------------------------------------
Document::Document (void) : TagNode(NULL) {
}

//------------------------------------------------------------------------------------------------------------------------
/*void	Document::print	(ostream& os, string prefix, string suffix) const {
	for (int i = 0; i < m_ChildNodesNr; i++)
		m_ChildNodes[i]->print(os);
}*/

//------------------------------------------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void Document::printString(ostream& os, string& toPrint, string prefix) const {
	for (int i = 0; i < m_ChildNodesNr; i++)
		m_ChildNodes[i]->printString(os, toPrint, prefix);
}