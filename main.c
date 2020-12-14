#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include "./include/structs.h"
#include "./include/text_support.h"






int main(int argc, char* argv[]){
	int i, j, id = 0 ,fIndex = 0;
	char* datasetX=NULL, *datasetW=NULL, *stopwordsFile=NULL, *tmpdir1, *json, *tmp;
	char buff[200];
	DIR* dir_ptr1,*dir_ptr2;
	FILE* output;
	struct dirent* dirent_ptr;
	Item* item;
	Stats* fileStats;
	Pair *pair;
	HashTable pairs;
	HashTable cliques;
	HashTable words;
	HashTable stopwords;
	HashTable stats;
	
	
	if(argc != 7){
		printf("Argument error\n");
			return 1;
	}
	
	for(i = 1 ; i < argc ; i += 2){ //Den mas endiaferei h seira pou dinontai kai den epitrepei tis epanalhpseis twn parametrwn

		if(!strcmp("-x",argv[i]) && datasetX == NULL)
			datasetX=argv[i+1];
		else if(!strcmp("-w",argv[i]) && datasetW == NULL)
			datasetW=argv[i+1];
		else if(!strcmp("-s",argv[i]) && stopwordsFile == NULL)
			stopwordsFile = argv[i+1];
		else{
			printf("Argument error\n");
			return 1;
		}
	}
	
	// Trees initialise
	RBinit();

	//Hash Table initialise
	HTinit(&cliques);
	HTinit(&pairs);
	HTinit(&stopwords);
	HTinit(&words);
	HTinit(&stats);

	
	//Read Stopwords file
	read_stopwords(&stopwords , stopwordsFile);




	dir_ptr1 = opendir(datasetX);
	while(dirent_ptr = readdir(dir_ptr1)) //Diavasma twn katalogwn
		if(strcmp(dirent_ptr->d_name,".") && strcmp(dirent_ptr->d_name,"..")){
			tmpdir1 = (char*)malloc(strlen(datasetX) + strlen(dirent_ptr->d_name) + 2);
			strcpy(tmpdir1,datasetX);
			strcat(tmpdir1,"/");
			strcat(tmpdir1,dirent_ptr->d_name);
			dir_ptr2 = opendir(tmpdir1);
			while(dirent_ptr = readdir(dir_ptr2)) //Diavasma twn json arxeiwn
				if(strcmp(dirent_ptr->d_name,".")  && strcmp(dirent_ptr->d_name,"..")){
					json = (char*)malloc(strlen(tmpdir1) + strlen(dirent_ptr->d_name) + 2);
					strcpy(json,tmpdir1);
					strcat(json,"/");
					strcat(json,dirent_ptr->d_name); 
					//printf("%s\n",json);
					if( item = parse(json) ){						// an epistrefetai to item dhmiourgeitai to pair (to opoio prepei na bei sthn domh apothikeushs twn pairs)
									
						fileStats = (Stats*)malloc(sizeof(Stats)); //Dhmiourgia stats tou arxeiou
						fileStats->item = item;
						fileStats->index = fIndex++;
						CreateDictionary(fileStats, &words, &stopwords);
						HTinsert(&stats, item->id , (void*)fileStats);


						pair = (Pair*)malloc(sizeof(Pair));
						pair->item = item;
						
						pair->cliq = (Clique*) malloc(sizeof(Clique));
						sprintf(buff,"%d",id);
						tmp = (char*)malloc(strlen(buff) + 1 );
						strcpy(tmp,buff);
						pair->cliq->id = tmp;
						id++;
						
						pair->cliq->related = (Queue*)malloc(sizeof(Queue));
						HTinit(&(pair->cliq->unrelated));
						
						QueueInit(pair->cliq->related);
						QueueInsert(pair->cliq->related, (void**)&pair); // Sthn arxh h related oura exei mono to idio to pair 
						
							
	 					HTinsert(&pairs,item->id,(void*)pair);
	 					
	 					
					}
						
					free(json);
				}
			free(tmpdir1);
			closedir(dir_ptr2);
		}
			
	closedir(dir_ptr1);
	
	
	// CSV READ
	
	read_csv(&pairs,datasetW);
	
	for( i = 0; i < numBuckets; i++)
		MakeCliqueHT(pairs.buckets[i],&cliques);
	
	for( i = 0; i < numBuckets; i++)
		ChangeUnrelated(cliques.buckets[i]);
	
	
	output = fopen("unrelated.csv","w");
	for( i = 0; i < numBuckets; i++)
		printUnrelated(cliques.buckets[i],output,buff);
	fclose(output);
	
	output = fopen("related.csv","w");
	sprintf(buff,"left_item , right_item\n");
	fwrite(buff,sizeof(char),strlen(buff),output);
	for( i = 0; i < numBuckets; i++)
		printRelated(pairs.buckets[i],output,buff);
	fclose(output);
	
	
	CutOffDictionary(&words,lim);
	
	
	//Bow Array
	double** bow_array = (double**)malloc( sizeof(double*) * fIndex);
	for( i = 0 ; i < fIndex ; i++ )
		bow_array[i] = (double*)malloc( sizeof(double) * lim ); 
	
	// Arxikopoihsh me 0		
	for( i = 0 ; i < fIndex ; i++ )
		for( j = 0 ; j < lim ; j++ )
			bow_array[i][j] = 0.0;
			
	for( i = 0; i < numBuckets; i++)
		CreateArray( stats.buckets[i], &words, bow_array );		
	





	double** tfidf_array = Bow_To_Tfidf(bow_array, fIndex ,lim);

	for( i = 0 ; i < fIndex ; i++ ){
		free(bow_array[i]);
		free(tfidf_array[i]);
	}
	free(bow_array);
	free(tfidf_array);
	


	HTdestr(&pairs,&PairDestroy,'v');
	HTdestr(&cliques,&CliqueDestroy,'v');
	HTdestr(&stats,NULL,'v');
	HTdestr(&words,NULL,'b');
	HTdestr(&stopwords,NULL,'k');
	RBdestr();
	
	return 0;
	
}


