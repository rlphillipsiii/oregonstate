# ifndef DyArrayH
# define DyArrayH

/* Dynamic Array Stack (Header)
   Winter 2010
   CS261
*/

# ifndef EQ
# define EQ(a, b) (a->id == b->id)
# endif

struct city {
	int id;
	int x;
	int y;
};

struct dyArray { /* dyArray Structure */
	struct city **data;
	int size;
	int capacity;
};

/* Prototypes */
void dyArrayInit(struct dyArray *da, int initialCapacity);
void dyArrayFree (struct dyArray *da);
void dyArrayInsert (struct dyArray *da, int index, struct city *e);

/* stack operations interface */
void dyArrayPush(struct dyArray *da, struct city *e);
void dyArrayPop(struct dyArray *da);
struct city *dyArrayTop(struct dyArray *da);
int dyArrayIsEmpty(struct dyArray *da);

/* bag operations interface */
void dyArrayAdd (struct dyArray *da, struct city *d);
int dyArraySize (struct dyArray *da);
void dyArrayRemove (struct dyArray *da, struct city *d);
int dyArrayContains (struct dyArray *da, struct city *e);

/* array operations interface */
struct city *dyArrayGet (struct dyArray *da, int index);
void dyArraySet (struct dyArray *da, int index, struct city *newValue);

/* sorted array operations */
void sortedArrayAdd (struct dyArray *da, struct city *newValue);
int sortedArrayContains (struct dyArray *da, struct city *testValue);
void sortedArrayRemove (struct dyArray *da, struct city *testValue);

# endif
