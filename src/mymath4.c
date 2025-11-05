#include "mymath.h"

void	sort2f(float *a, float *b)
{
	float	temp;

	if (*a <= *b)
		return ;
	temp = *a;
	*a = *b;
	*b = temp;
}
