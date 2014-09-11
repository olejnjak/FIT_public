//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef TEXTNODE_H
#define TEXTNODE_H

#include "node.h"
#include "functions.h"
#include <iostream>
#include <cstring>
using namespace std;

class TagNode;

//! \brief Class represents text node.
class TextNode : public Node {
protected:
	string	m_Content;		//!< Text content.

	//void	print 				(ostream& os, string prefix = "", string suffix="") const;
	void 	printString		(ostream& os, string& toPrint, string prefix) const;
	void	removeSpace 	(void);

public:
				TextNode			(const string& src);
	//void	PushToStack (Stack<TagNode*>& stack);
};
#endif
