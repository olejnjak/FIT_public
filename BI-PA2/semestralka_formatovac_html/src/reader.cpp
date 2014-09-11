//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "reader.h"
//---------------------------------------------------------------------------------------
//! \brief Constructor initializes new instance with apropriate stream.
//! \param is Input stream.
Reader::Reader (istream& is) : m_Read(), m_Is(is), m_Inline("src/inline.cfg"), m_Block("src/block.cfg") {
	m_InputLine = 1;
	init();
}
//---------------------------------------------------------------------------------------
//! \brief Method initializes automata.
void Reader::init (void) {
	m_NextFunction	= (&Reader::state0);
	m_Read = "";
}
//---------------------------------------------------------------------------------------
//! \brief Method represents state 0 of automata
//! \returns Pointer to read tag.
Node * Reader::state0 (void) { //initial state
	skipSpaces();					//in state 0 skip following whitespaces
	char c = m_Is.get();	//read first character
	m_Read += c;
	switch (c) {
		case '<':
			m_NextFunction = (&Reader::state2); break; //go to state2
		default:
			if (c == '\n') m_InputLine++;
			m_NextFunction = NULL; //ensures automata to stop
			return readText(); //if read character isn't '<' then I have to read input like text
	}
	return NULL;
}

//---------------------------------------------------------------------------------------
//! \brief Method represents state 2 of automata
//! \returns Pointer to read tag.
Node * Reader::state2 (void) { 	//possible tag, comment, doctype, endtag
	char c = m_Is.get();
	switch (c) {
		case '\n': m_InputLine++;
		case '/':										//if '/' is read I'm reading ending tag
			m_NextFunction = NULL;		//ensures automata to stop
			m_Read += c;
			return readEndTag();
		case '!':										//if '!' is read I have to decide wheter DOCTYPE or comment is read
			m_Read += c;
			m_NextFunction = (&Reader::state4);
			break;
		case ' ': break;
		/*case isLetter(c):
			m_NextFunction = NULL;
			return readTag();*/
		default:
			m_NextFunction = NULL;		//ensures automata to stop
			m_Read += c;
			if (isLetter(c)) return readTag();	//if letter is read then I'm reading tag
			else return readText();							//otherwise I'm reading it as text
	}
	return NULL;
}
//---------------------------------------------------------------------------------------
//! \brief Method represents state 4 of automata
//! \returns Pointer to read tag.
Node * Reader::state4 (void) { //possible comment
	char c = m_Is.get();
	switch (c) {
		m_InputLine++;
		case 'd': return state4doctype(c); break;	//if 'd' or 'D' is read check if it's "doctype" and read it
		case 'D': return state4doctype(c); break;
		case '-':	//try to read comment
			{
				char tmp = m_Is.get();

				if (tmp == '\n') m_InputLine++;

				m_Read += c;
				m_Read += tmp;

				m_NextFunction = NULL;	//ensures automata to stop
				if (tmp == '-') return readComment();	//if proper comment, read it
				else return readText();	//no proper comment -> read text
			}
		break;
		default: //in other cases read text
			m_Read += c;
			m_NextFunction = NULL; //ensures automata to stop
			readText();
		break;

	}
	return NULL;
}

//---------------------------------------------------------------------------------------
//! \brief Tries to read string 'doctype' from input stream.
//! \returns Pointer to read tag.
Node * Reader::state4doctype (char c) {
	const string doctype = "DOCTYPE";
	string tmp; tmp += c;

	//make temporar string
	for (int i = 0; i < (int)doctype.length()-1; i++) {
		char x = m_Is.get();
		if (x == '\n') m_InputLine++;
		tmp += x;
	}

	m_NextFunction = NULL; //ensures automata to stop
	m_Read += tmp;

	stringToUpper(tmp); //convert it to uppercase
	if (tmp == doctype) return readDoctype(); //check whether it's doctype
	else return readText(); //if not read text
}
//---------------------------------------------------------------------------------------
//! \brief Reads text from input stream.
//! \returns Pointer to read node.
Node * Reader::readText (void) {
	m_NextFunction = NULL;
	char c = m_Is.peek();
	while (c != '<' && m_Is.good()) { //stop if next character is '<'
		m_Read += c;
		m_Is.get();
		if (c == '\n') m_InputLine++;
		c = m_Is.peek();
	}

	Node * ret = new TextNode(m_Read);
	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Reads tag from input stream.
//! \returns Pointer to read tag.
Node * Reader::readTag (void) {
	m_NextFunction = NULL;								//ensures automata to stop
	bool end = false;
	string name = readTagName(end);

	stringToLower(name);
	bool line = m_Inline.IsKnown(name);
	bool block = m_Block.IsKnown(name);

	Node * ret;

	if (!line && !block) {								//check if tag is known
		unknownTag(name);
		return readText();
	}

	//check if tag is pair
	bool pair = m_Inline.IsPair(name) || m_Block.IsPair(name);
	Tag * tag = new Tag(name, pair);

	//create apropriate object
	if (line) ret = new InlineTag(tag);
	else ret = new BlockTag(tag);

	//read attributes until the whole tag is read
	while (!end) {
		string attr = readAttrName(end); 	//read attribute name
		if (attr=="") continue;
		string val = readAttrVal(end);		//read attribute val

		stringToLower(attr);							//saving values in lowercase

		//check if the attribute is known
		if (!m_Inline.IsKnown(name, attr) && !m_Block.IsKnown(name, attr)) unknownAttr(name, attr);

		tag->AddAttribute (attr, val);		//finally add attribute to tag
	}

	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Reads endtag from input stream.
//! \returns Pointer to read tag.
Node * Reader::readEndTag	(void) 	{
	skipSpaces();
	m_NextFunction = NULL;		//ensure automata to end
	char c = m_Is.get();
	if (c == '\n') m_InputLine++;
	while (c != '>') {				//read until '>' is read
		m_Read += c;
		c = m_Is.get();
		if (c =='\n') m_InputLine++;
	}

	int len = m_Read.length();
	//char * tmp = m_Read.c_str()
	char * tmp = new char[len+2];
	strcpy (tmp, m_Read.c_str());	//make C string equivalent to m_Read
	//c_str() is not used because it returns pointer to internal C++ string location
	//and it's supposed to be const.
	tmp[len] = 0;									//remove last '>'

	//check if tag is known
	bool line = m_Inline.IsKnown(tmp+2);
	bool block = m_Block.IsKnown(tmp+2);

	if (!line && !block) {	//if unknown
		unknownTag(tmp+2);		//write error
		tmp[len] = '>';				//readd removed '>'
		tmp[len+1] = 0;				//add terminating 0

		Node * ret = new TextNode(tmp);
		delete [] tmp;

		return ret;	//return textnode
	}

	Node * ret = new EndTag(tmp+2);
	delete [] tmp;

	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Reads comment tag from input stream.
//! \returns Pointer to read tag.
Node * Reader::readComment (void) {
	m_NextFunction = NULL; 			//ensure automata to end
	char prev1 = 0, prev2 = 0;
	char c = m_Is.get();
	if (c == '\n') m_InputLine++;
	m_Read += c;

	while (1)  {
		//read until sequence '-->' is read
		if ((c == '>' && prev1 == '-' && prev2 == '-') || !m_Is.good()) break;
		prev2 = prev1;
		prev1 = c;

		c = m_Is.get();
		if (c == '\n') m_InputLine++;
		m_Read += c;
	}

	//create C string without beginning '<!--' and ending '-->'
	unsigned len = m_Read.length()+1;
	char * tmp = new char[len];

	strcpy(tmp, m_Read.c_str());
	tmp[len-4] = 0;

	Node * ret = new CommentTag(tmp+4);
	delete [] tmp;

	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Reads doctype tag from input stream.
//! \returns Pointer to read tag.
Node * Reader::readDoctype (void) {
	m_NextFunction = NULL;		//ensure automata to end
	char c = m_Is.get();
	if (c == '\n') m_InputLine++;
	while (c != '>' && m_Is.good()) { //read until '>' is read
		m_Read += c;
		c = m_Is.get();
		if (c == '\n') m_InputLine++;
	}

	//make C string withour beggining '<!DOCTYPE ' and ending '>'
	char * tmp = new char[m_Read.length()+1];
	memset (tmp, 0, m_Read.length()+1);
	strcpy (tmp, m_Read.c_str());

	DoctypeTag * ret = new DoctypeTag(tmp+10);
	delete [] tmp;

	return ret;
}

//---------------------------------------------------------------------------------------
//! The original string is lost.
//! \brief Function converts string to uppercase.
//! \param src String to be converted.
//! \returns Converted string.
string Reader::stringToUpper(string& src) {
	for (unsigned i = 0; i < src.length(); i++) {
		char c = src[i];
		if (c >= 'A' && c <= 'Z') continue; //if character is upper case continue
		else if (c >= 'a' && c <= 'z') { //if it's lower case convert it
			char cnt = c - 'a';
			src[i] = cnt + 'A';
			continue;
		}
		//nonletters stay the same
	}
	return src;
}
//---------------------------------------------------------------------------------------
//! The original string is lost.
//! \brief Function converts string to uppercase.
//! \param src String to be converted.
//! \returns Converted string.
string Reader::stringToLower(string& src) {
	for (unsigned i = 0; i < src.length(); i++) {
		char c = src[i];
		if (c >= 'a' && c <= 'z') continue; //if character is lower case continue
		else if (c >= 'A' && c <= 'Z') { //if it's uppercase convert it
			char cnt = c - 'A';
			src[i] = cnt + 'a';
			continue;
		}
		//nonletters stay the same
	}
	return src;
}
//---------------------------------------------------------------------------------------
//! \brief Determines if given char is letter.
//! \param c Input character.
//! retval true if c is letter
//! retval false if c isn't letter
bool Reader::isLetter (char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
//---------------------------------------------------------------------------------------
//! \brief Method runs automata.
//! \returns Address of read node.
Node * Reader::Run (void) {
	if (m_Is.eof()) return NULL;
	Node * ret = NULL;
	while (m_NextFunction) { //while not in final state
		ret = (this->*m_NextFunction)(); //run state functions
	}
	init(); //when done initialize for next run
	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Method reads tag name from input stream.
//! \param end variable is set to true if tag has ended (has no attributes)
//! \returns read tag name.
string Reader::readTagName (bool& end) const {
	string ret = "";
	ret += m_Read[m_Read.length()-1];
	char c = m_Is.get();
	if (c == '\n') m_InputLine++;
	while (c != ' ' && c != '>' && m_Is.good()) { //tagnames are separated by ' ' or '>'
		ret += c;
		c = m_Is.get();
		if (c == '\n') m_InputLine++;
	}
	if (c == '>') end = true; //if tag ended signalise it
	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Method skips whitespaces in the stream.
void Reader::skipSpaces (void) const {
	char c = m_Is.peek();
	while (c == ' ' || c == '\n' || c=='\t') { //while whitespace skip
		m_Is.get();
		if (c == '\n') m_InputLine++;
		c = m_Is.peek();
	}
}
//---------------------------------------------------------------------------------------
//! \brief Method reads attribute name from input stream.
//! \param end variable is set to true if tag has ended (has no more attributes)
//! \returns read attribute name.
string Reader::readAttrName (bool& end) const {
	skipSpaces();

	string ret = "";
	char c = m_Is.get();
	if (c == '\n') m_InputLine++;

	if (c == '/' && m_Is.peek() == '>') { //ending characters
		end = true;
		char x = m_Is.get();
		if (x == '\n') m_InputLine++;
		return ret;
	}

	while (c!= ' ' && c != '=' && c != '>' && m_Is.good()) { //attribute name is/should be separated by ' ' and '='
		ret += c;
		c = m_Is.get();
		if (c == '\n') m_InputLine++;
	}
	if (c == '>') end = true; //if tag ended signalise it
	return ret;
}
//---------------------------------------------------------------------------------------
//! \brief Method reads attribute value from input stream.
//! \param end variable is set to true if tag has ended (has no more attributes)
//! \returns read attribute value.
string Reader::readAttrVal (bool& end) const {
	string ret = "";
	char sep = m_Is.get(); //read first char
	if (sep == '\n') m_InputLine++;
	char sep2 = '>'; //attribute is ended by '>', error if in value
	char c = sep, prev = 0;
	if (sep != '\'' && sep != '"') sep = ' '; //if first char wasn't ' or " then first separator is ' '
	else {
		c = m_Is.get();
		if (c == '\n') m_InputLine++;
	}

	while (1) {
		//while not separator -> read
		if ((c == sep && prev !='\\') || c == sep2 || !m_Is.good()) break;
		ret += c;
		c = m_Is.get();
		if (c == '\n') m_InputLine++;
	}
	if (c == '>') end = true; //if tag ended signalise it
	return ret;
}

//---------------------------------------------------------------------------------------
//! \brief Method is called if unknown tag is read and writes note to stderr.
//! \param name name of unknown tag
void Reader::unknownTag (string name) {
	throw UnknownTagException (name, m_InputLine);
}
//---------------------------------------------------------------------------------------
//! \brief Method is called if unknown attribute is read and writes note to stderr.
//! \param name name of tag
//! \param attr name of unknown attribute
void Reader::unknownAttr (string name, string attr) {
	cerr << "Warning (input line " << m_InputLine << "): Attribute '" << attr << "' doesn't seem to be valid for tag <" << name << ">. You can add it to config files (inline.cfg, block.cfg)." << endl;
}
//---------------------------------------------------------------------------------------
//! \brief Method returns number of current input line.
//! \returns number of current input line
unsigned Reader::GetLine (void) const {
	return m_InputLine;
}
//---------------------------------------------------------------------------------------
Reader::UnknownTagException::UnknownTagException (string str, unsigned line) : m_Name(str) {
	m_Line = line;
}
//---------------------------------------------------------------------------------------
ostream& operator<< (ostream& os, const Reader::UnknownTagException& x) {
	os << "Warning (input line " << x.m_Line << "): Tag <" << x.m_Name << "> doesn't seem to be valid. You can add it to config files (inline.cfg, block.cfg)." << endl;
	return os;
}