//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz
#ifndef TAGNODE_H
#define TAGNODE_H

#include "node.h"
#include "tag.h"
#include <cstring>

//! \brief Class represents tag.
class TagNode : public Node {
	friend class EndTag;
protected:
	Node 			 ** m_ChildNodes;					//!< List of child nodes (inspired by DOM model).
	int						m_ChildNodesNr;				//!< Number of child nodes.
	int						m_ChildNodesMax;			//!< Max number of child nodes.

	Tag 	* m_Tag;											//!< Tag represented by node.

public:
												TagNode 		(Tag * tag = NULL);
	virtual								~TagNode  	(void);

								void		AddChild		(Node * node);
};

#endif
