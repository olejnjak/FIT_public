bool checkString(const char * inputString) {
	const int automata[19][4] = {
		{0,0,0,0}, //0 - failstate
		{0,0,16,0}, //1
		{18,2,0,5}, //2
		{10,1,9,0}, //>3
		{0,0,0,11}, //4
		{0,2,0,0}, //5
		{0,2,8,0}, //6
		{0,0,17,15},//7
		{18,0,0,0},//8
		{12,0,0,0}, //9
		{14,0,0,0},//10
		{0,0,18,0},//11
		{4,0,13,11},//12
		{0,0,13,11},//13
		{0,2,0,6},//14
		{0,0,17,0},//15
		{0,0,0,7},//<16
		{0,0,0,15},//<17		
		{0,0,0,0}//<18
	};
	
	int state = 3, i = 0;
	while (inputString[i] != 0) {
		int input = inputString[i++]-'a';
		state = automata[state][input];
	}
		
	return state>15;
}
