/*
Maximilien Danisch

Info:
Feel free to use these lines as you wish. 
Efficient implementation of Kruskal's algorithm using a UnionFind datastruture.
https://en.wikipedia.org/wiki/Kruskal's_algorithm
https://en.wikipedia.org/wiki/Disjoint-set_data_structure

Should scale to at least one billion edges on a commodity machine.

To compile:
"gcc kruskal.c -O9 -o kruskal".

To execute:
./kruskal edgelist.txt res.txt
edgelist.txt should contain one edge on each line "u v w" u and v are node id (unsigned int) and w is the edge weight (double).
res.txt will contained the list of the edges of the resulting tree
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

typedef struct {
	unsigned s;
	unsigned t;
	double w;
} edge;

typedef struct {
	unsigned n;//number of nodes
	unsigned e;//number of edges
	edge *edges;//list of edges
} edgelist;

//Compute the maximum of three unsigned integers.
inline unsigned int max3(unsigned int a,unsigned int b,unsigned int c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

int cmpfunc (const void * a, const void * b){
	if ( ((edge*)a)->w>((edge*)b)->w ){
		return 1;
	}
	return -1;
}

edgelist* readedgelist(char* input){
	unsigned e1=NLINKS;
	edgelist *g=malloc(sizeof(edgelist));
	FILE *file;

	g->n=0;
	g->e=0;
	file=fopen(input,"r");
	g->edges=malloc(e1*sizeof(edge));
	while (fscanf(file,"%u %u %lf", &(g->edges[g->e].s), &(g->edges[g->e].t), &(g->edges[g->e].w))==3) {//Add one edge
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		g->e++;
		if (g->e==e1) {
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);
	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	qsort(g->edges,g->e,sizeof(edge),cmpfunc);
	return g;
}

// unionfind structure :
typedef struct {
	unsigned n;//number of objects
	unsigned *p;//parents
	unsigned char *r;//ranks
} unionfind;

unionfind* allocuf(unsigned n){
	unsigned i;
	unionfind* uf=malloc(sizeof(unionfind));
	uf->n=n;
	uf->p=malloc(n*sizeof(unsigned));
	uf->r=malloc(n*sizeof(char));
	for (i=0;i<n;i++){
		uf->p[i]=i;
		uf->r[i]=0;
	}
	return uf;
}

/*//create a singleton {x}
void MakeSet(unsigned x, unionfind *uf){
	uf->p[x]=x;
	uf->r[x]=0;
}
//Decided to do create directly all singletons
*/

//Find the cluster of element x
unsigned Find(unsigned x, unionfind *uf){
	if (uf->p[x]!=x){
		uf->p[x]=Find(uf->p[x],uf);
	}
	return uf->p[x];
}

//Merge the clusters xr and yr
void Union(unsigned xr, unsigned yr, unionfind *uf){
	//if (xr==yr){
	//	return;
	//}
	if (uf->r[xr] < uf->r[yr]){
     		uf->p[xr] = yr;
	}
	else if (uf->r[xr] > uf->r[yr]) {
		uf->p[yr] = xr;
	}
	else {
		uf->p[yr] = xr;
		uf->r[xr] = uf->r[xr]+1;
	}
}

edgelist *alloctree(unsigned n){
	edgelist *el=malloc(sizeof(edgelist));
	el->edges=malloc((n-1)*sizeof(edge));
	el->n=0;
	el->e=0;
	return el;
}

edgelist* kruskal(edgelist* el){
	unsigned i,u,v,p,q;
	edgelist* elr=alloctree(el->n);
	unionfind *uf=allocuf(el->n);

	for (i=0;i<el->e;i++){
		u=el->edges[i].s;
		v=el->edges[i].t;
		p=Find(u,uf);
		q=Find(v,uf);
		if (p!=q){
			Union(p,q,uf);
			elr->edges[elr->e++]=el->edges[i];
		}
	}
	return elr;
}

double printres(edgelist* el, char* output){
	FILE* file=fopen(output,"w");
	double s=0;
	unsigned i;
	for (i=0;i<el->e;i++){
		fprintf(file,"%u %u %le\n",el->edges[i].s,el->edges[i].t,el->edges[i].w);
		s+=el->edges[i].w;
	}
	fclose(file);
	return s;
}


int main(int argc,char** argv){
	edgelist *el,*elr;
	double s;

	time_t t0,t1,t2;
	t1=time(NULL);
	t0=t1;

	printf("Reading edge list from file %s\n",argv[1]);
	el=readedgelist(argv[1]);

	t2=time(NULL);
	printf("- Time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	t1=t2;

	printf("Computing minimum spaning tree\n");

	elr=kruskal(el);

	t2=time(NULL);
	printf("- Time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
	t1=t2;

	printf("Printing result in file %s\n",argv[2]);

	s=printres(elr,argv[2]);
	printf("Sum of the weight in a minimum spaning tree: %le\n",s);

	printf("- Time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t0)/3600,((t2-t0)%3600)/60,((t2-t0)%60));

	return 0;
}
