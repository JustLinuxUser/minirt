#include <stdlib.h>
#include "rt_utils.h"

void free_zero(void *ptr)
{
	void **real_ptr = (void **)ptr;
	free(*real_ptr);
	*real_ptr = 0;
}
