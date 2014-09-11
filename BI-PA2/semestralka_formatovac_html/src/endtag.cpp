//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "endtag.h"

//------------------------------------------------------------------------------------------------------------------------
//! \brief Initializes instance with name.
//! \param name Tag name.
EndTag::EndTag (string name) {
	m_Tag = new Tag(name, true);
}

//------------------------------------------------------------------------------------------------------------------------
/*void EndTag::print (ostream& os, string prefix, string suffix) const {
}*/

//------------------------------------------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void  EndTag::printString(ostream& os, string& toPrint, string prefix) const {
}

//------------------------------------------------------------------------------------------------------------------------
//! \brief Pops element form stack.
//! \brief Method pairs tags.
//! \param stack Stack with opened tags.
void 	EndTag::PushToStack (Stack<TagNode*>& stack) {
	try {
		TagNode * top = stack.Pop();

		if (!top->m_Tag) {
			cerr << "ERROR (input line "<< m_InputLine << "):\tError pairing tags" << endl << "\textra </" << m_Tag->m_Name << ">" << endl << "\tTrying to fix that." << endl;
		}
		else if (top->m_Tag->m_Name != m_Tag->m_Name) {
//			cerr << top->m_Tag->m_Name << " " << m_Tag->m_Name << "\t";
			cerr << "ERROR (input line "<< m_InputLine << "):\tError pairing tags" << endl << "\texpecting </" << top->m_Tag->m_Name <<"> found </" << m_Tag->m_Name << ">" << endl << "\tTrying to fix that." << endl;
		}
		delete this;
	}
	catch (Stack<TagNode*>::StackEmptyException e) {
		//Empty stack
		cerr << "ERROR (input line "<< m_InputLine << "):\tError pairing tags" << endl << "\tmissing opening tag for <" << m_Tag->m_Name << ">" << endl;
		delete this;
	}
}