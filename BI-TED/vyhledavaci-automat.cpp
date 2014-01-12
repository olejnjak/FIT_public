//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz
#include <iostream>
using namespace std;

#define STREAM cout

void generateSubgraphs (int cols, int lines) {
  lines++;
  for (int i = 1; i <= cols-1; i++) { //go through all columns
    STREAM << "  subgraph col" << i << " {" << endl << "    rank=\"same\";" << endl;
    
    for (int j = 1; j <= (i+1 < lines ? i+1 : lines); j++) {  //generate apropriate number of nodes in column
      STREAM << "    " << j << i << " [label=\"" << i;
      for (int k = 1; k < j; k++) STREAM << "'"; //generate '
      STREAM << "\", shape=\"circle\", group=\"l" << j << "\"];" << endl;
    }
    
    STREAM << "  }" << endl << endl;
  }
  
  STREAM << "  subgraph end {" << endl;
  STREAM << "    rank=\"same\";" << endl;
  for (int i = 1; i <= lines; i++) {
    STREAM << "    end" << i << " [peripheries=2, label=\"" << cols;
    for (int k = 1; k < i; k++) STREAM << "'"; //generate '
    STREAM << "\", shape=\"circle\", group=\"l" << i << "\"];" << endl;
  }
  STREAM << "  }" << endl;
}

void generateEdges (string text, int lines) {
  STREAM << "  start [shape=\"point\", style=\"invis\"];" << endl;
  STREAM << "  10 [shape=\"circle\", group=\"l1\" label=\"0\"];" << endl;
  STREAM << "  start->10;" << endl;
  STREAM << "  10->10 [label=\"A\"];" << endl;
//  STREAM << "  10->11 [label=\""<< text[0] <<"\"];" << endl;
  
  for (int i = 1; i <= lines+1; i++) {
    int j;
    for (j = i-1; j < (int)text.length()-1; j++) {
      STREAM << "  "<< i << j << "->" << i << j+1 << " [label=\"" << text[j] << "\"];" << endl;
    }
    STREAM << "  " << i << j << "->end"<< i << " [label=\"" << text[j] << "\"];" << endl;
  }
  STREAM << endl;
  
  for (int i = 1; i < (int)text.length(); i++) { //go through columns
    for (int j = 1; j <= (i < lines ? i : lines); j++)
      STREAM << "  " << j << i << "->" << j+1 << i << " [label=\"N" << text[i] << "\" dir=\"back\"];" << endl;
  }
  STREAM << endl;
  
  int i;
  for (i = 0; i < (int)text.length()-1; i++) {
    for (int j = 1; j <= (i+1 < lines ? i+1 : lines); j++) {
      STREAM << "  " << j << i << "->" << j+1 << i+1 << " [label=\"E,N" << text[i] << "\"];" << endl;
    }
  }
  for (int j = 1; j <= (i+1 < lines ? i+1 : lines); j++) {
    STREAM << "  " << j << i << "->end" << j+1 << " [label=\"E,N" << text[i] << "\"];" << endl;
  }
}

int main (int argc, char * argv[]) {
  string input; int lines;
  
  cin >> input; cin >> lines;

  STREAM << "digraph Automat {" << endl;
  STREAM << "  rankdir=LR" << endl;
  generateSubgraphs(input.length(), lines);
  STREAM << endl;
  generateEdges(input, lines);
  STREAM << "}" << endl;
  
  return 0;
}
