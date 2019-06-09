#ifndef AUTOMAT_HDR
#define AUTOMAT_HDR

#include<iostream>
#include "Queue.hpp"
#include "DynamicArray.hpp"
#include "Set.hpp"
#include "String.hpp"

struct edge {

	int dest;
	char ch;
	edge() {
		dest = -1;
		ch = '/0';
	}

	edge(int dest, char ch) {
		this->dest = dest;
		this->ch = ch;
	}

	void changeDest(int dest) {
		this->dest = dest;
	}
};

class Automata {

private:
	int startState;
	Set<int> finalStates;
	DynamicArray<DynamicArray<edge>> automata;


public:
	Automata();

	bool Recognize(String str); //return true if automata recognize the string
	bool isEmptyLanguage();

	friend Automata Union(const Automata& a, const Automata& b);
	friend Automata Concat(const Automata& a, const Automata& b);
	friend Automata KleeneStar(const Automata& a); 

	bool AddTransition(int start, int end, char ch); //add edge between two states
	bool IsDeterministic();

	int AddState();
	bool ChangeStartState(int state);
	bool MakeStateFinal(int state);
	int getStatesCount() const;
	int getStartState() const;
	Set<int> getFinalStates() const;

	void Print();

private:
	bool ExistState(int state); //check if state exists
	void Absorb(const Automata& a);
	void CopyTransitions(int x, int y); 
	Set<int> HavePathTo(int begin, String str); //return set of states reachable with that word

};

Automata BuildAutomata(String reg);
Automata CreateBaseAutomata(char ch); //automata with two states and one letter


//definitions
Automata::Automata() {

	startState = 0;
	AddState();
}

int Automata::AddState() {

	DynamicArray<edge> t;
	automata.PushBack(t);
	return automata.getSize() - 1;
}

bool Automata::ChangeStartState(int state) {

	if (!ExistState(state))
		return false;
	startState = state;
	return true;
}

bool Automata::MakeStateFinal(int state) {

	if (!ExistState(state))
		return false;
	return finalStates.Add(state);
}

int Automata::getStatesCount() const {
	return automata.getSize();
}

int Automata::getStartState() const {
	return startState;
}

Set<int> Automata::getFinalStates() const {
	return finalStates;
}

bool Automata::AddTransition(int start, int end, char ch) {

	if (!ExistState(start) && !ExistState(end))
		return false;
	edge e(end, ch);
	automata[start].PushBack(e);
	return true;
}

bool Automata::ExistState(int state) {
	return state < automata.getSize();
}

Set<int> Automata::HavePathTo(int begin, String str) {

	char firstCh = str[0];
	Set<int> result;

	if (str.getLenght() == 0) {
		result.Add(begin);
		return result;
	}

	for (int i = 0; i < automata[begin].getSize(); i++) {

		int otherState = automata[begin][i].dest;
		char symb = automata[begin][i].ch;

		if (symb == firstCh) {
			Set<int> resultStates = HavePathTo(otherState, str.SubString(1));
			result = Union(result, resultStates);
		}
	}
	return result;
}

bool Automata::Recognize(String str) {

	Set<int> result = HavePathTo(startState, str);
	Set<int> intersection = Intersection(finalStates, result);

	return intersection.getSize();
}

bool Automata::isEmptyLanguage() {

	bool* visited = new bool[automata.getSize()];
	for (int i = 0; i < automata.getSize(); i++)
		visited[i] = false;

	Queue<int> gray;
	gray.Push(getStartState());
	visited[getStartState()] = true;

	while (!gray.isEmpty()) {

		int currentVertex = gray.Peek();
		gray.Pop();

		if (getFinalStates().Contains(currentVertex)) {
			delete[] visited;
			return false;
		}

		for (int i = 0; i < automata[currentVertex].getSize(); i++) {

			int neighbor = automata[currentVertex][i].dest;
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				gray.Push(neighbor);
			}
		}
	}
	delete[] visited;
	return true;
}

bool Automata::IsDeterministic() {

	for (int i = 0; i < automata.getSize(); i++) {
		for (int j = 0; j < automata[i].getSize(); j++) {
			for (int k = j + 1; k < automata[i].getSize(); k++) {
				if (automata[i][j].ch == automata[i][k].ch)
					return false;
			}
		}
	}
	return true;
}

void Automata::Absorb(const Automata& a) {

	int sizeAutomata = automata.getSize();

	for (int k = 0; k < a.automata.getSize(); k++) {
		automata.PushBack(a.automata[k]);

		for (int j = 0; j < a.automata[k].getSize(); j++) 
			automata[k + sizeAutomata][j].changeDest(sizeAutomata + automata[k + sizeAutomata][j].dest);
		
		if (a.finalStates.Contains(k)) 
			finalStates.Add(sizeAutomata + k);
	}
}
			
void Automata::CopyTransitions(int x, int y) {

	int to = automata[y].getSize();

	for (int i = 0; i < to; i++) 
		automata[x].PushBack(automata[y][i]);
}

void Automata::Print() {

	for (int i = 0; i < automata.getSize(); i++) {

		std::cout << "State: " << i;
		if (i == startState)
			std::cout << "(S)";
		if (finalStates.Contains(i))
			std::cout << "(F)";
		std::cout << " transitions: ";
		for (int j = 0; j < automata[i].getSize(); j++) 
			std::cout << "with " << automata[i][j].ch << " to " << automata[i][j].dest << "   ";
		
		std::cout << std::endl;
	}
}

Automata Union(const Automata& a, const Automata& b) {

	Automata result = a;
	result.Absorb(b);
	result.AddState(); //it will be the new start state
	result.ChangeStartState(result.getStatesCount() - 1); 
	result.CopyTransitions(result.getStatesCount() - 1, a.getStartState());
	result.CopyTransitions(result.getStatesCount() -1 , a.getStatesCount() + b.getStartState());
	
	if (a.getFinalStates().Contains(a.getStartState()) || b.getFinalStates().Contains(b.getStartState()))
		result.MakeStateFinal(result.getStartState());

	return result;
}

Automata Concat(const Automata& a, const Automata& b) {

	Automata result = a;
	Set<int> firstAutomataFinalStats(a.getFinalStates()); // copy c-tor
	result.Absorb(b);
	
	for (int i = 0; i < firstAutomataFinalStats.getSize(); i++) 
		result.CopyTransitions(firstAutomataFinalStats.getElement(i), b.getStartState() + a.getStatesCount());
	
	if (!b.getFinalStates().Contains(b.getStartState())) 
		result.finalStates = SetDifference(result.finalStates, firstAutomataFinalStats);
	
	return result;
}

Automata KleeneStar(const Automata& a) {

	Automata result = a;
	result.AddState();
	result.ChangeStartState(result.getStatesCount() - 1);
	result.CopyTransitions(result.getStatesCount() - 1, a.getStartState());
	
	for (int i = 0; i < result.getFinalStates().getSize(); i++) 
		result.CopyTransitions(result.getFinalStates().getElement(i), result.getStartState());
	result.MakeStateFinal(result.getStartState());

	return result;
}

Automata BuildAutomata(String reg) {

	Automata result;

	if (reg.getLenght() == 3)
		result = CreateBaseAutomata(reg[1]); //(a)

	else if (reg[reg.getLenght() - 1] == '*')
		result = KleeneStar(BuildAutomata(reg.SubString(1, reg.getLenght() - 3)));
	else {
		String newReg = reg.SubString(1, reg.getLenght() - 2);  //remove brackets

		int countOpeningBrackets = 0;
		int operationIndex;

		for (int i = 0; i < newReg.getLenght(); i++) {
			if (newReg[i] == '(')
				countOpeningBrackets++;
			else if (newReg[i] == ')')
				countOpeningBrackets--;

			if (countOpeningBrackets == 0) {
				operationIndex = i + 1;
				break;
			}
		}

		if (newReg[operationIndex] == '+')
			result = Union(BuildAutomata(newReg.SubString(0, operationIndex - 1)),
				BuildAutomata(newReg.SubString(operationIndex + 1, newReg.getLenght() - 1)));
		else if (newReg[operationIndex] = '.')
			result = Concat(BuildAutomata(newReg.SubString(0, operationIndex - 1)),
				BuildAutomata(newReg.SubString(operationIndex + 1, newReg.getLenght() - 1)));
	}

	return result;
}

Automata CreateBaseAutomata(char ch) {

	Automata a;
	a.AddState();
	a.AddTransition(0, 1, ch);
	a.MakeStateFinal(1);
	return a;
}


#endif // !AUTOMAT_HDR
