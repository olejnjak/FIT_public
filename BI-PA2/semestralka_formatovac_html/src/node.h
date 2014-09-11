//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef NODE_H
#define NODE_H

#include "tag.h"
#include "functions.h"
#include "stack.h"

class TagNode;

//! \brief Class represents DOM node.
class Node {
	friend class BlockTag;
	friend class InlineTag;
	friend class Document;
protected:
	static			string 			m_Indent;     //!< Set of characters to be indented with.

							unsigned 		m_InputLine;	//!< Number of current input line.

	virtual			void				printString	(ostream& os, string& toPrint, string prefix) const = 0;
	//virtual			void				print 			(ostream& os, string prefix = "", string suffix="") const = 0;

public:
	static			void				SetIndent		(string indent);

	virtual									~Node 			(void) {}

	virtual			void				PushToStack	(Stack<TagNode*>& stack);
							void				SetLine			(unsigned line);

	friend			ostream&		operator<<	(ostream& os, const Node& x);
};


#include "tagnode.h"
#endif
