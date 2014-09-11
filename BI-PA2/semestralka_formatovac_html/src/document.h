//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "tagnode.h"

//! \brief Class represents DOM object document (the parent of all nodes)
class Document : public TagNode {
protected:
	//void	print	(ostream& os, string prefix="", string suffix="") const;
	void printString(ostream& os, string& toPrint, string prefix) const;
public:
				Document (void);
};
