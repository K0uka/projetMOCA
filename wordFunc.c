#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "dico.h"
#include "globals.h"
#include "macro.h"
#include "charFunc.h"
#include "conversion.h"
#include "maillon.h"
#include "wordFunc.h"

void displayWord(mot_t* word, FILE *filedes) {
  if (word == NULL) {
  } else {
    maillon_t* link = word->tete_mot;
    emplacement_t* list = word->tete_liste;
    if (link != NULL) {
      fprintf(filedes,"%s",maillonToString(link));
      fflush(stdout);
    }
    while(list != NULL) {
      fprintf(filedes," (%i,%i)",list->line,list->colonne);
      list = list->next;
    }
    free(link);
    free(list);
  }
}

char *next_word(FILE *f, unsigned int *nblin, unsigned int *nbcol){
  char s[100]; 
  char *res; 
  unsigned int i=0, startl = current_line, startc = current_col;
  char sep;
  sep = fgetc(f);
  while (strchr(separators,sep) != NULL  || sep == '\n') { 
    startc++;
    if (sep == '\n'){ 
      startl++; startc = 1;
    } 
    sep = fgetc(f);
  } 
  ungetc(sep,f);
  if (nblin != NULL) *nblin = startl;  
  if (nbcol != NULL) *nbcol = startc;
  while ((strchr(separators, s[i]=fgetc(f)) == NULL) && s[i] != '\n'){
    i++; startc++;
  }
  sep = s[i]; 
  s[i] = '\0';
  res = (char *)malloc(strlen(s)+1); 
  strcpy(res,s);
  while (strchr(separators,sep) != NULL  || sep == '\n') { 
    startc++;
    if (sep == '\n'){  
      startl++; startc = 1;
    } 
    sep = fgetc(f);
  } 
  ungetc(sep,f);
  current_line = startl;
  current_col = startc;
  return res;
}


int compareWord(mot_t* w1, mot_t* w2) {
  if (w1 == NULL) {
    return 1;
  } else if (w2 == NULL) {
    return -1;
  } else {
    char* word1 = maillonToString(w1->tete_mot);
    char* word2 = maillonToString(w2->tete_mot);
    int minSize = (strlen(word1)<strlen(word2))?strlen(word1):strlen(word2);
    int i;//,j = 0;
    int pos = 0;
    while(i<minSize && pos == 0) {
      pos = (word1[i]<word2[i])?-1:(word1[i]>word2[i])?1:0;
      i++;
    }
    return (pos == 0 && strlen(word1) < strlen(word2))?-1:(pos == 0 && strlen(word1) > strlen(word2))?1:pos;
  }
}


void addTailWord(dico* dictionary, mot_t* linkWord) {
  dico* newDictionary = (dico*) malloc(sizeof(dico));
  newDictionary->mot = linkWord;
  newDictionary->next = NULL;
  if(dictionary->next == NULL) {
    dictionary->next = newDictionary;
  } else {
    newDictionary->next = dictionary->next;
    dictionary->next = newDictionary;
  }
}


void addHeadWord(dico* dictionary, mot_t* linkWord) {
  dico* newDictionary = (dico*) malloc(sizeof(dico));
  newDictionary->mot = linkWord;
  newDictionary->next = dictionary;
  dictionary = newDictionary;
}

void incWord(emplacement_t* location, unsigned int line, unsigned int colonne) {
  emplacement_t* newLocation = (emplacement_t*) malloc(sizeof(emplacement_t));
  emplacement_t* tempLocation = (emplacement_t*) malloc(sizeof(emplacement_t));
  tempLocation = location;
  newLocation->next = NULL;
  newLocation->line = line;
  newLocation->colonne = colonne;
  while(tempLocation->next != NULL) {
    tempLocation = tempLocation->next;
  }
  tempLocation->next = newLocation;
}
