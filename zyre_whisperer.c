#include <zyre.h>

// this program starts a zyre node and whispers
// messages to a node with a specific UUID
int main(int argc, char *argv[])
{

    char * peer_uuid = "no";

    // create a new node
    zyre_t *node = zyre_new("whisperer");
    if (!node)
    {
        return 1;                 //  Could not create new node
    }

    // this sends an ENTER message
    zyre_start(node);
    // wait for a while
    zclock_sleep(250);
    // print UUID of node
    printf("UUID: %s\n", zyre_uuid(node));

    // Recieve the ENTER message for another node to get the uuid to whisper to
    while (peer_uuid == "no")
    { 
	zmsg_t *msg = zyre_recv(node); // recieve message,
	char *command = zmsg_popstr(msg); // parse first string of message (command)
	if(streq(command, "ENTER")) // looking for an ENTER
	{
	    printf("Someone Joined!\n");
	    peer_uuid = zmsg_popstr(msg); // get second string of message (peer uuid)
	    char *name = zmsg_popstr(msg); // get thirst string of messabe (peer name)
	    printf("They are: %s AKA %s\n",peer_uuid,name);
	}
	else // not an ENTER message (realistically this is dead code because the first message this client will ever see is somone entering the network)
	{
	    printf("Still alone...\n");
	}
	zstr_free(&command);
    }

    for (int i = 0; i < 5; i++)
    {
        // this sends a WHISPER message
        zyre_whispers(node, peer_uuid, "hello %d", i);
        zclock_sleep(1000);
    }
    // this sends an EXIT message
    zyre_stop(node);
    // wait for node to stop
    zclock_sleep(100);
    zyre_destroy(&node);
    return 0;
}
