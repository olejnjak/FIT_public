//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "node.h"

//----------------------------------------------------------------------------------------------------------------------------
ostream& operator<<	(ostream& os, const Node& x) {
	string output = "";
	x.printString(os, output, "");
	os << output;
	return os;
}

//----------------------------------------------------------------------------------------------------------------------------
//! \brief Method pushes object to stack if it's paired.
//! \param stack Stack where to push object.
void Node::PushToStack (Stack<TagNode*>& stack) {
	try {
		TagNode * top = stack.Top();
		top->AddChild(this);
	}
	catch (Stack<TagNode*>::StackEmptyException e) {
		//Empty stack
		cerr << "ERROR (input line " << m_InputLine <<"): Error pairing tags" << endl;
	}
}

//----------------------------------------------------------------------------------------------------------------------------
//! \brief Method sets current input line.
//! \param line New input line.
void Node::SetLine (unsigned line) {
	m_InputLine = line;
}

//----------------------------------------------------------------------------------------------------------------------------
//! \brief Method sets sequence of characters to indent with.
//! \param indent Indent string.
void Node::SetIndent (string indent) {
	Node::m_Indent = indent;
}
