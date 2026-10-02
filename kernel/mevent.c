/* typedef struct s_Datum { */
/*   Payload* v; */
/*   struct s_Datum* (*clone)(Eh*); */
/*   void (*reclaim)(Eh*); */
/*   void* other; */
/* } Datum; */

//////////////////

/// preamble
typedef struct s_Eh *Eh;
typedef struct s_Payload Payload;
typedef struct s_Datum Datum;
typedef Datum* (*Fclone)(Eh);
typedef void (*Freclaim)(Eh);
/// end preamble

typedef struct s_Datum {
  Payload* v;
  Fclone clone;
  Freclaim reclaim;
  void* other;
} Datum;

int main (int argc, char** argv) {
}
