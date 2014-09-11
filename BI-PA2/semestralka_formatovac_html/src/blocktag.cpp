//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "blocktag.h"

//------------------------------------------------------------------------------------------------------------------------
//! \brief Construct new BlockTag.
//! \param tag Tag to represent.
BlockTag::BlockTag(Tag * tag) : TagNode(tag) {
}

/*void BlockTag::print(ostream& os, string prefix, string suffix) const {
	string begin, end;
	m_Tag->MakeString(begin, end);

	os << prefix << begin << endl;

	for (int i = 0; i < m_ChildNodesNr; i++)
		m_ChildNodes[i]->print(os, prefix+"  ", "\n");

	os << prefix << end << endl;
}*/

//------------------------------------------------------------------------------------------------------------------------
//! \brief Function removes new lines from end of string.
//! \param toPrint String with new line to be removed.
void BlockTag::removeNl (string& toPrint) {
	if (!toPrint.length()) return;
	for (unsigned i = toPrint.length()-1; i >= 0; i--) {
		if (toPrint[i] == '\n') {
			shortenString(toPrint,i);
			break;
		}
		else if (toPrint[i] != ' ' && toPrint[i] != '\t') break;
		if (i == 0) toPrint = "";
	}
}

//------------------------------------------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void BlockTag::printString(ostream& os, string& toPrint, string prefix) const {
	string begin, end;
	m_Tag->MakeString(begin, end);	//make string from tags

	removeNl(toPrint);

	toPrint += "\n" + prefix + begin + "\n";

	for (int i = 0; i < m_ChildNodesNr; i++)
		m_ChildNodes[i]->printString(os, toPrint, prefix+m_Indent);

	removeNl(toPrint);

	if (end != "" ) toPrint += "\n" + prefix + end;
	os << toPrint;
	toPrint = "\n";
}

//------------------------------------------------------------------------------------------------------------------------
//! \brief Method pushes object to stack and adds it to its parent node.
void BlockTag::PushToStack (Stack<TagNode*>& stack) {
	Node::PushToStack(stack);
	if (m_Tag->IsPair()) stack.Push(this); //push only if tag is paired
}