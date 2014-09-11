//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef TAG_H
#define TAG_H

#include <string>
#include "functions.h"
using namespace std;


//! \brief Class represents one HTML tag with its attributes.
class Tag {
	friend class EndTag;
protected:
	//! Structure to save tag attributes.
	struct Attribute {
		string 			m_Name; 	//!< Name of attribute.
		string 			m_Value;	//!< Attribute's value.

								Attribute (string name = "", string value = "");
	};

	string				m_Name;				//!< Tag name.

	Attribute   *	m_Attrs;			//!< Array with attributes.
	int						m_AttrsNr;		//!< Number of attributes.
	int						m_AttrsMax;		//!< Max number of attributes in array.
	bool					m_Pair;				//!< Sign if tag is paired.

public:
												Tag 					(string name, bool pair);
												Tag 					(const Tag& src);
												~Tag 					(void);

						void				AddAttribute	(string name, string val);
						void 				MakeString 		(string& begin, string& end) const;
						bool				IsPair				(void) const;
};
#endif
