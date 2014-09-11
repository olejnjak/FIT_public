//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "tagnode.h"

//---------------------------------------------------------------------------------------
//! \brief Create new tag node.
//! \param tag Tag to be represented.
TagNode::TagNode (Tag * tag) {
	m_Tag = tag;
	m_ChildNodes = NULL;
	m_ChildNodesNr = m_ChildNodesMax = 0;
}

//---------------------------------------------------------------------------------------
TagNode::~TagNode (void) {
	delete m_Tag;
	for (int i = 0; i < m_ChildNodesNr; i++) delete m_ChildNodes[i];
	delete [] m_ChildNodes;
}

//---------------------------------------------------------------------------------------
//! \brief Add new child to node.
//! \param node Child node to be added.
void TagNode::AddChild (Node * node) {
	if (m_ChildNodesNr == m_ChildNodesMax) enlargeArray(m_ChildNodes, m_ChildNodesMax);

	m_ChildNodes[m_ChildNodesNr++] = node;
}