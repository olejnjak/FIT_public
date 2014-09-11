//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz
#include <iostream>
#include <string>
#include <cstring>

#include "functions.h"
#include "node.h"
#include "commenttag.h"
#include "doctypetag.h"
#include "textnode.h"
#include "endtag.h"
#include "container.h"
#include "inlinetag.h"
#include "blocktag.h"
using namespace std;

//! \brief Class implements finite automata to parse HTML code.
class Reader {
	char 			m_State;		//!< Inner state.
	string 		m_Read;			//!< String with read content.
	istream&	m_Is;				//!< Stream to read from.
	Container	m_Inline;		//!< List with known inline tags.
	Container	m_Block;		//!< List with known block tags.
	mutable 	unsigned 	m_InputLine;	//!< Number of current input line.

	typedef  	Node* (Reader::*m_FnType)(void); //!< Type to define method pointer
	m_FnType 	m_NextFunction;	//!< Pointer to current state.

					Node  *	readTag				(void);
					Node  *	readEndTag		(void);
					Node  *	readComment		(void);
					Node  *	readDoctype		(void);
					Node  *	readText			(void);

					Node  *	state0				(void);
					Node  *	state2				(void);
					Node  *	state4				(void);
					Node 	*	state4doctype (char c);

					void		init 					(void);

					string 	readTagName		(bool& end) const;
					string 	readAttrName	(bool& end) const;
					string 	readAttrVal		(bool& end) const;
					void		skipSpaces		(void) const;

					void		unknownTag		(string name);
					void		unknownAttr		(string name, string attr);
	static	string 	stringToUpper	(string& src);
	static	string 	stringToLower	(string& src);
	static  bool		isLetter 			(char c);

public:
	//! Class represents error exception when unknown tag is read.
	class UnknownTagException {
		string 		m_Name; //! Exception content.
		unsigned	m_Line;
	public:
												UnknownTagException 	(string str, unsigned int);

			friend 	ostream& 	operator<< 						(ostream& os, const UnknownTagException& x);
	};

						Reader				(void);
						Reader 				(istream& is);
	Node *		Run						(void);
	unsigned	GetLine				(void) const;
};

