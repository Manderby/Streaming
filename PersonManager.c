
// This is a file submitted by Twitch follower "rebundo" during a programming
// stream. His example was originally just the Person class where he proudly
// showed how he managed to get a grip on the concept of "encapsulation".
//
// Then, during the stream, I provided some feedback and started refactoring
// and improving the code. In the end we added a Manager, made everything
// const-safe, ensured safety using nullptr guards and startet a small testing
// function.

// This is the last state of the code, more or less untouched for everyone to
// download and look at. Have fun!
//
// Note that the original comments have been erased.



//person.h
#ifndef PERSON_H
#define PERSON_H

typedef struct Person Person;

Person* createPerson(const char* name);
//void setFriend(Person* person, Person* friend);
//void setFriendship(Person* person1, Person* person2);
void printPerson(const Person* person);
void deletePerson(Person* person);

#endif



//person.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "person.h"

struct Person {
	char name[50];
	//Person* friendWithBenefits;
	const Person* friend;
	//Person const * alsoAFriend;
  //Person const * const myFiance;
  //Person * const bff;
};

//void setFriend(Person* person, Person* friend) {
//	person->friend = friend;
//}

//void setFriendship(Person* person1, Person* person2) {
//	person1->friend = person2;
//	person2->friend = person1;
//}

//void destroyFriendship(Person* person1, Person* person2) {
//	person1->friend = NULL;
//	person2->friend = NULL;
//}

Person* createPerson(const char* name) {
  Person* newPerson = (Person*)malloc(sizeof(Person));

	size_t len = strlen(name);
	if(len > 50 - 1) { len = 50 - 1;}
	strncpy(newPerson->name, name, len);
	newPerson->name[len] = '\0';

	newPerson->friend = NULL;

	return newPerson;
}


void printPerson(const Person* person) {
  if(person == NULL) {
    printf("Person not found\n");
  }else{
    printf("Name: %s\n", person->name);
    if (person->friend != NULL) {
      printf("Friend: %s\n", person->friend->name);
    } else {
      printf("No friend assigned.\n");
    }
  }
}


void deletePerson(Person* person) {
//	if(person->friend != NULL) {
//    person->friend->friend = NULL;  // not allowed!
//  }
  free(person);
}



// personManager.c
#define ARRAY_COUNT 2
typedef int bool;

typedef struct PersonManager PersonManager;
struct PersonManager {
  Person* person[ARRAY_COUNT];
};

bool validateManager(const PersonManager* manager) {
  bool valid = manager != NULL;
  if(!valid)
      printf("Manager object is NULL\n");
  return valid;
}
bool validateIndex(size_t index) {
  bool valid = index < ARRAY_COUNT;
  if(!valid)
      printf("Index out of bounds\n");
  return valid;
}



PersonManager* createPersonManager() {
  PersonManager* manager = malloc(sizeof(PersonManager));
  for(size_t i = 0; i < ARRAY_COUNT; ++i) {
    manager->person[i] = NULL;
  }
  return manager;
}

void setManagerPerson(PersonManager* manager, Person* person, size_t index) {
  if(!validateIndex(index))
    return;
    
  if(manager->person[index] != NULL) {
    printf("There is already a person at index %d\n", (int)index);
    return;
  }
  
  manager->person[index] = person;
}

const Person* getManagerPerson(PersonManager* manager, size_t index) {
  if(!validateManager(manager))
    return NULL;
  if(!validateIndex(index))
    return NULL;
  
  return manager->person[index];
}

void setManagerFriendship(PersonManager* manager, size_t indexA, size_t indexB) {
  if(!validateIndex(indexA) || !validateIndex(indexB))
    return;

  if(manager->person[indexA] == NULL || manager->person[indexB] == NULL) {
    printf("Friendship not possible, as persons do not exists.\n");
    return;
  }

  manager->person[indexA]->friend = manager->person[indexB];
  manager->person[indexB]->friend = manager->person[indexA];
}

void printManagerPerson(PersonManager* manager, size_t index) {
  if(!validateIndex(index))
    return;

  printPerson(manager->person[index]);
}

void deleteManagerPerson(PersonManager* manager, size_t index) {
  if(!validateIndex(index))
    return;

  if(manager->person[index] == NULL)
    return;
    
  const Person* friendPerson = manager->person[index]->friend;
  
  // not allowed:
  // friendPerson->friend = NULL;
  
  if(friendPerson != NULL) {
    for(size_t i = 0; i < ARRAY_COUNT; ++i) {
      if(manager->person[i] == friendPerson) {
        manager->person[i]->friend = NULL;
      }
    }
  }
  
  deletePerson(manager->person[index]);
  manager->person[index] = NULL;
}

void deletePersonManager(PersonManager* manager) {
  for(size_t i = 0; i < ARRAY_COUNT; ++i) {
    if(manager->person[i] != NULL) {
      deleteManagerPerson(manager, i);
    }
  }
  free(manager);
}



void test() {
  PersonManager* manager = createPersonManager();
  if(getManagerPerson(NULL, 0) != NULL) {printf("Error\n");}
  if(getManagerPerson(manager, 0) != NULL) {printf("Error\n");}
  if(getManagerPerson(manager, 1) != NULL) {printf("Error\n");}
  if(getManagerPerson(manager, ARRAY_COUNT) != NULL) {printf("Error\n");}
  
  deletePersonManager(manager);
}



// main.c
//#include "person.h"

int main() {
  test();

  PersonManager* manager = createPersonManager();
  
  setManagerPerson(manager, createPerson("Alice"), 0);
  setManagerPerson(manager, createPerson("Bob"), 1);

  setManagerFriendship(manager, 0, 1);

	printManagerPerson(manager, 0);
	printManagerPerson(manager, 1);

  deleteManagerPerson(manager, 1);

	printManagerPerson(manager, 0);

  deletePersonManager(manager);

	return 0;
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
