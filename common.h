#ifndef _COMMON_H_
#define _COMMON_H_

// Name for the queue of messages going to the server.
#define SERVER_QUEUE "/glbrook2-server-queue"

// Name for the queue of messages going to the current client.
#define CLIENT_QUEUE "/glbrook2-client-queue"

// Maximum length for a message in the queue
// (should be long enough for any resonable encoding of messages)
#define MESSAGE_LIMIT 1024

// Maximum name length.
#define NAME_MAX 12

// Minimum number of names on the list.
#define LIST_MIN 2

// Maximum number of names on the list.
#define LIST_MAX 6

typedef struct NodeStruct Node;

// Node for linked list
struct NodeStruct {
  char *str;
  Node *next;
  Node *prev;
};

// Representation for the ordered list of names.
typedef struct List {
  Node *head;
  Node *tail;
} RankList;

#endif
