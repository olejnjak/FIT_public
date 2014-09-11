//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#include "container.h"

//-------------------------------------------------------------------------------------------------------
//! \brief Constructor loads data from config file.
//! \param cfg File with tag configuration.
//! \exception ConfigError Config file cfg not found.
Container::Container (string cfg) {
	m_Tags = NULL;
	m_TagsNr = m_TagsMax = 0;

	ifstream input(cfg.c_str());
	if (!input.is_open()) throw ConfigError(cfg);

	while (input.good()) {
		string line;
		getline(input, line);
		if (line[0] == '#') continue;

		istringstream linestream (line.c_str());

		string tagname, pair;
		linestream >> tagname;
		linestream >> pair;
		if (tagname != "") addTag(tagname, pair == "pair");

		while (linestream.good()) {
			string attr;
			linestream >> attr;
			if (attr != "") m_Tags[m_TagsNr-1].AddAttr(attr);
			attr = "";
		}
	}
	input.close();
}

//-------------------------------------------------------------------------------------------------------
Container::~Container (void) {
	delete [] m_Tags;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Method adds tag to Container.
//! \param name Name of tag.
//! \param pair Sign whether tag is paired or not.
void Container::addTag (string name, bool pair) {
	if (m_TagsNr == m_TagsMax) enlargeArray(m_Tags, m_TagsMax);

	m_Tags[m_TagsNr].m_TagName = name;
	m_Tags[m_TagsNr++].m_Pair = pair;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Method checks if tag is known.
//! \param tag Tag to be checked.
//! \retval true tag is known
//! \retval false tag is unknown
bool Container::IsKnown (string tag) const {
	return (findTag(tag) != NULL);
}


//-------------------------------------------------------------------------------------------------------
//! \brief Method checks if attirbute is know for tag.
//! \param tag Tag to be checked.
//! \param attr Attribute to be checked.
//! \retval true attribute is for tag known
//! \retval false attribute is for tag unknown
bool Container::IsKnown (string tag, string attr) const {
	TagAttr * find = findTag(tag);
	if (!find) return false;

	return find->IsKnown(attr);
}

//-------------------------------------------------------------------------------------------------------
//! \brief Method tries to find tag in container.
//! \param tag Tag to find.
//! \returns pointer found tag
//! \retval NULL tag is not found
Container::TagAttr	*	Container::findTag (string tag) const {
	for (int i = 0; i < m_TagsNr; i++) {
		if (m_Tags[i].m_TagName == tag) return &m_Tags[i];
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Method checks if tag is paired.
//! \param tag Tag to be checked.
//! \retval true tag is paired
//! \retval false tag is not paired
bool	Container::IsPair (string tag) const {
	TagAttr * find = findTag(tag);

	if (!find) return false;

	return find->m_Pair;
}

//-------------------------------------------------------------------------------------------------------
Container::TagAttr::TagAttr (void) {
	m_KnownAttrs = NULL;
	m_AttrsNr = m_AttrsMax = 0;
}

//-------------------------------------------------------------------------------------------------------
Container::TagAttr::~TagAttr (void) {
	delete [] m_KnownAttrs;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Method adds attribute to tag.
//! \param attr Attribute to be added.
void Container::TagAttr::AddAttr (string attr) {
	if (m_AttrsNr == m_AttrsMax) enlargeArray(m_KnownAttrs, m_AttrsMax);

	m_KnownAttrs[m_AttrsNr++] = attr;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Method checks if attribute is known.
//! \param attr Attribute to be checked.
//! \retval true attribute is known
//! \retval false attribute is not known
bool Container::TagAttr::IsKnown (string attr) const {
	for (int i = 0; i < m_AttrsNr; i++)
		if (m_KnownAttrs[i] == attr) return true;
	return false;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Operator = performs hard copy of TagAttr.
Container::TagAttr&	Container::TagAttr::operator=		(const Container::TagAttr& src) {
	if (&src == this) return *this;

	delete [] m_KnownAttrs;
	m_KnownAttrs = new string[src.m_AttrsMax];
	m_AttrsNr = src.m_AttrsNr;
	m_AttrsMax = src.m_AttrsMax;
	m_TagName = src.m_TagName;
	m_Pair = src.m_Pair;
	for (int i = 0; i < m_AttrsNr; i++) m_KnownAttrs[i] = src.m_KnownAttrs[i];

	return *this;
}

//-------------------------------------------------------------------------------------------------------
//! \brief Initializes instance of ConfigError
//! \param str Name of config file.
Container::ConfigError::ConfigError(string str) : m_Str(str) {
}
//-------------------------------------------------------------------------------------------------------
ostream& operator<< (ostream& os, const Container::ConfigError& e) {
	os << "ERROR: Failed to open config file " << e.m_Str << ".";
	return os;
}