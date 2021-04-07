#include <zyre.h>

// this program starts a zyre node, joins a group and
// shouts messages to all nodes in the group
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./zyre_shouter <group name>\n");
        return 1;
    }

    char * group_name = argv[1];
    char * node_name = "Shouter";

    // create a new node
    zyre_t *node = zyre_new(node_name);
    if (!node)
    {
        return 1;                 //  Could not create new node
    }

    // this sends an ENTER message
    zyre_start(node);
    // this sends a JOIN message
    zyre_join(node, group_name);
    // wait for a while
    zclock_sleep(250);
    // print UUID of node
    printf("UUID: %s\n", zyre_uuid(node));
    for (int i = 0; i < 5; i++)
    {
        // this sends a SHOUT message
        zyre_shouts(node, group_name,
			"This is a long message to test what a long message looks like on the input interface %d", i);
        zclock_sleep(1000);
    }
    zyre_shouts(node, group_name,
		    "The next two messages will include blank and then null messages");
    zclock_sleep(1000);
    zyre_shouts(node, group_name, "");
    zclock_sleep(1000);
    zyre_shouts(node, group_name, NULL);
    zclock_sleep(1000);
    // this sends a LEAVE message
    zyre_leave(node, group_name);
    // this sends an EXIT message
    zyre_stop(node);
    // wait for node to stop
    zclock_sleep(100);
    zyre_destroy(&node);
    return 0;
}
