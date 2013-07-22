#include "zhelpers.h"

int main (void)
{
  // Socket to talk to clients
  void *context = zmq_ctx_new ();
  void *responder = zmq_socket (context, ZMQ_REP);
  int rc = zmq_bind (responder, "tcp://*:5555");
  assert (rc == 0);

  while (1) {
    char* buffer = s_recv(responder);
    printf ("Received %s\n", buffer);
    int in1, in2;
    sscanf(buffer, "%d %d", &in1, &in2);
    printf("Received (%d, %d)\n", in1, in2);
    char ret[100];
    sprintf(ret, "%d", in1 + in2);
    s_send(responder, ret);
    free(buffer);
  }
  return 0;
}
