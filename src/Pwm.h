#include <vector>
#include <iostream>
#ifndef SINGLETON_H
#define SINGLETON_H

class PWM
{
	public:
	static PWM& getInstance();
	void enable();

	private:
	PWM();
	~PWM();
};

#endif
