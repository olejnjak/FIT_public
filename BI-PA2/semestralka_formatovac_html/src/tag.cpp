//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "tag.h"
#include <iostream>

//---------------------------------------------------------------------------------------
//! \brief Initialize new tag.
//! \param name Tag name.
//! \param pair Sign if tag is paired.
Tag::Tag (string name, bool pair) : m_Name(name) {
	m_Attrs = NULL;
	m_AttrsMax = m_AttrsNr = 0;
	m_Pair = pair;
}

//---------------------------------------------------------------------------------------
Tag::~Tag (void) {
	delete [] m_Attrs;
}

//---------------------------------------------------------------------------------------
Tag::Tag (const Tag& src) : m_Name (src.m_Name) {
	m_Attrs 		= new Attribute[src.m_AttrsMax];
	m_AttrsMax 	= src.m_AttrsMax;
	m_AttrsNr		=	src.m_AttrsNr;

	for (int i = 0; i < src.m_AttrsNr; i++) m_Attrs[i] = src.m_Attrs[i];
}

//---------------------------------------------------------------------------------------
//! \brief Initialize new tag attribute.
//! \param name Attribute name.
//! \param value Attribute value.
Tag::Attribute::Attribute (string name, string value) : m_Name(name), m_Value(value) {
}

//---------------------------------------------------------------------------------------
//! \brief Add attribute to tag.
//! \param name Attribute name.
//! \param val Attribute value.
void Tag::AddAttribute (string name, string val) {
	if (m_AttrsNr == m_AttrsMax) enlargeArray(m_Attrs, m_AttrsMax);

	m_Attrs[m_AttrsNr].m_Name = name;
	m_Attrs[m_AttrsNr++].m_Value = val;

}

//---------------------------------------------------------------------------------------
//! \brief Makes string from object content.
//! \param begin beginning tag
//! \param end ending tag
void Tag::MakeString (string& begin, string& end) const {
	if (m_Pair) {
		begin = "<" + m_Name;
		for (int i = 0; i < m_AttrsNr; i++) begin += " " + m_Attrs[i].m_Name + "='" + m_Attrs[i].m_Value + "'";
		begin += ">";

		end = "</"+m_Name+">";
	}
	else {
		begin = "<" + m_Name;
		for (int i = 0; i < m_AttrsNr; i++) begin += " " + m_Attrs[i].m_Name + "='" + m_Attrs[i].m_Value + "'";
		begin += " />";

		end = "";
	}
}

//---------------------------------------------------------------------------------------
//! \brief Method checks if tag is paired.
//! \retval true tag is paired
//! \retval false tag is not paired
bool Tag::IsPair (void) const {
	return m_Pair;
}