//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "inlinetag.h"

//----------------------------------------------------------------------------------------------------------------------------
//! \brief Construct new InlineTag.
//! \param tag Tag to represent.
InlineTag::InlineTag(Tag * tag) : TagNode(tag) {
}

//----------------------------------------------------------------------------------------------------------------------------
/*void InlineTag::print(ostream& os, string prefix, string suffix) const {
	string begin, end;
	m_Tag->MakeString(begin, end);
	os << prefix << begin;
	for (int i = 0; i < m_ChildNodesNr; i++)
		m_ChildNodes[i]->print(os);
	os << end << suffix;
}*/

//----------------------------------------------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void InlineTag::printString(ostream& os, string& toPrint, string prefix) const {
	string begin, end;
	m_Tag->MakeString(begin, end); //make string from tag

	{ //check if prefix should be printed
		//prefix is printed when tag is first in line
		if (toPrint.length()) {
			//unsigned i = toPrint.length()-1;
			//while (i >= 0 && (toPrint[i] == ' ' || toPrint[i] == '\t')) i--;

			if (toPrint[toPrint.length()-1] != '\n') toPrint += begin;
			else toPrint += prefix + begin;
		}
	}

	for (int i = 0; i < m_ChildNodesNr; i++)  //print child nodes
		m_ChildNodes[i]->printString(os, toPrint, "");

	if (toPrint.length()) {
		for (unsigned i = toPrint.length()-1; i >= 0; i--) { //remove '\n' on the end if any
			if (toPrint[i] == '\n') {
				if (i < toPrint.length()-1) shortenString(toPrint, i+1);
				break;
			}
			else if (toPrint[i] != ' ' && toPrint[i] != '\t') break;
			if (i == 0) toPrint = "";
		}
	}
	toPrint += end;
}

//----------------------------------------------------------------------------------------------------------------------------
//! \brief Method pushes object to stack and adds it to its parent node.
void InlineTag::PushToStack (Stack<TagNode*>& stack) {
	Node::PushToStack(stack);
	if (m_Tag->IsPair()) stack.Push(this); //push only if tag is paired
}