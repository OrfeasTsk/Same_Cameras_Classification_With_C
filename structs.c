#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/structs.h"


int getNumId2(char* fullId){
	int count;
	
	for( count = 0; count < 2 ; fullId++)		// Mono o arithmos
		if(*fullId == '/')
			count++;			
						
	return atoi(fullId);
	
}


/*##################                  Start Generic Queue                                        ##########################*/

void QueueInit(Queue* queue){ 			//Arxikopoihsh ouras
        
        queue->head=NULL;
        queue->tail=NULL;
        queue->count=0;    
}

void QueueInsert(Queue* queue,void ** data){ //Eisagwgh sthn oura
        struct QueueNode *node=(struct QueueNode *)malloc(sizeof(struct QueueNode));
        node->data=*data;
        node->next=NULL;
        
        if(queue->tail == NULL){ 		//Gia na bei o prwtos komvos
        	queue->head=node;
        	queue->tail=node;
    	}
    	else{
            queue->tail->next=node;
            queue->tail=node;
        }
        
        queue->count++;
}

int QueueEmpty(Queue* queue){
	
	return queue->count == 0;
}

void QueueConcat(Queue* q1 , Queue* q2,Clique* cliq){
	struct QueueNode* curr = q2->head,*prev;
	Pair* pair;
	
	while( curr != NULL ){
		pair = (Pair*)(curr->data);
		pair->cliq = cliq; //Ola ta items exoun ws related oura thn q1
		QueueInsert(q1,(void**)&pair); //Eisagwgh tous apo thn q2 sthn q1
		prev = curr;
		curr = curr->next;
		free(prev); //Apodesmeush tou komvou ouras ths q2
	}
	
	free(q2); //Apodesmeush ths q2
	
}

/*void QueueDelete(Queue* queue, int id){
	
	QueueNode* curr = queue->head,*temp,*prev = NULL;
	Clique* clique;
								
	while( curr != NULL ){								// diasxizoume thn oura 
		clique  = (Clique*)(curr->data);														// related_pairs	
		if( clique->id == id){										// an einai to idio item
		
			if (curr == queue->head ){														// an einai o head
				if( curr == queue->tail ){													// an einai to monadiko antikeimeno
					queue->head=NULL;														// diagrafh tou komvou
					queue->tail=NULL;
					queue->count--;
					temp=curr;
					curr = curr->next; 
					free(temp);
					free(clique);
				}
				else{																				// diaforetika o head tha deiksei ston epomeno komvo
					queue->head=curr->next;
					queue->count--;
					temp=curr;
					curr = curr->next; 
					free(temp);
					free(clique);
				}
			}
			else if(curr == queue->tail){													// an einai to teleutaio item
				queue->tail = prev;															// tha metakinithei enan komvo pisw
				prev->next = NULL;
				queue->count--;
				temp = curr;
				curr = curr->next;
				free(temp);
				free(clique);
			}
			else{																					// se opoiadhpote allh periptwsh
				prev->next=curr->next;
				queue->count--;
				temp = curr;
				curr = curr->next;
				free(temp);
				free(clique)
			}
			
		}
		prev = curr;
		curr = curr->next;
		
	}
	
}


void QueuePrint(Queue* queue){ 				//Ektypwsh ouras
        struct QueueNode *Temp;

        if(queue->head == NULL)
                return;
        
        for(Temp = queue->head ; Temp != NULL ; Temp = Temp->next)
                printf("%s",Temp->data);
        
}*/

/*##################                  End Generic Queue                                        ##########################*/


/*##################     Start Stack For only one char !! Used in Json Parse !!                ##########################*/


void StackInit(Stack * stack){ //Arxikopoihsh ths listas
	
	stack->head=NULL;
	stack->count=0;
	
}

struct StackNode * push(Stack * stack, char data){//Eisagwgh sthn arxh
	
	struct StackNode * Temp=(struct StackNode *)malloc(sizeof(struct StackNode));
	Temp->next=stack->head;
    Temp->data=data;
    stack->head=Temp;
    stack->count++;
	
	return Temp;
	
}

void check(Stack * stack, char c ){ // Diagrafh prwtou item elegxontas an prepei na diagraftei(auto to blepoume apo ta analoga tou [ me ], { me } kai " me " ), alliws push tou char
	
/*	if(stack->count == 0){
		printf("\nStack is empty!!\n");
	}*/
	
	struct StackNode * Temp;
	
	if(c==']'){					//Elegxoume an sthn korufh uparxei to analogo char
		if(stack->head->data == '['){
			Temp=stack->head;
    		stack->head=stack->head->next;
    		stack->count--;
    		free(Temp);
		}
	}	
	else if(c=='}'){				//Elegxoume an sthn korufh uparxei to analogo char
		if(stack->head->data == '{'){
			Temp=stack->head;
    		stack->head=stack->head->next;
    		stack->count--;
    		free(Temp);
		}
	}
	else if(c=='"'){				//Elegxoume an sthn korufh uparxei to analogo char
		if(stack->head->data == '"'){
			Temp=stack->head;
    		stack->head=stack->head->next;
    		stack->count--;
    		free(Temp);
		}
		else						// Giati tha exei elegxtei hdh to " 
			push(stack,c);
	}
	else if(c == '[' || c == '{')							// diaforetika kanoume push to stoixeio
		push(stack,c);
	
}

int StackEmpty(Stack * stack){
	return stack->count == 0;
}



/*##################              End Stack                            ##########################*/







//RED-BLACK





void treeprint(Link p, int indent,int type,Link head) /* Pretty print tree */
{ int i;
if (p != z) { /* If tree is not empty */
treeprint(p->r, indent+4,1,head);
/* Print right subtree 4 places right of root node */
for (i=0 ; i < indent ; i++)
printf(" "); /* Take care for indentation */
if(type==1 && p!=head)
printf("/");
if (type==2)
printf("\\");
printf("%d\n", p->rbitem->id); /* Print root node */
treeprint(p->l, indent+4,2,head);
/* Print left subtree 4 places right of root node */
}
} 


Link rotR(Link h) //Deksia peristrofh
  { Link x = h->l; h->l = x->r; x->r = h;
    return x; }

Link rotL(Link h) //Aristerh peristrofh
  { Link x = h->r; h->r = x->l; x->l = h;
    return x; }


Link NEW(void* id,void** obj, Link l, Link r, int color){ 
	Link x =(Link)malloc(sizeof(struct RBnode));
  	x->l = l; x->r = r; 
	x->color=color;
	if( obj != NULL){
		x->rbitem = (RBItem*)malloc(sizeof(RBItem));
    	QueueInit(&(x->rbitem->objs)); //Arxikopoihsh ths ouras
		QueueInsert(&(x->rbitem->objs),obj);	//Eisagwgh sthn lista tou komvou
		x->rbitem->id=id;
	}
    return x;
  }

 
void RBinit() //Arxikopoihsh tou kenou komvou
	{ z = NEW( -1 ,NULL, NULL, NULL, BLACK); }
  
void RBdestr()
{
	free(z);
	z = NULL;
}
  
void RBTinit(Link* head)//Arxikopoihsh tou deikth tou dentrou
  { *head = z; }


Link MakeRBTree(Link h,Link head){  //Synarthsh pou ftiaxnei to red-black tree 
Link leftchild=h->l , rightchild=h->r ;
if(leftchild->color==RED && rightchild->color==RED) //Periptwsh pou to deksi kai to aristero paidi einai kokkina
		if(rightchild->l->color != BLACK || rightchild->r->color != BLACK  || leftchild->l->color != BLACK || leftchild->r->color != BLACK){ //An yparxei kokkino eggoni
			if(h!=head)//Kanoume recoloring ektos ths rizas
				h->color=RED;
				leftchild->color=BLACK;
				rightchild->color=BLACK;
				return h;
}
			
if(leftchild->color==BLACK && rightchild->color==RED){ //Periptwsh pou to aristero paidi einai mauro kai to deksi kokkino
	if( rightchild->r->color == RED ){ //Periptwsh pou to aristero eggoni apo ta deksia einai mauro kai to deksi eggoni apo deksia kokkino 
		h->color=RED; //Allagh xrwmatos
		rightchild->color=BLACK; //Allagh xrwmatos
		h=rotL(h); //Peristrofh ston patera
		return h;
		}
	if(rightchild->l->color == RED){ //Periptwsh pou to aristero eggoni apo ta deskia einai kokkino kai to deksi eggoni apo deksia mauro
		h->color=RED; //Allagh xrwmatos
		rightchild->l->color=BLACK; //Allagh xrwmatos
		h->r=rotR(rightchild); // Peristrofh sto paidi
		h=rotL(h); //Peristrofh ston patera
		return h;
		}
}


if(leftchild->color==RED && rightchild->color==BLACK){ //Periptwsh pou to deksi paidi einai mauro kai to aristero kokkino
	if(leftchild->l->color == RED /*&& leftchild->r->color == BLACK*/ ){//Periptwsh pou to aristero eggoni apo ta aristera einai kokkino kai to deksi eggoni apo aristera mauro
		h->color=RED; //Allagh xrwmatos
		leftchild->color=BLACK; //Allagh xrwmatos
		h=rotR(h); //Peristrofh ston patera
		return h;
		}
	if( leftchild->r->color == RED ){ //Periptwsh pou to aristero eggoni apo ta aristera einai mauro kai to deksi eggoni apo deksia kokkino
		h->color=RED; //Allagh xrwmatos
		leftchild->r->color=BLACK; //Allagh xrwmatos
		h->l=rotL(leftchild); //Peristrofh sto paidi
		h=rotR(h); // Peristrofh ston patera
		return h;
		}
}

return h;

}

Link insertRnum(Link h, int* id,void** obj ,Link head, int allowDupl)
{   int v = *id;
    RBItem* t = h->rbitem;
    if (h == z) //Base case
	    if(h != head)
	    	return NEW((void*)id,obj, z, z, RED);  //Kathe neos komvos prepei na einai kokkinos
	    else 
			return NEW((void*)id,obj, z , z , BLACK); // Ektos apo thn riza pou einai maurh
    if( v < *(int *)(t->id) ){ //Psaxnoume thn thesh gia na valoume ton neo komvo
        h->l = insertRnum(h->l, id,obj,head,allowDupl);
        if(h->color==BLACK) //Kathe fora pou vriskoume mauro komvo pou exei toulaxiston ena "eggoni" apo aristera (afou to paidi exei bei aristera)
	    	h=MakeRBTree(h,head);
	}
    else if( *(int *)(t->id) < v ) {
		h->r = insertRnum(h->r, id,obj,head,allowDupl);
		if(h->color==BLACK) //Kathe fora pou vriskoume mauro komvo pou exei toulaxiston ena "eggoni" apo deksia (afou to paidi exei bei deksia)
			h=MakeRBTree(h,head);
	}
	else if(allowDupl)
			QueueInsert(&(h->rbitem->objs),obj);	//Eisagwgh sthn oura tou komvou
		
	return h;
  }
  
  
  Link insertRstr(Link h, char* id,void** obj ,Link head, int allowDupl)
{   
	char* v = id;
    RBItem* t = h->rbitem;
   
    if (h == z) //Base case
	    if(h != head)
	    	return NEW((void*)id,obj, z, z, RED);  //Kathe neos komvos prepei na einai kokkinos
	    else 
			return NEW((void*)id,obj, z , z , BLACK); // Ektos apo thn riza pou einai maurh
    if( strcmp(v,(char*)(t->id)) < 0 ){ //Psaxnoume thn thesh gia na valoume ton neo komvo
        h->l = insertRstr(h->l, id,obj,head,allowDupl);
        if(h->color==BLACK) //Kathe fora pou vriskoume mauro komvo pou exei toulaxiston ena "eggoni" apo aristera (afou to paidi exei bei aristera)
	    	h=MakeRBTree(h,head);
	}
    else if( strcmp((char*)(t->id),v) < 0 ) {
		h->r = insertRstr(h->r, id,obj,head,allowDupl);
		if(h->color==BLACK) //Kathe fora pou vriskoume mauro komvo pou exei toulaxiston ena "eggoni" apo deksia (afou to paidi exei bei deksia)
			h=MakeRBTree(h,head);
	}
	else if(allowDupl)
			QueueInsert(&(h->rbitem->objs),obj);	//Eisagwgh sthn oura tou komvou
		
	return h;
  }
  
Link RBTinsertR( Link head , void* id , void** obj , int type , int allowDupl ){ 

	if(type==0)			// numbers
		head = insertRnum( head , (int*)id , obj , head , allowDupl );
	else if(type==1)	//strings
		head = insertRstr( head , (char*)id , obj , head , allowDupl );
  	return head; 
}


Pair* findPair(Link h, int id, char* fullId){
   
    RBItem* t = h->rbitem;
    struct QueueNode* curr;
    Pair* pair;
    
    if(h == z)								// an den brethei timi 
		return NULL;
    
    if( id < t->id )						// diasxizoume to dentro gia na broume thn timi
		return findPair(h->l, id, fullId);
	else if (t->id < id)
		return findPair(h->r, id, fullId);
	else{									// otan tin broume elegxoume thn oura twn pairs
		for( curr = t->objs.head ; curr != NULL ; curr = curr->next){
			pair  = (Pair*)(curr->data);
			if(!strcmp(fullId,pair->item->id))
				return pair;
		}
		
		return NULL;
	}
    
    
	
}

void printOutput(Link h,FILE* output,char* buff){			
	
	RBItem* t = h->rbitem;
    struct QueueNode* qnptr, *curr, *prev=NULL, *temp;
 	
		
    Pair* pair;	
	Pair* rel_pair;	
	
	if(h == z)			// base-case
		return;
		
	printOutput(h->l,output,buff);	// anadromika phgainoume aristera
	
	for( qnptr = t->objs.head ; qnptr != NULL ; qnptr = qnptr->next){									// diasxizoume  thn oura twn pairs
	
		pair  = (Pair*)(qnptr->data);
		curr = pair->cliq->related->head;
		while( curr != NULL ){								// diasxizoume thn related 
			rel_pair  = (Pair*)(curr->data);															// related_pairs	
			if( !strcmp( pair->item->id , rel_pair->item->id ) ){										// an einai to idio item
			
				if (curr == pair->cliq->related->head ){														// an einai o head
					if( curr == pair->cliq->related->tail ){													// an einai to monadiko antikeimeno
						pair->cliq->related->head=NULL;														// diagrafh tou komvou
						pair->cliq->related->tail=NULL;
						pair->cliq->related->count--;
						temp=curr;
						curr = curr->next; 
						free(temp);
						free(pair->cliq->related); //To teleutaio pair katastrefei thn oura
						RBTdestrC(&(pair->cliq->unrelated)); //To teleutaio pair katastrefei to dentro
						free(pair->cliq);

					}
					else{																				// diaforetika o head tha deiksei ston epomeno komvo
						pair->cliq->related->head=curr->next;
						pair->cliq->related->count--;
						temp=curr;
						curr = curr->next; 
						free(temp);
					}
				}
				else if(curr == pair->cliq->related->tail){													// an einai to teleutaio item
					pair->cliq->related->tail = prev;															// tha metakinithei enan komvo pisw
					prev->next = NULL;
					pair->cliq->related->count--;
					temp = curr;
					curr = curr->next;
					free(temp);
				}
				else{																					// se opoiadhpote allh periptwsh
					prev->next=curr->next;
					pair->cliq->related->count--;
					temp = curr;
					curr = curr->next;
					free(temp);
				}
				
			}
			else{																						// an den einai ta ektypwnoume
				sprintf(buff,"%s , %s \n", pair->item->id , rel_pair->item->id );			
				fwrite(buff,sizeof(char),strlen(buff),output);
				prev = curr;
				curr = curr->next;
			}
			
		}
	
	}
	
	
	printOutput(h->r,output,buff);	// anadromika phgainoume deksia
	
	
	
	
}





void ItemDestroy(Item* item){
	Spec* spec;
	struct QueueNode* curr = item->specs.head,*Temp;
	
	
	while( curr != NULL ){
		spec = (Spec*)(curr->data);
		free(spec->name);
		free(spec->value);
		free(spec);
		Temp = curr;
		curr = curr->next;
		free(Temp);
	}
	
	free(item->id);
	free(item);
}


void PairDestroy(Pair* pair){
	
	
	ItemDestroy(pair->item);
	free(pair);
	
	
}



void RBTdestrP(Link* head)//Katastrofh tou dentrou zeugariwn
{
	struct QueueNode* curr,*Temp;
	Pair* pair;
	
    if (*head == z){
		*head = NULL;
		return;
	}
		
    RBTdestrP(&((*head)->l));
    RBTdestrP(&((*head)->r));
    
    curr = (*head)->rbitem->objs.head;
    
    while( curr != NULL){
    	pair = (Pair*)(curr->data);
		PairDestroy(pair);
		Temp = curr;
		curr = curr->next;
		free(Temp);
	}
    
    
    free((*head)->rbitem);
	free(*head);
	*head = NULL;
}



void RBTdestrC(Link* head)//Katastrofh tou dentrou klikwn
{
	Clique* cliq;
	
	
    if (*head == z){
		*head = NULL;
		return;
	}
		
    RBTdestrC(&((*head)->l));
    RBTdestrC(&((*head)->r));
    
    
    if( (*head)->rbitem->objs.head != NULL) //Den yparxoun diplotypa
		free((*head)->rbitem->objs.head );
    
    
    free((*head)->rbitem);
	free(*head);
	*head = NULL;
}






void RBTmerge(Link* head1,Link* head2)				//Merge duo dentrwn
{
	struct QueueNode* curr,*Temp;
	Pair* pair;
	
    if (*head2 == z){
		*head2 = NULL;
		return;
	}
		
    RBTmerge(head1,&((*head2)->l));
    RBTmerge(head1,&((*head2)->r));
    
    curr = (*head2)->rbitem->objs.head;
    
    while( curr != NULL){
    	pair = (Pair*)(curr->data);
    	
    	*head1 = RBTinsertR(*head1,(*head2)->rbitem->id,(void**)&pair,0);

		Temp = curr;
		curr = curr->next;
		free(Temp);
	}
    
    
    free((*head2)->rbitem);
	free(*head2);
	*head2 = NULL;
}


void CliqueConcat(Pair* pair1 , Pair* pair2, int choice){
	Clique* temp;
	
	
	if(choice == 1){	// dld an tairiazoun
		temp = pair2->cliq;
		RBTmerge(&(pair1->cliq->unrelated),&(pair2->cliq->unrelated));
		QueueConcat(pair1->cliq->related,pair2->cliq->related,pair1->cliq);
		free(temp);
		}
	else{				// dld den tairiazoun
		pair1->cliq->unrelated = RBTinsertR(pair1->cliq->unrelated,getNumId2(pair2->item->id),(void**)&pair2,0);	
		pair2->cliq->unrelated = RBTinsertR(pair2->cliq->unrelated,getNumId2(pair1->item->id),(void**)&pair1,0);
	}
	
}

/*void QueueDestroy(Queue * queue){//Katastrofh ths listas

	struct ListNode * Temp;
	
	while(list->head != NULL){
       Temp=list->head;
       list->head=list->head->next;
       free(Temp);
    }

}*/
void MakeCliqueTree(Link pairTree, Link* cliqueTree){
	
	RBItem* t = pairTree->rbitem;
    struct QueueNode* qnptr;
 			
    Pair* pair;	

	
	if(pairTree == z)			// base-case
		return;
		
	MakeCliqueTree(pairTree->l,cliqueTree);	// anadromika phgainoume aristera
	
	for( qnptr = t->objs.head ; qnptr != NULL ; qnptr = qnptr->next){									// diasxizoume  thn oura twn pairs
	
		pair  = (Pair*)(qnptr->data);
		*cliqueTree = RBTinsertR(*cliqueTree,pair->cliq->id,(void**)&(pair->cliq),0);
	
	}

	MakeCliqueTree(pairTree->r,cliqueTree);	// anadromika phgainoume deksia

}


void MakeCliqueUnrelated(Link* oldTree, Link* newTree){
	
	RBItem* t = (*oldTree)->rbitem;
 			
    Pair* pair;	

	
	if(*oldTree == z)			// base-case
		return;
		
	MakeCliqueUnrelated(&((*oldTree)->l),newTree);	// anadromika phgainoume aristera
	MakeCliqueUnrelated(&((*oldTree)->r),newTree);	// anadromika phgainoume deksia

	
	if(t->objs.head != NULL){									// Den yparxoun diplotypa
	
		pair  = (Pair*)(t->objs.head->data);
		*newTree = RBTinsertR(*newTree,pair->cliq->id,(void**)&(pair->cliq),0);
		free(t->objs.head);
	}


	free((*oldTree)->rbitem);
	free(*oldTree);
	*oldTree = NULL;
	

}



void ChangeUnrelated(Link h){
	
	RBItem* t = h->rbitem;
    struct QueueNode* qnptr;		
    Clique* cliq;	
	Link unrelated;
	
	
	if(h == z)			// base-case
		return;
	
	ChangeUnrelated(h->l);	// anadromika phgainoume aristera
	
	RBTinit(&unrelated);
	
	if(t->objs.head != NULL){									// Den yparxoun diplotypa
	
		cliq  = (Clique*)(t->objs.head->data);
		MakeCliqueUnrelated(&(cliq->unrelated),&unrelated);
		cliq->unrelated = unrelated;
	
	}
	
	
	ChangeUnrelated(h->r);	// anadromika phgainoume aristera
	
}




void RemoveUnrelated(Link h , int id){
	
	
	RBItem* t = h->rbitem;
    Clique* cliq;
    
    if(h == z)								// an den brethei timi 
		return;
    
    if( id < t->id )						// diasxizoume to dentro gia na broume thn timi
		return RemoveUnrelated(h->l, id);
	else if (t->id < id)
		return RemoveUnrelated(h->r, id);
	else{									// otan tin broume elegxoume thn oura twn pairs
			if(t->objs.head != NULL){
				cliq  = (Clique*)(t->objs.head->data);
				if(id == cliq->id){
					free(t->objs.head);
					t->objs.head = NULL;
					t->objs.tail = NULL;
					t->objs.count = 0;	
				}
			}

	
	}
	
}






void VisitUnrelated(Link h, Clique* cliq,FILE* output,char* buff){
	
	RBItem* t = h->rbitem;
    struct QueueNode* ptr,*unrelptr;		
    Clique* unrelc;
	Pair* pair, *unrelp;	
	
	if(h == z)			// base-case
		return;
	
	VisitUnrelated(h->l,cliq,output,buff);	// anadromika phgainoume aristera
	
	
	if(t->objs.head != NULL){									// Den yparxoun diplotypa
	
		unrelc  = (Clique*)(t->objs.head->data);
		
		for( ptr = cliq->related->head ; ptr != NULL ; ptr = ptr->next ){
			pair = (Pair*)(ptr->data);
			for( unrelptr = unrelc->related->head ; unrelptr != NULL ; unrelptr = unrelptr->next){
				unrelp = (Pair*)(unrelptr->data);
				sprintf(buff,"%s , %s \n", pair->item->id , unrelp->item->id );
				fwrite(buff,sizeof(char),strlen(buff),output);
			}	
		}
		
		RemoveUnrelated(unrelc->unrelated, cliq->id);
	
	}
	
	
	VisitUnrelated(h->r,cliq,output,buff);	// anadromika phgainoume aristera
	
}





void printUnrelated(Link h,FILE* output,char* buff){
	
	RBItem* t = h->rbitem;		
    Clique* cliq;	
	
	if(h == z)			// base-case
		return;
	
	printUnrelated(h->l,output,buff);	// anadromika phgainoume aristera
	
	
	if(t->objs.head != NULL){									// Den yparxoun diplotypa
									
		cliq  = (Clique*)(t->objs.head->data);
		VisitUnrelated(cliq->unrelated,cliq,output,buff);
		
	}
	
	
	printUnrelated(h->r,output,buff);	// anadromika phgainoume aristera
	
}


/*##################                  Start of hash tables                             ##########################*/

void HTinit( HashTable* ht, int numBuckets ){			// initialise of hash table
	
	int i;
	
	ht->buckets=(Link*)malloc(sizeof(Link)*numBuckets); 
	for( i = 0; i < numBuckets; i++ )					// each hash table has a tree
		RBTinit( &(ht->buckets[i]) );
	
}

int hashFunctionStr(char* str,int numBuckets){ //Polynomial hash function for strings
	int hash=0;
	int constant=33;
	while(*str != '\0'){
		hash=(constant*hash + *str) % numBuckets;
		str++;
	}
	return hash;
}

int hashFunctionNum(int num,int numBuckets){ //Polynomial hash function for numbers
	
	return ( num % numBuckets );

}



void HTinsert( HashTable* ht, int numBuckets, void* key, void** item, int type, int allowDupl ){			
	
	int hashnum;
	
	if ( type == 0 ){					// akeraioi
		hashnum = hashFunctionNum ( *(int*)key , numBuckets );
		ht->buckets[hashnum] = RBTinsertR( ht->buckets[hashnum] , key , item , type , allowDupl );
		
	}
	else if ( type == 1 ){				//strings
		hashnum = hashFunctionStr ( (char*) key , numBuckets );
		ht->buckets[hashnum] = RBTinsertR( ht->buckets[hashnum] , key , item , type , allowDupl );
	}
	
}


/*##################                  End of hash tables                               ##########################*/




