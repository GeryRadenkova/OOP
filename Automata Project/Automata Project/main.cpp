#include<iostream>
#include "Automat.hpp"

#include"String.hpp"

int main() {

	Automata a;
	a.AddState();
	a.AddState();
	a.AddState();
	a.AddState();
	a.AddTransition(0, 1, 'a');
	a.AddTransition(1, 2, 'b');
	a.AddTransition(2, 1, 'a');
	a.AddTransition(2, 3, 'b');
	a.AddTransition(0, 4, 'a');
	a.MakeStateFinal(3);
	a.MakeStateFinal(4);

	Automata b;
	b.AddState();
	b.AddState();
	b.AddState();
	b.AddState();
	b.AddTransition(0, 1, 'a');
	b.AddTransition(1, 2, 'b');
	b.AddTransition(2, 1, 'b');
	b.AddTransition(2, 3, 'a');
	b.AddTransition(2, 4, 'b');
	b.MakeStateFinal(3);
	b.MakeStateFinal(4);


	Automata f;
	f = Union(a,b);
	f.Print();


	//‘Ë„.5
	/*std::cout << b.isEmptyLanguage() << std::endl;

	Automata c;
	c.AddState();
	c.AddState();
	c.AddTransition(0, 1, 'a');
	c.AddTransition(1, 2, '1');
	c.AddTransition(2, 1, 'a');

	std::cout << c.isEmptyLanguage() << std::endl;*/

	////‘Ë„.4
	//std::cout << b.Recognize("abb") << std::endl;
	//std::cout << b.Recognize("abbba") << std::endl;
	//std::cout << b.Recognize("aab") << std::endl;
	//std::cout << b.Recognize("aba") << std::endl;

	//Print();

	//‘Ë„.10
	/*Automata g;
	g = BuildAutomata("(((a).(((a)+(b)))*).(b))");
	g.Print();*/


	/*Automata d;
	d = BuildAutomata("((a).(((a)+(b)))*)");
	d.Print();
	d.isEmptyLanguage();

	std::cout << d.Recognize("a") << std::endl;
	std::cout << d.Recognize("ba") << std::endl;
	std::cout << d.Recognize("aba") << std::endl;
	std::cout << d.Recognize("bbaaa") << std::endl;
	std::cout << d.Recognize("aabbbbba") << std::endl;*/

   	return 0;
}
