//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "commenttag.h"

//------------------------------------------------------------------------------------------------------------------------
//! \brief Initializes instance with src content.
//! \param src Comment content.
CommentTag::CommentTag (const char * src) : m_Content(src) {
}

//------------------------------------------------------------------------------------------------------------------------
/*void CommentTag::print  (ostream& os, string prefix, string suffix) const {
	os << "comment" << endl;
}*/

//------------------------------------------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void CommentTag::printString(ostream& os, string& toPrint, string prefix) const {
	toPrint += prefix+"<!--" + m_Content + "-->\n";
}

//------------------------------------------------------------------------------------------------------------------------
//! Here is object only linked to its parentNode.
//! \brief Method pushes object to stack and adds it to its parent node.
void CommentTag::PushToStack (Stack<TagNode*>& stack) {
	Node::PushToStack(stack);
}