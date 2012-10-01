// Marbles.cpp : Defines the entry point for the console application.
//

#include "behaviour\Behaviour.h"

int UserTask(void* user)
{
	(void)user;
	return 0;
}

int main(int argc, char* argv[])
{
	//Task task = Application.Start(UserTask, NULL);
	(void)argc; (void) argv;
	return 0;
}
