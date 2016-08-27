#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <string.h>
#include <list>
#include <map>
#include <sstream>

using namespace std;
ofstream outfile;

int maxPlayerInput;
int cutOffDepth;
int N;
int maximum(struct boardState *state);
int minimum(struct boardState *state);
int alphaBetaMaximum(struct boardState *state);
int alphaBetaMinimum(struct boardState *state);
int m = -30000;
int x = 0;

struct boardState {

	int depth;
	int bottomPlayer[20];
	int topPlayer[20];

	int topMancala;
	int bottomMancala;
	int childrenVisited = -1;
	int value = -30000;
	bool playAgain = false;
	char maxPlayerName;
	char minPlayerName;
	string currentPlayer;
	int moveNumber;
	string state;
	list<struct boardState> childList;
	int alpha;
	int beta;
};
struct boardState finalState;



int maxValue(int a, int b) {
	if (a > b)
		return a;
	else
		return b;
}

int minValue(int a, int b) {
	if (a < b)
		return a;
	else
		return b;
}

bool isChildEmpty(struct boardState state) {

	bool flagTop = true;
	bool flagBottom = true;

	for (int i = 0; i < N; i++) {
		if (state.topPlayer[i] != 0)
		{
			flagTop = false;
			break;
		}
	}

	for (int i = 0; i < N; i++) {
		if (state.bottomPlayer[i] != 0)
		{
			flagBottom = false;
			break;
		}
	}

	return (flagTop || flagBottom);
}


struct boardState findNextTopState(struct boardState *state) {

	struct boardState childState = *state;
	struct boardState *childStatePointer;
	int i = state->childrenVisited + 1;
	//cout << "\nStart value is " << i << "\n";
	childState.playAgain = false;
	childState.childrenVisited = -1;


	if (maxPlayerInput == 1) {
		childState.currentPlayer = childState.minPlayerName;
		childState.value = -30000;
		childState.state = "min";
		childState.alpha = -30000;
		childState.beta = 30000;

	}
	else
	{
		childState.currentPlayer = childState.maxPlayerName;
		childState.value = 30000;
		childState.state = "max";
		childState.alpha = 30000;
		childState.beta = -30000;
	}

	int pebbles;
	int j = 0;

	while (childState.topPlayer[i] == 0 && state->childrenVisited < N) {
		state->childrenVisited++;
		i++;
	}

	if (i < N) {
		if (childState.topPlayer[i] != 0) {
			pebbles = childState.topPlayer[i];
			childState.topPlayer[i] = 0;
			childState.moveNumber = i;
			if (state->playAgain != true) {
				childState.depth++;
			}

		}
		else {
			childState.state = "empty";
			cout << "Game over";
			return childState;
		}

		//i = i + 1;
		i = i - 1;
		while (true) {

			while (pebbles != 0 && i >= 0) {
				childState.topPlayer[i]++;
				pebbles = pebbles - 1;

				if (pebbles == 0)
				{
					if (childState.topPlayer[i] == 1) {
						childState.topMancala = childState.topMancala + childState.bottomPlayer[i] + 1;
						childState.bottomPlayer[i] = 0;
						childState.topPlayer[i] = 0;
					}
				}

				i--;
			}
			//}

			if (pebbles != 0) {
				//cout << "PEBBLES" << pebbles;
				childState.topMancala++;
				pebbles = pebbles - 1;
				if (pebbles == 0) {
					//cout << "Oh yeay baby";
					childState.playAgain = true;
				}
				else {
					childState.playAgain = false;
				}
			}

			while (pebbles != 0 && j < N) {
				childState.bottomPlayer[j]++;
				pebbles = pebbles - 1;
				j++;
			}

			if (pebbles == 0) {
				//cout << "\n In min func ";
				//if (maxPlayerInput == 1) {
				if (childState.depth == cutOffDepth) {
					if (maxPlayerInput == 1)
						childState.value = childState.bottomMancala - childState.topMancala;
					else
						childState.value = childState.topMancala - childState.bottomMancala;
				}

				childState.alpha = state->alpha;
				childState.beta = state->beta;

				state->childrenVisited = state->childrenVisited + 1;
				while (state->topPlayer[state->childrenVisited + 1] == 0 && state->childrenVisited < N) {
					state->childrenVisited = state->childrenVisited + 1;
				}

				if (isChildEmpty(childState)) {
					childState.state = "empty";
				}
				return childState;
			}

			i = N - 1;
			j = 0;
		}
	}

	childState.state = "empty";
	return childState;
}

struct boardState findNextBottomState(struct boardState *state) {

	struct boardState childState = *state;
	struct boardState *childStatePointer;
	childState.playAgain = false;
	childState.childrenVisited = -1;
	//cout << "State of parent " << state->playAgain;
	int i = state->childrenVisited + 1;
	if (maxPlayerInput == 1) {
		childState.currentPlayer = childState.maxPlayerName;
		childState.state = "max";
		childState.value = +30000;
		childState.alpha = -30000;
		childState.beta = 30000;
	}
	else {
		childState.currentPlayer = childState.minPlayerName;
		childState.state = "min";
		childState.value = -30000;
		childState.alpha = 30000;
		childState.beta = -30000;
	}
	int pebbles;
	int j = N - 1;

	while (childState.bottomPlayer[i] == 0 && state->childrenVisited < N - 1) {
		state->childrenVisited++;
		i++;
	}

	if (i < N) {
		if (childState.bottomPlayer[i] != 0) {
			//cout << "First if";
			pebbles = childState.bottomPlayer[i];
			//cout << "Pebbles" << pebbles << " i is " << i;
			childState.bottomPlayer[i] = 0;
			childState.moveNumber = i;
			//state->childrenVisited = state->childrenVisited + 1;
			if (state->playAgain != true) {
				//cout << "Second if";
				childState.depth++;
			}

		}
		else {
			childState.state = "empty";
			cout << "Game over";
			return childState;
		}

		i = i + 1;

		while (true) {
			while (pebbles != 0 && i < N) {
				childState.bottomPlayer[i]++;
				pebbles = pebbles - 1;

				if (pebbles == 0)
				{
					if (childState.bottomPlayer[i] == 1) {
						childState.bottomMancala = childState.bottomMancala + childState.topPlayer[i] + 1;
						childState.bottomPlayer[i] = 0;
						childState.topPlayer[i] = 0;
					}
				}

				i++;
			}

			if (pebbles != 0) {
				childState.bottomMancala++;
				pebbles = pebbles - 1;
				if (pebbles == 0) {
					//cout << "Its true";
					childState.playAgain = true;
				}
				else {
					childState.playAgain = false;
				}
			}

			while (pebbles != 0 && j >= 0) {
				childState.topPlayer[j]++;
				pebbles = pebbles - 1;
				j--;
			}

			if (pebbles == 0) {
				//cout << "\nPebbles is ZERO\n";
				//if (maxPlayerInput == 2) {
				if (childState.depth == cutOffDepth) {
					if (maxPlayerInput == 1)
						childState.value = childState.bottomMancala - childState.topMancala;
					else
						childState.value = childState.topMancala - childState.bottomMancala;
				}

				childState.alpha = state->alpha;
				childState.beta = state->beta;
				state->childrenVisited = state->childrenVisited + 1;
				while (state->bottomPlayer[state->childrenVisited + 1] == 0 && state->childrenVisited < N) {
					state->childrenVisited = state->childrenVisited + 1;
				}
				if (isChildEmpty(childState)) {
					childState.state = "empty";
				}
				return childState;
			}

			i = 0;
			j = N - 1;
		}
	}
	childState.state = "empty";
	return childState;

}

void display(struct boardState *state) {


	string a;
	if (state->value == 30000)
		a = "Infinity";
	else if (state->value == -30000)
		a = "-Infinity";
	else
		a = to_string(state->value);

	if (state->depth != 0)
		outfile << state->currentPlayer << to_string(state->moveNumber + 2) << "," << state->depth << "," << a << "\n";

	else
		outfile << state->currentPlayer << "," << state->depth << "," << a << "\n";
}

void displayAlphaBeta(struct boardState *state) {


	string a, alpha, beta;
	if (state->value == 30000)
		a = "Infinity";
	else if (state->value == -30000)
		a = "-Infinity";
	else
		a = to_string(state->value);

	if (state->alpha == 30000)
		alpha = "Infinity";
	else if (state->alpha == -30000)
		alpha = "-Infinity";
	else
		alpha = to_string(state->alpha);

	if (state->beta == 30000)
		beta = "Infinity";
	else if (state->beta == -30000)
		beta = "-Infinity";
	else
		beta = to_string(state->beta);

	if (state->depth != 0)
		outfile << state->currentPlayer << to_string(state->moveNumber + 2) << "," << state->depth << "," << a << "," << alpha << "," << beta << "\n";

	else
		outfile << state->currentPlayer << "," << state->depth << "," << a << "," << alpha << "," << beta << "\n";
}


int minimum(struct boardState *state) {

	if (state->depth == cutOffDepth && state->state == "max") {

		//cout << "IN MIN" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";

		if (maxPlayerInput == 1) {
			state->value = state->bottomMancala - state->topMancala;
		}
		else {
			state->value = state->topMancala - state->bottomMancala;
		}

		display(state);
		//cout << "\nReturn values in minimum funct " << state.value << "\n";
		return state->value;
	}

	struct boardState childState;
	struct boardState *childStatePointer = &childState;

	while (state->childrenVisited < N - 1) {

		//cout << "IN MIN" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";

		if (maxPlayerInput == 1) {
			childState = findNextTopState(state);
		}
		else {
			childState = findNextBottomState(state);
		}


		if (childState.state != "empty") {

			if (!childState.playAgain) {
				//cout << "In minimum - Children visited is " << state.childrenVisited << "and State value is " << state.value;
				if (childState.depth != cutOffDepth || childState.playAgain) {
					display(childStatePointer);
					//cout << "bleh";
				}
				state->value = minValue(state->value, maximum(childStatePointer));
				display(state);
			}
			else if (childState.playAgain) {
				//cout << "Enters else if";
				if (childState.childrenVisited == -1) {
					childState.value = 30000;
					//cout << "State value2 is " << state.value;
				}
				if (childState.depth != cutOffDepth || childState.playAgain) {
					display(childStatePointer);
					//cout << "bleh";
				}
				state->value = minValue(state->value, minimum(childStatePointer));
				display(state);
			}
		}
		else {
			cout << "comes in min function " << childState.state << "parent is " << state->currentPlayer << state->moveNumber << " at depth" << state->depth << " and child is " << childState.currentPlayer << childState.moveNumber;


			//childState = findNextTopState(state);
			for (int i = 0; i < N; i++) {
				childState.bottomMancala += childState.bottomPlayer[i];
				childState.bottomPlayer[i] = 0;
			}

			for (int i = 0; i < N; i++) {
				childState.topMancala += childState.topPlayer[i];
				childState.topPlayer[i] = 0;
			}

			if (maxPlayerInput == 1) {
				childState.value = childState.bottomMancala - childState.topMancala;
			}
			else {
				childState.value = childState.topMancala - childState.bottomMancala;
			}

			display(childStatePointer);
			state->value = minValue(state->value, childState.value);
			display(state);
			//return childState.value;
		}
	}
	//cout << "\nReturn values in minimum fucnt " << state.value << "\n";
	return state->value;
}

int maximum(struct boardState *state) {

	int i = 0;

	if (state->depth == cutOffDepth && state->state == "min") {

		//cout << "IN MAX" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";

		if (maxPlayerInput == 1) {
			state->value = state->bottomMancala - state->topMancala;
		}
		else {
			state->value = state->topMancala - state->bottomMancala;
		}
		//cout << "\nFirst state value " << state.value;
		display(state);
		//cout << "\nReturn values in maximum fucnt " << state.value << "\n";
		return state->value;
	}

	struct boardState childState;
	struct boardState *childStatePointer = &childState;

	while (state->childrenVisited < N - 1) {

		//cout << "IN MAX" << state->currentPlayer << state->moveNumber+2 << " " << state->childrenVisited <<"\n";
		if (maxPlayerInput == 1) {
			childState = findNextBottomState(state);
		}
		else {
			childState = findNextTopState(state);
		}

		//cout << childState.currentPlayer << childState.maxPlayerName << childState.depth;
		if (childState.state != "empty") {

			if (!childState.playAgain) {
				//cout << "Comes here";
				if (childState.depth != cutOffDepth || childState.playAgain) {
					display(childStatePointer);
				}
				int p = minimum(childStatePointer);
				state->value = maxValue(state->value, p);

				display(state);

				if (state->depth == 0) {

					x = x + 1;
					if (p > m) {
						m = p;
						finalState = childState;
					}
				}

			}

			else if (childState.playAgain) {

				if (childState.childrenVisited == -1) {
					childState.value = -30000;

				}
				if (childState.depth != cutOffDepth || childState.playAgain) {
					display(childStatePointer);
				}
				int p = maximum(childStatePointer);
				state->value = maxValue(state->value, p);
				//print values
				display(state);
				if (state->depth == 0) {
					//cout << "\nChild1 " << childState.currentPlayer << childState.moveNumber + 2 << " value " << p;
					x = x + 1;
					if (p > m) {
						m = p;
						finalState = childState;
					}
				}

			}

			if (state->playAgain && state->depth == 1) {
				state->childList.push_back(childState);
			}

		}

		else {

			for (int i = 0; i < N; i++) {
				childState.topMancala += childState.topPlayer[i];
				childState.topPlayer[i] = 0;
			}

			for (int i = 0; i < N; i++) {
				childState.bottomMancala += childState.bottomPlayer[i];
				childState.bottomPlayer[i] = 0;
			}

			if (maxPlayerInput == 1) {
				childState.value = childState.bottomMancala - childState.topMancala;
			}
			else {
				childState.value = childState.topMancala - childState.bottomMancala;
			}

			display(childStatePointer);
			state->value = maxValue(state->value, childState.value);
			display(state);

			if (state->depth == 0) {
				cout << "\nChild1 " << childState.currentPlayer << childState.moveNumber + 2 << " value " << childState.value;
				if (childState.value > m) {
					m = childState.value;
					childState.playAgain = false;
					finalState = childState;
				}
			}
			if (state->playAgain && state->depth == 1) {
				state->childList.push_back(childState);
			}
			//return childState.value;
		}
	}

	return state->value;
}

int alphaBetaMinimum(struct boardState *state) {

	if (state->depth == cutOffDepth && state->state == "max") {

		//cout << "IN MIN" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";

		if (maxPlayerInput == 1) {
			state->value = state->bottomMancala - state->topMancala;
		}
		else {
			state->value = state->topMancala - state->bottomMancala;
		}

		displayAlphaBeta(state);
		//cout << "\nReturn values in minimum funct " << state.value << "\n";
		return state->value;
	}

	struct boardState childState;
	struct boardState *childStatePointer = &childState;

	while (state->childrenVisited < N - 1) {

		//cout << "IN MIN" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";

		if (maxPlayerInput == 1) {
			childState = findNextTopState(state);
		}
		else {
			childState = findNextBottomState(state);
		}


		if (childState.state != "empty") {

			if (!childState.playAgain) {
				if (childState.depth != cutOffDepth || childState.playAgain) {
					displayAlphaBeta(childStatePointer);
				}
				//cout << "In minimum - Children visited is " << state.childrenVisited << "and State value is " << state.value;
				int p = alphaBetaMaximum(childStatePointer);
				state->value = minValue(state->value, p);
				int beta = minValue(state->beta, p);
				if (beta <= state->alpha) {
					displayAlphaBeta(state);
					return state->alpha;
				}
				state->beta = beta;
				displayAlphaBeta(state);

			}
			else if (childState.playAgain) {
				if (childState.childrenVisited == -1) {
					childState.value = 30000;
					//cout << "State value2 is " << state.value;
				}
				if (childState.depth != cutOffDepth || childState.playAgain) {
					displayAlphaBeta(childStatePointer);
					//cout << "bleh";
				}
				int p = alphaBetaMinimum(childStatePointer);
				state->value = minValue(state->value, p);
				int beta = minValue(state->beta, p);
				if (beta <= state->alpha) {
					displayAlphaBeta(state);
					return state->alpha;
				}

				state->beta = beta;
				displayAlphaBeta(state);


			}
		}

		else {

			for (int i = 0; i < N; i++) {
				childState.bottomMancala += childState.bottomPlayer[i];
				childState.bottomPlayer[i] = 0;
			}

			for (int i = 0; i < N; i++) {
				childState.topMancala += childState.topPlayer[i];
				childState.topPlayer[i] = 0;
			}

			if (maxPlayerInput == 1) {
				childState.value = childState.bottomMancala - childState.topMancala;
			}
			else {
				childState.value = childState.topMancala - childState.bottomMancala;
			}

			displayAlphaBeta(childStatePointer);

			state->value = minValue(state->value, childState.value);
			int beta = minValue(state->beta, childState.value);
			if (beta <= state->alpha) {
				displayAlphaBeta(state);
				return state->alpha;
			}

			state->beta = beta;
			displayAlphaBeta(state);

			//return childState.value;
		}

	}

	//cout << "\nReturn values in minimum fucnt " << state.value << "\n";
	return state->value;
}

int alphaBetaMaximum(struct boardState *state) {

	int i = 0;

	if (state->depth == cutOffDepth && state->state == "min") {

		//cout << "IN MAX" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";

		if (maxPlayerInput == 1) {
			state->value = state->bottomMancala - state->topMancala;
		}
		else {
			state->value = state->topMancala - state->bottomMancala;
		}
		//cout << "\nFirst state value " << state.value;
		displayAlphaBeta(state);
		//cout << "\nReturn values in maximum fucnt " << state.value << "\n";
		return state->value;
	}

	struct boardState childState;
	struct boardState *childStatePointer = &childState;

	while (state->childrenVisited < N - 1) {

		//cout << "IN MAX" << state->currentPlayer << state->moveNumber + 2 << " " << state->childrenVisited << "\n";
		if (maxPlayerInput == 1) {
			childState = findNextBottomState(state);
		}
		else {
			childState = findNextTopState(state);
		}

		//cout << childState.currentPlayer << childState.maxPlayerName << childState.depth;
		if (childState.state != "empty") {

			//cout << "IN HERE " << state.childrenVisited;
			if (!childState.playAgain) {
				//cout << "Comes here";
				if (childState.depth != cutOffDepth || childState.playAgain) {
					displayAlphaBeta(childStatePointer);
				}
				int p = alphaBetaMinimum(childStatePointer);
				state->value = maxValue(state->value, p);
				int alpha = maxValue(state->alpha, p);
				if (alpha >= state->beta)
				{
					displayAlphaBeta(state);
					return state->beta;
				}
				state->alpha = alpha;
				displayAlphaBeta(state);

				if (state->depth == 0) {
					x = x + 1;
					if (p > m) {
						m = p;
						finalState = childState;
					}
				}

			}

			else if (childState.playAgain) {

				if (childState.childrenVisited == -1) {
					//if (state->childrenVisited == -1) {
					childState.value = -30000;

				}
				if (childState.depth != cutOffDepth || childState.playAgain) {
					displayAlphaBeta(childStatePointer);
				}

				int p = alphaBetaMaximum(childStatePointer);
				state->value = maxValue(state->value, p);
				int alpha = maxValue(state->alpha, p);
				if (alpha >= state->beta)
				{
					displayAlphaBeta(state);
					return state->beta;
				}
				state->alpha = alpha;
				displayAlphaBeta(state);

				if (state->depth == 0) {
					//cout << "\nChild1 " << childState.currentPlayer << childState.moveNumber + 2 << " value " << p;
					x = x + 1;
					if (p > m) {
						m = p;
						finalState = childState;

					}
				}

			}

			if (state->playAgain && state->depth == 1) {
				state->childList.push_back(childState);
			}

		}

		else {

			for (int i = 0; i < N; i++) {
				childState.topMancala += childState.topPlayer[i];
				childState.topPlayer[i] = 0;
			}

			for (int i = 0; i < N; i++) {
				childState.bottomMancala += childState.bottomPlayer[i];
				childState.bottomPlayer[i] = 0;
			}

			if (maxPlayerInput == 1) {
				childState.value = childState.bottomMancala - childState.topMancala;
			}
			else {
				childState.value = childState.topMancala - childState.bottomMancala;
			}

			displayAlphaBeta(childStatePointer);

			state->value = maxValue(state->value, childState.value);
			int alpha = maxValue(state->alpha, childState.value);
			if (alpha >= state->beta)
			{
				displayAlphaBeta(state);
				return state->beta;
			}
			state->alpha = alpha;
			displayAlphaBeta(state);

			if (state->depth == 0) {
				//cout << "\nChild1 " << childState.currentPlayer << childState.moveNumber + 2 << " value " << p;
				x = x + 1;
				if (childState.value > m) {
					m = childState.value;
					childState.playAgain = false;
					finalState = childState;

				}
			}

			if (state->playAgain && state->depth == 1) {
				state->childList.push_back(childState);
			}
			//return childState.value;
		}
	}

	return state->value;
}

int main(int argc, char *argv[]) {
	//int cutOffDepth = 2;

	outfile.open("traverse_log.txt");
	ifstream inFile(argv[argc - 1]);
	string line;
	int operation;

	getline(inFile, line);
	operation = atoi(line.c_str());

	getline(inFile, line);
	maxPlayerInput = atoi(line.c_str());

	getline(inFile, line);
	cutOffDepth = atoi(line.c_str());

	struct boardState initialState;
	initialState.currentPlayer = "root";
	initialState.state = "max";
	initialState.value = -30000;
	initialState.alpha = -30000;
	initialState.beta = 30000;


	if (maxPlayerInput == 1) {
		initialState.minPlayerName = 'A';
		initialState.maxPlayerName = 'B';

	}
	else
	{
		initialState.maxPlayerName = 'A';
		initialState.minPlayerName = 'B';
	}

	getline(inFile, line);
	istringstream iss1(line);
	int num;
	int i = 0;
	while (iss1 >> num)
	{
		initialState.topPlayer[i] = num;
		i++;
	}

	N = i;

	getline(inFile, line);
	istringstream iss2(line);
	int num1;
	int j = 0;
	while (iss2 >> num1)
	{
		initialState.bottomPlayer[j] = num1;
		j++;
	}

	getline(inFile, line);
	initialState.topMancala = atoi(line.c_str());

	getline(inFile, line);
	initialState.bottomMancala = atoi(line.c_str());

	initialState.depth = 0;
	struct boardState *initialStatePointer = &initialState;


	if (operation == 1) {
		cutOffDepth = 1;
		int finalValue = maximum(&initialState);
	}
	else if (operation == 2) {
		//ofstream outfile("output.txt");
		outfile << "Node,Depth,Value\n";
		display(initialStatePointer);
		int finalValue = maximum(&initialState);
	}
	else {
		//ofstream outfile("output.txt");
		outfile << "Node,Depth,Value,Alpha,Beta\n";
		displayAlphaBeta(initialStatePointer);
		int finalValue = alphaBetaMaximum(&initialState);
	}

	struct boardState output;
	if (finalState.playAgain) {
		finalState.childrenVisited = -1;
		struct boardState *finalStatePointer = &finalState;
		struct boardState maxChild = finalState.childList.front();

		for (std::list<struct boardState>::iterator it = finalState.childList.begin(); it != finalState.childList.end(); it++)
		{
			struct boardState temp = *it;
			if (temp.value > maxChild.value) {
				maxChild = temp;
			}

		}
		output = maxChild;
		//cout << "\n MAP " << output.currentPlayer << output.moveNumber + 2;

	}
	else
		output = finalState;

	ofstream outfileState("next_state.txt");
	for (int i = 0; i < N; i++) {
		outfileState << output.topPlayer[i] << " ";
	}
	outfileState << "\n";
	for (int i = 0; i < N; i++) {
		outfileState << output.bottomPlayer[i] << " ";
	}
	outfileState << "\n";
	outfileState << output.topMancala << "\n";
	outfileState << output.bottomMancala << "\n";
	//cout << output.depth << "\n";
	//cout << output.childrenVisited << "\n";
	outfile.close();
	outfileState.close();
	return 0;

}