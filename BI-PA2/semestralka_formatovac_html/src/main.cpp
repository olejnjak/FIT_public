//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

/*! \mainpage
\section Introduction
This program was written as a semestral project in subject BI-PA2 on <a href='http://fit.cvut.cz' target='_blank'>Faculty of Information Technology</a> on <a href='http://cvut.cz' target='_blank'>Czech Technical University in Prague</a>.

Program is supposed to help you with formatting your HTML codes.

\section Compilation
You can compile the program by using attached <a href='http://en.wikipedia.org/wiki/Make_%28software%29' target='_blank'>Makefile</a> using command <code>make compile</code>.

\section Usage
You have to run the program with two arguments.
<ol type='1'>
	<li><i>input HTML file</i></li>
	<li><i>output HTML file</i></li>
	<li><i>optional: string with indent sequence</i></li>
</ol>

Hope that it will help you make your codes more readable for other programmers.
*/

#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

#include "reader.h"
#include "container.h"
#include "inlinetag.h"
#include "textnode.h"
#include "commenttag.h"
#include "doctypetag.h"
#include "document.h"
#include "functions.h"

string Node::m_Indent;

int main (int argc, char * argv[]) {
	if (argc == 2 && !strcmp(argv[1], "test")) {
		return 0;
	}
	else if (argc != 3 && argc != 4) {
		cerr << "Usage " << argv[0] << " <input file> <output file> [indent string]" << endl;
		return 1;
	}

	ifstream in (argv[1]);

	if (!in.is_open()) {
		cerr << "ERROR: Failed opening input stream for reading." << endl;
		cerr << "No output generated. Ending program." << endl;
		return 3;
	}

	Document * document = NULL;
	Node * x = NULL;


	if (argc == 4) Node::SetIndent(argv[3]);
	else Node::SetIndent("  ");

	try {
		Reader a(in);

		Stack<TagNode*> stack;
		document = new Document();
		stack.Push(document);

		while (in.good()) {
			x =NULL;
			x = a.Run();
			unsigned line = a.GetLine();
			if (!in.good()) {
				delete x;
				break;
			}

			if (x) {
				x->SetLine(line);
				x->PushToStack(stack);
			}
		}
		in.close();

		ofstream out (argv[2]);

		if (!out.is_open()) {
			cerr << "ERROR: Failed opening output stream for writing." << endl;
			cerr << "No output generated. Ending program." << endl;
			delete document;
			return 3;
		}

		out << *document;
		out.close();
		delete document;
	}
	catch (Container::ConfigError e) {
		in.close();
		cerr << e << endl;
		cerr << "No output generated. Ending program." << endl;
		delete document;
		return 2;
	}
	catch (Reader::UnknownTagException e) {
		in.close();
		cerr << e << endl;
		cerr << "No output generated. Ending program." << endl;
		delete document;
		delete x;
		return 2;
	}
	return 0;
}
