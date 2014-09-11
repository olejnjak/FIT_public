//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef COMMENTTAG_H
#define COMMENTTAG_H
#include <iostream>
using namespace std;

#include "tagnode.h"

//! \brief Class represents comment.
class CommentTag : public TagNode {
	string m_Content;		//!< Comment content.
//	virtual				void		print 			(ostream& os, string prefix = "", string suffix="") const;
								void 		printString(ostream& os, string& toPrint, string prefix) const;
public:
												CommentTag 	(const char * str);
								void 		PushToStack (Stack<TagNode*>& stack);
};
#endif
