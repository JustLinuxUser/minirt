#include "../dsa/vec_gen.h"
#include "../utils/utils.h"

typedef int TAB;
#ifndef GEN_TYPE
# define TAB
# define GEN_TYPE int
#endif

#ifndef TYPE_SHORT
# define TYPE_SHORT GEN_TYPE
#endif

#define CONCAT_NX(A, B) A ## B

#define CONCAT(A, B) CONCAT_NX(A, B)

#define STRINGIFY_NX (A) #A
#define STRINGIFY (A) STRINGIFY_NX(A)

#define FNAME(ACT) CONCAT(CONCAT(vec_, TYPE_SHORT), CONCAT(_, ACT))

#define VEC_TYPE CONCAT(t_vec_, GEN_TYPE)

typedef int START;

typedef struct CONCAT(s, VEC_TYPE)
{
TAB t_vec_gen TAB inner;
} TAB VEC_TYPE;

static bool TAB FNAME(init)(VEC_TYPE *ret, size_t init_alloc)
{
	return (vec_gen_init(&ret->inner, sizeof(GEN_TYPE), init_alloc));
}

static bool TAB FNAME(push)(VEC_TYPE *v, GEN_TYPE el)
{
	return (vec_gen_push(&v->inner, &el));
}

static GEN_TYPE TAB FNAME(pop)(VEC_TYPE *v)
{
	GEN_TYPE TAB el;

	ft_assert(vec_gen_pop(&v->inner, &el));
	return (el);
}

static GEN_TYPE TAB *FNAME(idx)(VEC_TYPE *v, size_t idx)
{
	return ((GEN_TYPE *)vec_gen_idx(&v->inner, idx));
}
