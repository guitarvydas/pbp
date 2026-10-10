// hardware level types
typedef char* Str;
typedef unsigned char Byte;
typedef unsigned char Bool;
typedef int Index;



// DI level types
typedef Str Port;
typedef Str Payload;

typedef Bool Byte;
#define FALSE 0
#define TRUE 1

typedef Byte Dir;
#define Down 0
#define Across 1
#define Up 2
#define Through 3

typedef struct s_Eh Eh;
typedef struct s_Eh Part;
typedef struct s_Eh Leaf;
typedef struct s_Eh Container;

typedef struct s_Mevent Mevent;
typedef struct s_Datum Datum;
typedef Datum* (*Fclone) (Datum*);
typedef void (*Freclaim) (Datum*);
typedef void (*Fhandler) (Eh*, Mevent*);
typedef void (*Finject) (Eh*, Mevent*);
typedef void (*Freset) (Eh*);
typedef void Any;

#include "mevent.h"

typedef struct s_MeventCell {
  struct s_Mevent* first;
  struct s_MeventCell* next;
} MeventCell;
typedef struct s_Queue_of_Mevent {
  struct s_MeventCell* head;
  struct s_MeventCell* tail;
} Queue_of_Mevent;

typedef struct s_PartCell {
  struct s_Part* first;
  struct s_PartCell* next;
} PartCell;

typedef struct s_WireCell {
  struct s_Wire* first;
  struct s_WireCell* next;
} WireCell;


typedef struct s_Queue_of_Part {
  struct s_PartCell* head;
  struct s_PartCell* tail;
} Queue_of_Part;

typedef struct s_List_of_Part {
  struct s_PartCell* head;
} List_of_Part;

typedef struct s_List_of_Wire {
  struct s_WireCell* head;
} List_of_Wire;


#include "eh.h"

typedef Part Sender;
typedef Port SenderPort;
typedef Part Receiver;
typedef Port ReceiverPort;
typedef Part Container;
typedef Part Leaf;

typedef struct s_Connector {
  Dir direction;
  Sender* sender;
  SenderPort source_port;
  Receiver* receiver;
  ReceiverPort receiver_port;
} Connector;
typedef Connector Wire;
typedef Wire Wire_Proto;

typedef struct s_List_of_Connector {
  Connector* car;
  struct s_List_of_Connector* cdr;
} List_of_Connector;

#include "preamble.h"
#include "gensym.h"
#include "connector.h"
#include "template.h"
#include "container.h"
#include "leaf.h"
#include "jit.h"
#include "stock.h"
#include "start.h"


// typedef Fhandler ???;
// typedef Finject ???;
