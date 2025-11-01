#include "rt_parser.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "../libft/libft.h"
#include "../error.h"

void free_node(t_rt_node nd) {
	if (nd.t == RT_ND_LIST) {
		for (size_t i = 0; i < nd.list.len; i++)
			free_node(nd.list.buff[i]);
	}
	if (nd.t == RT_ND_DICT) {
		for (size_t i = 0; i < nd.dict.len; i++)
			free_node(nd.dict.buff[i].v);
	}
	free(nd.list.buff);
	free(nd.dict.buff);
}

bool dyn_str_read_file(char* file, t_dyn_str* ret) {
    char buff[1024];
    int len;
    int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
		print_err();
        ft_eprintf("Opening the file for reading: %s: %s\n", file,
                   strerror(errno));
        return (false);
    }
    while (1) {
        len = read(fd, buff, sizeof(buff));
        if (len == 0)
            break;
        if (len > 0)
            dyn_str_pushnstr(ret, buff, len);
        else
            return (print_err(),
					ft_printf("Failed to read the file: %s: %s\n", file,
                               strerror(errno)),
                    close(fd), false);
    }
    close(fd);
    return (true);
}

bool dyn_str_write_file(t_dyn_str buff, char *fname)
{
	int		fd;
	size_t	written;
	int	ret;

	fd = open(fname, O_CREAT | O_TRUNC | O_WRONLY, 0777);
	if (fd < 0) {
		print_err();
        ft_eprintf("Opening the file for writing: %s: %s\n", fname,
                   strerror(errno));
        return (false);
	}
	written = 0;
	while (1) {
		ret = write(fd, buff.buff + written, buff.len - written);
		if (ret < 0)
            return (print_err(),
					ft_printf("Failed to write to file: %s: %s\n", fname,
                               strerror(errno)),
                    close(fd), false);
		if (ret <= 0)
			break;
		written += ret;
	}
	close(fd);
	return (true);
}

char* token_name(enum RT_TT tt) {
    switch (tt) {
        case RT_NONE:
            return "RT_NONE";
        case RT_IDENT:
            return "RT_IDENT";
        case RT_TUPLE:
            return "RT_TUPLE";
        case RT_BOOL:
            return "RT_BOOL";
        case RT_STRING:
            return "RT_STRING";
        case RT_LBRACE:
            return "RT_LBRACE";
        case RT_RBRACE:
            return "RT_RBRACE";
        case RT_LBRACKET:
            return "RT_LBRACKET";
        case RT_RBRACKET:
            return "RT_RBRACKET";
        case RT_COLON:
            return "RT_COLON";
        case RT_EOF:
            return "RT_EOF";
    }
	return (0);
}

