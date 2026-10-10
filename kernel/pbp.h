// hardware level types
deftype char* Str;
deftype Byte unsigned char;
deftype int Index;

// DI level types
deftype Str Port;
deftype Str Payload;

deftype BOOL Byte;
#define FALSE 0
#define TRUE 1

deftype Dir Byte;
#define Down 0
#define Across 1
#define Up 2
#define Through 3


// Templates (T == Template)

deftype Index SenderTIndex;
deftype Port SenderPort;
deftype Index ReceiverTIndex;
deftype Port ReceiverPort;

deftype s_SenderT {
  Index part;
  Port port;
} SenderT;
deftype s_ReceiverT {
  Index part;
  Port port;
} ReceiverT;

deftype s_WireT {
  Dir dir;
  SenderT sender;
  ReceiverT receiver;
} WireT;

deftype s_List_of_WireT {
} List_of_WireT;

deftype s_PartT {
} PartT;

deftype struct s_ContainerT {
  List_of_PartT children;
  List_of_WireT wires;
} ContainerT;

deftype s_Table_by_ID_of_PartT {
} Table_by_ID_of_PartT;



// Runtime (I == Instance)

deftype s_Eh {
} Eh;

deftype s_PartI {
} PartI;

deftype s_LeafI {
} LeafI;

deftype s_ContainerI {
} ContainerI;

deftype s_WireI {
} WireI;

deftype s_List_of_WireI {
} List_of_WireI;

deftype s_Mevent {
} Mevent;

