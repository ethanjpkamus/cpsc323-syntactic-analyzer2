// S = shift
// R = production rule
// ST = state
// A = ?
// EMP = empty
typedef enum {S, R, ST, A, EMP} TYPE;

struct elem
{
	elem()
	: thing(EMP), num(0)
	{}

	elem(TYPE t, int i)
	: thing(t), num(i)
	{}

	TYPE thing;
	int num;
};
