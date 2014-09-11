//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz
#include "textnode.h"

//---------------------------------------------------------------------------------------
//! \brief Create new text node.
//! \param src String content of node.
TextNode::TextNode (const string& src) {
	m_Content = src;
	removeSpace();
}

//---------------------------------------------------------------------------------------
/*void TextNode::print (ostream& os, string prefix, string suffix) const {
	os << prefix;
	for (unsigned i = 0; i < m_Content.length(); i++) {
		if (m_Content[i] == '\n' && i != m_Content.length()) os << endl << prefix;
		else os << m_Content[i];
	}
	os << suffix;
}*/

//---------------------------------------------------------------------------------------
//! \brief Method appends object's content to string.
//! \param os Output stream.
//! \param toPrint String where to append.
//! \param prefix Prefix to be prepended to object's content.
void TextNode::printString(ostream& os, string& toPrint, string prefix) const {
	if (toPrint.length() && toPrint[toPrint.length()-1] == '\n') toPrint += prefix;

	for (unsigned i = 0; i < m_Content.length(); i++) {
		//if (m_Content[i] == '\n') {
		if (m_Content[i] == '\n') {
			toPrint += "\n" + prefix;

			while (m_Content[i] == ' ' || m_Content[i] == '\t' || m_Content[i] == '\n') {
				//toPrint += "";
				//toPrint += m_Content[i];
				i++;
			}
		}
		else toPrint += m_Content[i];
	}
}

//---------------------------------------------------------------------------------------
//! \brief Removes whitespaces from the end of node's content.
void	TextNode::removeSpace 	(void) {
	unsigned len = m_Content.length();
	unsigned i;

	for (unsigned j = len-1; j > 0; j--) {
		if (m_Content[j] != ' ' && m_Content[j] != '\t' && m_Content[j] != '\n') {
			shortenString(m_Content, j+1);
			break;
		}
	}

	for (i = 0; i < len; i++) {
		if (m_Content[i] != ' ' && m_Content[i] != '\t' && m_Content[i] != '\n') break;
	}

	char * tmp =  new char[len+1];
	strcpy(tmp, m_Content.c_str()+i);
	m_Content = tmp;
	delete [] tmp;
}
