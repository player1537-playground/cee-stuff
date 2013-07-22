// Weather update server
// Binds PUB socket to tcp://*:5556
// Publishes random weather updates

#include "zhelpers.h"

int main (void)
{
  // Prepare our context and publisher
  void *context = zmq_ctx_new ();
  void *publisher = zmq_socket (context, ZMQ_PUB);
  void *receiver = zmq_socket (context, ZMQ_REP);
  int rc = zmq_bind (publisher, "tcp://*:5556");
  assert (rc == 0);
  rc = zmq_bind (publisher, "ipc://chat.ipc");
  assert (rc == 0);
  rc = zmq_bind (receiver, "tcp://*:5558");
  assert (rc == 0);

  // Initialize random number generator
  while (1) {
    char* message = s_recv (receiver);
    s_send (publisher, message);
    free (message);
  }
  zmq_close (publisher);
  zmq_ctx_destroy (context);
  return 0;
}
