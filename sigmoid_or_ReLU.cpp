


#include <cmath>


double sigmoid_or_ReLU(double a)
{
	return (1.0 / (1 + pow(2.7182, -1 * a)));


	if (a > 0)
	{
		return a;
	}
	else
	{
		return 0;
	}
}