#include "libft/ft_printf/ft_printf.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

void print_err(void)
{
	static bool first = true;
	if (first)
		ft_printf("Error\n");
	first = false;
}

void critical_error(char *msg) {
	print_err();
	ft_eprintf("[CRITICAL ERROR]: %s\n", msg);
	exit(1);
}
