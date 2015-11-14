#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dyArray.h"

#define LEFT   1
#define RIGHT -1
#define NONE   0

int distance(struct city *c1, struct city *c2)
{
	int x = c1->x-c2->x;
	int y = c1->y-c2->y;
	
	double raw = round(sqrt(x*x+y*y));

	return (int) raw;
}

void write_solution(char *fname, struct dyArray *route, int distance)
{
	FILE *fout = fopen(fname, "w");

	fprintf(fout, "%d\n", distance);

	int i;
	for (i = 0; i < route->size; i++) {
		struct city *c = dyArrayGet(route, i);
		
		fprintf(fout, "%d\n", c->id);
	}
	
	fclose(fout);
}

void read_cities(char *fname, struct dyArray *cities)
{
	FILE *f = fopen(fname, "r");
	
	int id, x, y;
	while (fscanf(f, "%d %d %d\n", &id, &x, &y) != EOF) {
		struct city *tmp = (struct city *) malloc(sizeof(struct city));
		tmp->id = id;
		tmp->x = x;
		tmp->y = y;
		
		dyArrayAdd(cities, tmp);
	}
	
	fclose(f);
}

int get_rotation(struct city *ref, struct city *leftmost, struct city *new)
{
	int test = (leftmost->x-ref->x)*(new->y-ref->y)-(new->x-ref->x)*(leftmost->y-ref->y);

	if (test > 0)
		return LEFT;

	if (test < 0)
		return RIGHT;

	return NONE;
}

int find_next_point(struct dyArray *cities, struct city *ref)
{
	int index = 0;
	
	struct city *test = dyArrayGet(cities, 0);
	if (EQ(test, ref)) {
		test = dyArrayGet(cities, 1);
		index = 1;
	}
	
	int i;
	for (i = 0; i < cities->size; i++) {
		struct city *new = dyArrayGet(cities, i);
		if (EQ(new, ref))
			continue;

		int rot = get_rotation(ref, test, new);
		if (rot == RIGHT || rot == NONE && distance(ref, new) > distance(ref, test)) {
			test = new;
			index = i;
		}
	}

	return index;
}
                    
void convex_hull(struct dyArray *route, struct dyArray *cities)
{
	int i;	
	struct city *init = dyArrayGet(cities, 0);

	for (i = 1; i < cities->size; i++) {
		struct city *tmp = dyArrayGet(cities, i);
		if (init->x > tmp->x)
			init = tmp;
	}
	
	dyArrayAdd(route, init);

	for (i = 0; i < route->size; i++) {
		struct city *new = dyArrayGet(cities, find_next_point(cities, dyArrayGet(route, i)));

		if (!EQ(init, new))
			dyArrayAdd(route, new);
	}
}

void find_route(struct dyArray *route, struct dyArray *cities)
{
	int i, j;
	int index = 0, min = 0;

	convex_hull(route, cities);
	for (i = 0; i < route->size; i++)
		dyArrayRemove(cities, dyArrayGet(route, i));

	while (cities->size > 0) {
		min = 0;
		
		struct city *cheapest = NULL;
		struct city *last = dyArrayGet(route, route->size-1);
		
		for (i = 0; i < route->size; i++) {
			struct city *tmp = dyArrayGet(route, i);
			
			for (j = 0; j < cities->size; j++) {
				struct city *unvisited = dyArrayGet(cities, j);

				int dist = distance(last, unvisited)+distance(tmp, unvisited)-
					distance(last, tmp);
				
				if (min > dist || cheapest == NULL) {
					min = dist;
					index = i;
					cheapest = unvisited;
				}
			}

			last = tmp;
		}

		dyArrayInsert(route, index, cheapest);
		dyArrayRemove(cities, cheapest);
	}
}

int get_route_distance(struct dyArray *route)
{
	int total = 0;
	
	int i;
	for (i = 0; i < route->size; i++) {
		struct city *c = dyArrayGet(route, i);

		if (i < route->size-1)
			total += distance(c, dyArrayGet(route, i+1));
		else
			total += distance(c, dyArrayGet(route, 0));
	}

	return total;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("Usage: %s <input_file.\n", argv[0]);
		
		return -1;
	}
	
	struct dyArray cities, route;
	dyArrayInit(&cities, 128);
	dyArrayInit(&route, 128);
	
	read_cities(argv[1], &cities);
	find_route(&route, &cities);

	int distance = get_route_distance(&route);
	printf("ROUTE LENGTH: %d\n", distance);
	printf("TOTAL SIZE:   %d\n", route.size);

	char buf[strlen(argv[1])+4];
	sprintf(buf, "ans-%s", argv[1]);
	
	write_solution(buf, &route, distance);
	return 0;
}
