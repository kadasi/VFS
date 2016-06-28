//#include "sys/select.h"
#include "unistd.h"
#include <assert.h>
//#include <string.h>
#include "fcntl.h"


int main() {
    int fd = open( "poll.h", O_RDONLY | O_CREAT, 0400 );
    assert( fd >= 0 );

    char ch[8];
    read(fd, ch, 8);

    int can;
    can = canRead(fd);
    can = canWrite(fd);

//    fd_set rfds;
//    FD_ZERO(&rfds);
//    FD_SET(fd, &rfds);

//    assert(myselect(1, &rfds, NULL, NULL, NULL) == 1);
//    assert(FD_ISSET(fd, &rfds));
//    FD_ZERO(&rfds);
//    FD_SET(fd, &rfds);

//    assert(myselect(1, NULL, &rfds, NULL, NULL) == 0);
//    assert(!FD_ISSET(fd, &rfds));
//    FD_ZERO(&rfds);
//    FD_SET(fd, &rfds);

//    assert(myselect(1, NULL, NULL, &rfds, NULL) == 1);
//    assert(!FD_ISSET(fd, &rfds));
//    FD_ZERO(&rfds);
//    FD_SET(fd, &rfds);

    assert( close( fd ) == 0 );
    return 0;
}
