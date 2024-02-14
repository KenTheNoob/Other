/*
Solves a 15 puzzle using state space searach

Sample test cases
./puzzle 1 2 3 4 5 6 7 8 9 0 10 11 13 14 15 12
path length: 4

./puzzle 6 5 2 4 10 9 3 8 0 1 7 12 13 14 11 15
path length: 43

./puzzle 7  3  1  4  9  0  2  8  5 11  6 15 13 14 12 10
path length: 373(7000+ iterations)

./puzzle 3 9 1 15 14 11 4 6 13 0 10 12 2 7 8 5
unsolvable
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct state {
	int tiles[N][N];
	int f, g, h;
	short zero_row, zero_column;	/* location (row and colum) of blank tile 0 */
	struct state *next;
	struct state *parent;			/* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];
struct state *start,*goal;
struct state *fringe = NULL, *closed = NULL;
struct state *succ_states[4];

void print_a_state(struct state *pstate) {
	int i,j;
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) 
			printf("%2d ", pstate->tiles[i][j]);
		printf("\n");
	}
	printf("\n");
}

void initialize(char **argv){
	int i,j,k,index, tile;

	start=(struct state *) malloc(sizeof(struct state));
	index = 1;
	for (j=0;j<N;j++)
		for (k=0;k<N;k++) {
			tile=atoi(argv[index++]);
			start->tiles[j][k]=tile;
			if(tile==0) {
				start->zero_row=j;
				start->zero_column=k;
			}
		}
	start->f=0;
	start->g=0;
	start->h=0;
	start->next=NULL;
	start->parent=NULL;
	printf("initial state\n");
	print_a_state(start);

	goal=(struct state *) malloc(sizeof(struct state));
	goal_rows[0]=3;
	goal_columns[0]=3;

	for(index=1; index<NxN; index++){
		j=(index-1)/N;
		k=(index-1)%N;
		goal_rows[index]=j;
		goal_columns[index]=k;
		goal->tiles[j][k]=index;
	}
	goal->tiles[N-1][N-1]=0;	      /* empty tile=0 */
	goal->f=0;
	goal->g=0;
	goal->h=0;
	goal->next=NULL;
	printf("goal state\n");
	print_a_state(goal);

}

/* merge the remaining states pointed by succ_states[] into fringe. 
 * Insert states based on their f values --- keep f values sorted. */
void merge_to_fringe() {
	//...
	// Create a cursor for the fringe
    struct state *curr_state = fringe;
	// Keep track of the previous state for insertion into a linked list(before a node)
	struct state *prev;
	for(int i = 0; i<4; i++){
		// Only insert valid states
		if(succ_states[i] != NULL){
			// Compare each value starting from the fringe(head)
			curr_state = fringe;
			prev = NULL;
			// Figure out where to insert the node
			while(curr_state!=NULL && succ_states[i]->f > curr_state->f){
				prev = curr_state;
				curr_state = curr_state->next;
			}
			// Insert the state
			succ_states[i]->next = curr_state;
			if(prev != NULL){
				prev->next = succ_states[i];
			}
			else{
				// If state->f is smallest(best) or the fringe is empty, the state is the new head
				fringe = succ_states[i];
			}
		}
	}
}

/*swap two tiles in a state*/
void swap(int row1,int column1,int row2,int column2, struct state * pstate){
	int tile = pstate->tiles[row1][column1];
	pstate->tiles[row1][column1]=pstate->tiles[row2][column2];
	pstate->tiles[row2][column2]=tile;
}

/*update the f,g,h function values for a state pointed by a pointer in succ_states */
void update_fgh(int i) {
	struct state *pstate = succ_states[i];
	//...
	if(pstate != NULL){
		int j;
		int k;
		int l;
		// Calculate distance of each number from current state to goal/start state
		for(int num = 0; num<16; num++){
			j = 0;
			// j/4 is the row and j%4 is the column
			while(pstate->tiles[j/4][j%4] != num){
				j++;
			}
			k = 0;
			while(goal->tiles[k/4][k%4] != num){
				k++;
			}
			l = 0;
			while(start->tiles[l/4][l%4] != num){
				l++;
			}
			// Add the distance for each number
			pstate->h += abs(j/4-k/4)+abs(j%4-k%4);
			pstate->g += abs(j/4-l/4)+abs(j%4-l%4);
		}
		pstate->f = pstate->h + pstate->g;
	}
}

/* 0 goes down by a row */
void move_down(struct state * pstate){
	swap(pstate->zero_row, pstate->zero_column, pstate->zero_row+1, pstate->zero_column, pstate); 
	pstate->zero_row++;
}

/* 0 goes right by a column */
void move_right(struct state * pstate){
	swap(pstate->zero_row, pstate->zero_column, pstate->zero_row, pstate->zero_column+1, pstate); 
	pstate->zero_column++;
}

/* 0 goes up by a row */
void move_up(struct state * pstate){
	swap(pstate->zero_row, pstate->zero_column, pstate->zero_row-1, pstate->zero_column, pstate); 
	pstate->zero_row--;
}

/* 0 goes left by a column */
void move_left(struct state * pstate){
	swap(pstate->zero_row, pstate->zero_column, pstate->zero_row, pstate->zero_column-1, pstate); 
	pstate->zero_column--;
}

/* expand a state, and get its children states. Children states pointed by the pointers in succ_states.
 */
void expand(struct state *selected) {
	//...
	//succ_states[..] = ...;
	//...
	// Set all states to null
	for(int i = 0; i<4; i++){
		succ_states[i] = NULL;
	}
	// Only create valid states, invalid states stay null
	if(selected->zero_row>0){
		struct state* up = (struct state *) malloc(sizeof(struct state));
		// Copy the selected state to the created state, then move the created state accordingly
     	memcpy(up, selected, sizeof(struct state));
		move_up(up);
		succ_states[0] = up;
		// Set parent to the selected state for tracing back the path
		succ_states[0]->parent = selected;
	}
	if(selected->zero_row<N-1){
		struct state* down = (struct state *) malloc(sizeof(struct state));
     	memcpy(down, selected, sizeof(struct state));
		move_down(down);
		succ_states[1] = down;
		succ_states[1]->parent = selected;
	}
	if(selected->zero_column>0){
		struct state* left = (struct state *) malloc(sizeof(struct state));
     	memcpy(left, selected, sizeof(struct state));
		move_left(left);
		succ_states[2] = left;
		succ_states[2]->parent = selected;
	}
	if(selected->zero_column<N-1){
		struct state* right = (struct state *) malloc(sizeof(struct state));
     	memcpy(right, selected, sizeof(struct state));
		move_right(right);
		succ_states[3] = right;
		succ_states[3]->parent = selected;
	}
	for(int i = 0; i<4; i++){
		if(succ_states[i] != NULL){
			// Reset the f,g,h,next values of the created states to be calculated later
			succ_states[i]->f = 0;
			succ_states[i]->g = 0;
			succ_states[i]->h = 0;
			succ_states[i]->next = NULL;
		}
	}
}

int states_same(struct state *a,struct state *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg=TRUE;
	return flg;
}

/* Filtering:
 * check the state pointed by succ_states[i] to determine whether this state is repeating.
 * free the state if it is repeating. 
 */ 
void filter(int i, struct state *pstate_list){ 
	struct state *pstate = succ_states[i];
	//...
	// Only filter valid states
	if(pstate != NULL){
        struct state *curr_state = pstate_list;
		// Compare succ_states[i] to each state in pstate_list
		while(curr_state!=NULL){
			if(states_same(pstate, curr_state)){
				// If the state is in fringe/closed free it and set it to null(invalid state)
				free(succ_states[i]);
				succ_states[i] = NULL;
				return;
			}
			curr_state=curr_state->next;
		}
	}
}
int solvable(){
	int parity = 0;
	// Compare each value to all values after it
	for(int i = 0; i < 16; i++){
		for (int j = i + 1; j < 16; j++)
        {
			// If a tile A > B and A comes before B(i always comes before j), increase parity. Ignore the zero tile.
            if (start->tiles[i/4][i%4] != 0 && start->tiles[j/4][j%4] != 0 && start->tiles[i/4][i%4] > start->tiles[j/4][j%4])
            {
                parity++;
            }
        }
	}
	// Check if the 0 is on an even/odd row starting from the bottom
	// Formula for odd row is (4-(start->zero_row))%2 == 1 simplified to start->zero_row%2 == 1 simplified to start->zero_row & 1
	// If zero is on an odd row, parity must be even
	if(start->zero_row & 1){
		return !(parity & 1);
	}
	// If zero is on an even row, parity must be odd
	else{
		return parity & 1;
	}
	return parity;
}
int main(int argc,char **argv) {
	int iter,cnt;
	struct state *curr_state, *cp, *solution_path;
	int ret, i, pathlen=0, index[N-1];

	solution_path=NULL;
	initialize(argv);	/* init initial and goal states */
	fringe=start; 
	iter=0;
	// Check if the puzzle is solvable
	if(solvable() == 0){
		printf("no solution");
		return 0;
	}
	while (fringe!=NULL) {	
		curr_state=fringe;
		fringe=fringe->next;  /* get the first state from fringe to expand */
		// Head shifted after getting a state to expand

		if(states_same(curr_state,goal)){ /* a solution is found */
			do{ /* trace back and add the states on the path to a list */
				curr_state->next=solution_path;
				solution_path=curr_state;
				curr_state=curr_state->parent;
				pathlen++;
			} while(curr_state!=NULL);
			printf("Path (lengh=%d):\n", pathlen); 
			curr_state=solution_path;
			//... /* print out the states on the list */
			while(curr_state->next!=NULL){
				print_a_state(curr_state);
				curr_state = curr_state->next;
			}
			print_a_state(curr_state);
			break;
		}
		expand(curr_state);       /* generate new states */


		for(i=0;i<4;i++){
			filter(i,fringe); /* Check succ_states[i] agaist fringe to see if it is repeatng */
			filter(i,closed); /* Check succ_states[i] agaist closed to see if it is repeatng */
			update_fgh(i); /* evaluate the state if it is not repeating */
		}

		merge_to_fringe(); /* add new states to fringe */

		curr_state->next=closed;
		closed=curr_state;	/* curr_state has been checked/expand, add it to closed */
		/* print out something so that you know your program is still making progress 
		 */
		if(iter++ %1000 == 0) printf("iter %d\n", iter);
	}
	return 0;
} /* end of main */