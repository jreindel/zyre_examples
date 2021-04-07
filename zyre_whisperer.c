#include <zyre.h>

// this program starts a zyre node and whispers
// messages to a node with a specific UUID
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./zyre_whisperer <UUID>\n");
        return 1;
    }

//    char * peer_uuid = argv[1];
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

    // Recieve the ENTER message for another node
    while (peer_uuid == "no")
    { 
	zmsg_t *msg = zyre_recv(node);
	char *command = zmsg_popstr(msg);
	if(streq(command, "ENTER"))
	{
	    printf("Someone Joined!\n");
	    peer_uuid = zmsg_popstr(msg);
	    char *name = zmsg_popstr(msg);
	    printf("They are: %s AKA %s\n",peer_uuid,name);
	}
	else
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
