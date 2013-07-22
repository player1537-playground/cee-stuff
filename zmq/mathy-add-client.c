#include "zhelpers.h"

int main (void)
{
  printf ("Connecting to math server…\n");
  void *context = zmq_ctx_new ();
  void *requester = zmq_socket (context, ZMQ_REQ);
  zmq_connect (requester, "tcp://localhost:5555");
  srand(time(NULL));

  int request_nbr;
  for (request_nbr = 0; request_nbr < 1000; request_nbr++) {
    char buffer [10];
    int a = randof(30);
    int b = randof(30);
    printf ("Sending (%d, %d)…\n", a, b);
    char to_send[100];
    sprintf(to_send, "%d %d", a, b);
    s_send(requester, to_send);
    char* ret = s_recv(requester);
    int sum = atoi(ret);
    printf("%d + %d = %d\n", a, b, sum);
    free(ret);
  }
  zmq_close (requester);
  zmq_ctx_destroy (context);
  return 0;
}
