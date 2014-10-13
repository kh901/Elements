#include "common.h"

int Ceil(const int &a, const int &b)
{
	return (a/b) + (a % b != 0);
}
