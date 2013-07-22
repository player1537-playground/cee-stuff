// Weather update client
// Connects SUB socket to tcp://localhost:5556
// Collects weather updates and finds avg temp in zipcode

#include "zhelpers.h"

int main (int argc, char *argv [])
{
  // Socket to talk to server
  printf ("Collecting updates from chat server…\n");
  void *context = zmq_ctx_new ();
  void *subscriber = zmq_socket (context, ZMQ_SUB);
  int rc = zmq_connect (subscriber, "tcp://localhost:5556");
  assert (rc == 0);

  char *filter = (argc > 1)? argv [1]: ">";
  rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
		       filter, strlen (filter));
  assert (rc == 0);

  for (;;) {
    char *string = s_recv (subscriber);
    printf("Received: %s\n", string);
    free(string);
  }
  zmq_close (subscriber);
  zmq_ctx_destroy (context);
  return 0;
}
