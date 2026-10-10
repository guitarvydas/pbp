#ifndef PBP_H
#define PBP_H

// hardware level types
typedef struct s_Str {
  int count;
  char* s;
} Str;

typedef unsigned char Byte;
typedef unsigned char Bool;
typedef int Index;



// DI level types
typedef void TBD; // for things I haven't determined yet

typedef Str Port;
typedef Str Payload;

typedef Bool Byte;
#define FALSE 0
#define TRUE 1

typedef Str Dir;

typedef struct s_Eh Eh;
typedef struct s_Eh Part;
typedef struct s_Eh Leaf;
typedef struct s_Eh Container;

typedef struct s_Dict Dict;
typedef Dict Dict_of_Template;

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

typedef struct s_Sender Sender;
typedef struct s_Receiver Receiver;
typedef struct s_Connector Connector;
typedef Connector Wire;
typedef Wire Wire_Proto;

typedef struct s_List_of_Connector {
  Connector* car;
  struct s_List_of_Connector* cdr;
} List_of_Connector;

typedef struct s_Component_Registry Component_Registry;
typedef struct s_Template Template;
typedef Eh* (*Finstantiator) (Component_Registry*, Eh*, Str, Template*);

#include "preamble.h"
#include "gensym.h"
#include "connector.h"
#include "templates.h"
#include "container.h"
#include "leaf.h"
#include "jit.h"
#include "stock.h"
#include "start.h"


Connector* fresh_Connector (void);

Sender* mkSender (Str, Eh*, Connector*);

typedef TBD Table_by_ID_of_Part;

Eh* lookupstring (Connector*, Str);

#endif
