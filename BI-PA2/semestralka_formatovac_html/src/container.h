//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "functions.h"
using namespace std;

//! \brief Class contains set of known tags and their attributes.
class Container {
	//! \brief Struct represents tag and its attributes.
	struct TagAttr {
		string 		m_TagName; 			//!< Tag name
		bool			m_Pair;					//!< Sign if tag is paired.
		string	*	m_KnownAttrs;		//!< List of known attributes.
		int				m_AttrsNr;			//!< Number of known attributes.
		int				m_AttrsMax;			//!< Max number of attributes.

							TagAttr			(void);
							~TagAttr 		(void);

		void			AddAttr			(string attr);
		bool			IsKnown			(string attr) const;

		TagAttr&	operator=		(const TagAttr& src);
	};

	TagAttr		* m_Tags;			//!< List of known tags.
	int 				m_TagsNr;		//!< Number of known tgs.
	int 				m_TagsMax;	//!< Max number of known tags.

	void 				addTag			(string name, bool pair);
	TagAttr		*	findTag			(string tag) const;

public:
	//! Class represents error exception when config file cannot be opened.
	class ConfigError {
		string m_Str;	//!< Name of input config file.
	public:
												ConfigError(string str);
		friend 		ostream&	operator<< (ostream& os, const ConfigError& e);
	};

							Container 	(string cfg);
							~Container 	(void);

	bool				IsKnown			(string tag) const;
	bool				IsKnown			(string tag, string attr) const;
	bool				IsPair			(string tag) const;

};
#endif
