#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int readcount = getreadcount();
    printf(1, "Number of read() calls: %d\n", readcount);
    exit();
}
