#include "libft/ft_printf/ft_printf.h"
#include <unistd.h>
#include <stdlib.h>

void critical_error(char *msg) {
	ft_eprintf("Error\n[CRITICAL ERROR]: %s\n", msg);
	exit(1);
}
