#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "./include/structs.h"
#include "./include/text_support.h"


/*
void CutOffDictionary(HashTable* words,int limit){
	int i,*num;
	Heap heap;
	Details* details;
	
	HeapInit(&heap);
	
	for(i = 0 ; i < numBuckets; i++)
		HeapifyWords(&(words->buckets[i]),&heap);

	free(words->buckets);
	HTinit(words);
		
	for(i = 0; i < limit; i++)
		if( details = HeapRemoveFirst(&heap)){
			num = (int*)malloc(sizeof(int));
			*num = i;
			HTinsert(words,details->name,(void*)num);
			free(details);
		}
		else
			break;
	
	HeapDestroy(&heap);
		
}
*/

void CutOffDictionary( HashTable* words , int limit ){
	
	int i;
	Heap heap;
	Details* details;
	
	HeapInit(&heap);
	
	for(i = 0 ; i < numBuckets; i++)
		HeapifyWords(&(words->buckets[i]),&heap);

	free(words->buckets);
	HTinit(words);
		
	for(i = 0; i < limit; i++)
		if( details = HeapRemoveFirst(&heap)){
			details->wstats->index = i;
			HTinsert(words , details->name, (void*)details->wstats );
			free(details);
		}
		else
			break;
	
	HeapDestroy(&heap);
		
}


void CreateDictionary(FileStats* fstats , HashTable* words ,HashTable* stopwords,int* index){

	struct QueueNode* curr;
	Spec* spec;


	for( curr = fstats->item->specs.head; curr != NULL ; curr = curr->next){
		spec = (Spec*)(curr->data);
		textCleaning(spec->name);
		textCleaning(spec->value);
		tokenize(spec->name, words, stopwords , fstats , index );
		tokenize(spec->value, words, stopwords ,fstats ,index );		
	}

}





void textCleaning(char* text){

	int i;
	int backslash = 0;

	for( i = 0  ; i < strlen(text) ; i++)
		if( isalnum(text[i]) && backslash)
			text[i] = ' ';
    	else if( isalpha(text[i]) && isupper(text[i]) )
    		text[i] = tolower(text[i]);
    	else if(text[i] == '\\'){
    		backslash = 1;
    		text[i] = ' ';
    	}
    	else if( isspace(text[i]) && backslash)
    		backslash = 0;
    	else if ( ispunct(text[i]) || !isprint(text[i]) )
    		text[i]=' ';

	
}


void InsertWord(HashTable* words, HashTable* stopwords , char* temp,FileStats* fstats,int* index )
{
	char *token;
	int* num;
	WordStats* wstats;
	ModelStats* mstats;
	
		if(HTfind(stopwords,temp,'k') == NULL)
			if( wstats = (WordStats*)HTfind(words, temp ,'v')){
				if(mstats = (ModelStats*) HTfind(&(fstats->words),wstats->word,'v')){
					mstats->bow_val++;
					mstats->tfidf_val++;
					fstats->numOfWords++;
				}
				else{
					mstats = (ModelStats*)malloc(sizeof(ModelStats));
					mstats->wstats = wstats;
					mstats->bow_val = 1;
					mstats->tfidf_val = 1.0;
					HTinsert(&(fstats->words),wstats->word,(void*)mstats);
					fstats->numOfWords++;
					HTinsert(&(wstats->files),fstats->item->id,(void*)fstats);
				}

			}
			else{
				
				wstats = (WordStats*)malloc(sizeof(WordStats));
				wstats->word = (char*)malloc(strlen(temp) + 1);
				strcpy(wstats->word,temp);
				wstats->index = (*index)++;
				HTinit(&(wstats->files));

				HTinsert(words, wstats->word, (void*) wstats);
				
				mstats = (ModelStats*)malloc(sizeof(ModelStats));
				mstats->wstats = wstats;
				mstats->bow_val = 1;
				mstats->tfidf_val = 1.0;
				HTinsert(&(fstats->words),wstats->word,(void*)mstats);
				fstats->numOfWords++;
				HTinsert(&(wstats->files),fstats->item->id,(void*)fstats);	
			}
			
}




void tokenize(char* text, HashTable* words, HashTable* stopwords ,FileStats* fstats,int* index){

	int i = 0,start,count = 0,j;
	char temp[100];
	
	while(isspace(text[i]))
		i++;
	
	for( ; i < strlen(text) + 1 ; i++){
		if(!isspace(text[i]) && text[i] != '\0'){
			if(count == 0)
				start = i;				
			count++;		
		}
		else if( count > 0){
				if( count > 1){
					//token = (char*)malloc(count + 1);
					for(j = start; j < start+count; j++)
						temp[j-start] = text[j];
					temp[count] = '\0';
					
					
					InsertWord( words, stopwords , temp, fstats, index );

				}
				count = 0;
		}
		
	}
		
	
}

void read_stopwords(HashTable* ht, char* stopwordsFile){
	
	int i,hashnum;
	FILE * stopwords_file = fopen(stopwordsFile,"r"); 
	char* token;
	char line[50];
	
	if(stopwords_file == NULL){
		printf("Stopwords file is empty!\n");
		return;
	}
			
	while( fgets( line , sizeof(line) , stopwords_file ) ){
		
		token=(char*)malloc(strlen(line));
		strncpy(token,line,strlen(line));
		token[strlen(line) - 1 ] = '\0';

		HTinsert( ht  , token, (void*) token);

	}
	

	fclose(stopwords_file);
}






double sigmoid(double f){

	return 1.0/(1.0 + exp(-f));

}


double LRpred(LogisticRegression* lr,double* v, int size){
	int j;
	int f = lr->weights[0]; //Arxikopoihsh me ton stathero oro
	
	for( j = 0; j < size; j++ )
		f += lr->weights[j + 1] * v[j]; //w^T*xi
	
	return sigmoid(f);
}



void LRtrain(LogisticRegression* lr,double** X ,int rows,int cols ,int* y){

	int i,j,t;
	double f,error;

	lr->weights = (double*)malloc(sizeof(double)*(cols + 1)); // Dianysma me varh
	double* wtmp = (double*)malloc(sizeof(double)*(cols + 1)); //Dianysma me ta prohgoymena varh

	for( i = 0; i < cols + 1;  i++){ //Arxikopoihsh dianysmatwn
		lr->weights[i] = 0.0;
		wtmp[i] = 0.0;
	}

	for( t = 0; t < maxIters; t++){
		
		for( i = 0; i < rows ; i++){
			
			error = LRpred(lr,X[i],cols) - y[i]; // sigmoid(w^T*xi + b) - yi
			
			lr->weights[0] -= lrate * error;
			for( j = 0; j < cols ; j++)
				lr->weights[j + 1] -= lrate * error * X[i][j]; // wj = wj - learningRate * sum((sigmoid(w^T * xi + b) - yi) * xij)

		}

		f = 0.0;
		for( j = 0; j < cols + 1 ; j++){
			f += (lr->weights[j] - wtmp[j])*(lr->weights[j] - wtmp[j]);
			wtmp[j] = lr->weights[j];
		}

		if(sqrt(f) < epsilon)
			break;

	}

	free(wtmp);

}


Item* parse(char* json){
	char* tmp, c, prev = ' ';
	int i, j, start = 0, flag = 0, count = 0, isValue = 0 ,isPrim = 0;
	FILE* fd;
	Item* item;
	Spec* sp;
	Stack stack;
	
	
	fd = fopen(json,"r");	//Anoigma tou json
	if(fd == NULL) 
		return NULL;
		
		
	item = (Item*) malloc(sizeof(Item)); //Dhmiourgia antikeimenou
	QueueInit(&(item->specs)); //Arxikopoihsh listas stoixeiwn antikeimenou
	
	for( i = 0; json[i] != '/'; i++) //Megethos arxikou katalogou
		count++;
	
	item->id = (char*) malloc(strlen(json) - count - 4 ); //Megethos xwris thn katalhksh .json kai to onoma tou arxikou katalogou
	item->id[strlen(json) - count - 5] = '\0';
	
	for( i = count + 1 , j = 0; i < strlen(json) - 5 ; i++ ){  //Xwris thn katalhksh .json kai to onoma tou arxikou katalogou
		(item->id)[j++] = json[i];
		if(json[i] == '/')
			(item->id)[j++] = '/'; //Diplo slash
	}
		
	count = 0;
	StackInit(&stack);
	while(!feof(fd)){
		
		c = fgetc(fd);
		
		if(!isValue){ // An den einai h seira tou diavasmatos ths timhs(dhladh einai to onoma tou spec)
			if( c == '"' && flag == 0){ //Otan vrethei to arxiko eisagwgiko(")
				start = count; //Apothikeush ths theshs
				flag = 1 ; //To eisagwgiko den exei kleisei
			}
			else if ( c == '"' && prev != '\\' && flag == 1){ //An einai to eisagwgiko(") kleisimatos
				tmp = (char*)malloc((count - start - 1)*sizeof(char) + 1); //Desmeush xwrou gia thn leksh anamesa sta eisagwgika
				fseek(fd,start + 1,SEEK_SET); //Arxh diavasmatos meta to arxiko eisagwgiko
				for( i = 0; i < count - start - 1; i++)
					tmp[i] = fgetc(fd);
				tmp[count - start - 1] = '\0';
				fseek(fd,count + 1,SEEK_SET); //Diavasma meta to eisagwgiko kleisimatos
				sp = (Spec*)malloc(sizeof(Spec)); //Dhmiourgia neou spec
				sp->name = tmp; // Prosthikh onomatos
				isValue = 1; // Seira ths timhs
				start = -1;
				
				flag = 0; //To eisagwgiko exei kleisei
			} 
		}
		else{
			if(start == -1){ //Mexri na vrethei h arxh(start)
				if( c != ':' && c != ' '){
					if( (c == '"'  || c == '{' || c == '[') && prev != '\\'){
						push(&stack,c);
						isPrim = 0;
					}
					else
						isPrim=1;
					start = count;
				}
			}
			else{
				if(!StackEmpty(&stack)){
					if((c == '[' || c == '{'|| c == '"'  || c == ']' || c == '}') && prev != '\\')
						check(&stack,c);
				}
				else if(isPrim == 0){
					tmp = (char*)malloc((count - start)*sizeof(char) + 1); //Desmeush xwrou gia thn timh
					fseek(fd,start,SEEK_SET); //Arxh diavasmatos apo to start
					for( i = 0; i < count - start; i++)
						tmp[i] = fgetc(fd);
					tmp[count - start] = '\0';
					fseek(fd,count + 1,SEEK_SET); //Diavasma apo ekei pou stamathse o elegxos
				//	printf("%s\n",tmp);
					sp->value = tmp; // Prosthikh timhs
					QueueInsert(&(item->specs),(void**)&sp);
					isValue = 0; //Seira tou onomatos
				}
				else{
					if( c == ',' || c ==' '){
						tmp = (char*)malloc((count - start)*sizeof(char) + 3);
						fseek(fd,start,SEEK_SET); //Arxh diavasmatos apo to start
						tmp[0] = '"';
						for( i = 1 ; i < count - start + 1 ; i++)
							tmp[i] = fgetc(fd);
						tmp[count - start + 1] = '"';
						tmp[count - start + 2] = '\0';
						fseek(fd,count + 1,SEEK_SET); //Diavasma apo ekei pou stamathse o elegxos
					//	printf("%s\n",tmp);
						sp->value = tmp; // Prosthikh timhs
						QueueInsert(&(item->specs),(void**)&sp);
						isValue = 0; //Seira tou onomatos
						
					}	
				}	
			}
		}
			
		count++;
		prev = c;
	}
	
	
	fclose(fd);
	return item;
	
		
}


void read_csv(HashTable* ht,char* datasetW){
	
	int i,hashnum;
	FILE * csv_file = fopen(datasetW,"r"); 
	char* token;
	char line[400];
	Pair* pairA,* pairB;
	
	if(csv_file == NULL){
		printf("Csv file is empty!\n");
		return;
	}

	fgets(line, sizeof(line), csv_file); //Diavasma twn etiketwn tou csv

			
	while(fgets(line, sizeof(line), csv_file)){
		
		token = strtok(line,",");
		
		for(i = 0; (i < 3 && token != NULL) ; i++ ){
			if( i == 0 ){
				pairA = (Pair*) HTfind(ht,token,'v'); //Euresh tou left item	
			}
			if(i == 1){
				pairB = (Pair*) HTfind(ht,token,'v');  //Euresh tou right item
			}
			if( i == 2)
				if(pairA != NULL && pairB != NULL)
					if(atoi(token) == 1){ //An tairiazoun 
						if(pairA->cliq->related != pairB->cliq->related) //An den exoun enwthei ksana
							CliqueConcat(pairA, pairB, 1);
					}
					else{				// alliws sthn periptwsh tou 0 (dld dn tairiazoun)
						CliqueConcat(pairA, pairB, 0);
					}
			
			
			token = strtok(NULL, ",");			// continue to tokenize the string we passed first
		}
	}

	fclose(csv_file);
	
}




