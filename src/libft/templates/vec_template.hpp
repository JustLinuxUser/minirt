#include "../dsa/vec_gen.h"
#include "../utils/utils.h"
#include "stdio.h"

#ifndef GEN_TYPE
# define GEN_TYPE int
# error  "Define a GEN_TYPE"
#endif

#ifndef AUTOGENERATING
# define TAB
# define SIZE_T_TAB
# define TYPE_TAB
# define START
# else
# define TAB TAB
#endif

#ifndef GEN_SHORT_TYPE
# define GEN_SHORT_TYPE GEN_TYPE
#endif

#define CONCAT_NX(A, B) A ## B

#define CONCAT(A, B) CONCAT_NX(A, B)

#define FNAME(ACT) CONCAT(CONCAT(vec_, GEN_SHORT_TYPE), CONCAT(_, ACT))

#define VEC_TYPE CONCAT(t_vec_, GEN_SHORT_TYPE)
#define VEC_STRUCT_TYPE CONCAT(s_vec_, GEN_SHORT_TYPE)
#define TYPE_STRING STRINGIFY(VEC_TYPE)

START

typedef struct VEC_STRUCT_TYPE
{
	size_t	SIZE_T_TAB cap;
	size_t	SIZE_T_TAB len;
	GEN_TYPE TYPE_TAB *buff;
} TAB VEC_TYPE;

inline static bool TAB FNAME(init)(
TAB VEC_TYPE *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
	TAB TAB TAB sizeof(GEN_TYPE), init_alloc));
}

inline static bool TAB FNAME(push)(
TAB VEC_TYPE *v, GEN_TYPE el)
{
	return (vec_gen_push((t_vec_gen *)v,
	TAB TAB TAB	sizeof(GEN_TYPE), &el));
}

inline static GEN_TYPE TAB FNAME(pop)(
TAB	VEC_TYPE *v)
{
	GEN_TYPE TAB el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
	TAB TAB TAB sizeof(GEN_TYPE), &el));
	return (el);
}

inline static GEN_TYPE TAB *FNAME(idx)(
TAB	VEC_TYPE *v, size_t idx)
{
	return ((GEN_TYPE *)vec_gen_idx((t_vec_gen *)v,
	TAB TAB TAB sizeof(GEN_TYPE), idx));
}

#undef GEN_SHORT_TYPE
#undef GEN_TYPE
