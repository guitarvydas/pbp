// hardware level types
typedef char* Str;
typedef unsigned char Byte;
typedef unsigned char BOOL;
typedef int Index;

// DI level types
typedef Str Port;
typedef Str Payload;

typedef BOOL Byte;
#define FALSE 0
#define TRUE 1

typedef Byte Dir;
#define Down 0
#define Across 1
#define Up 2
#define Through 3



typedef Eh Part;
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

typedef struct s_List_of_Connector {
  Connector* car;
  struct s_List_of_Connector* cdr;
} List_of_Connector;

