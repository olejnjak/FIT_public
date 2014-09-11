//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef BLOCKTAG_H
#define BLOCKTAG_H
#include "tagnode.h"

//! \brief Class represents block tag.
class BlockTag : public TagNode {
protected:
					//void 	print			(ostream& os, string prefix = "", string suffix="") const;
					void 	printString(ostream& os, string& toPrint, string prefix) const;
	static 	void 	removeNl (string& toPrint);
public:
								BlockTag 	(Tag * tag);
					void 	PushToStack (Stack<TagNode*>& stack);
};
#endif
