#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "fcntl.h"
#include "unistd.h"

TEST_CASE("Open file which does not exist") {
    int fd = open("somefile", O_RDONLY, 0400);
    REQUIRE(fd == -1);
}
TEST_CASE("Open file and read from it") {
    int fd = open("poll.h", O_RDONLY, 0400);
    REQUIRE(fd >= 0);

    char buf[6];
    REQUIRE(read(fd, buf, 5) == 5);
    buf[5] = '\0';
    REQUIRE(strcmp(buf, "/* Co") == 0);
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Writing to readonly(created) file") {
    int fd = open("new", O_RDONLY | O_CREAT, 0400);
    REQUIRE(fd >= 0);

    REQUIRE(write(fd, "lalala", 6) == -1);
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Reading from writeonly(created) file") {
    int fd = open("newfile", O_WRONLY | O_CREAT, 0600);
    REQUIRE(fd >= 0);

    char *out;
    REQUIRE(read(fd, out, 6) == -1);
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Writing to readonly file") {
    int fd = open("poll.h", O_RDONLY | O_CREAT, 0400);
    REQUIRE(fd >= 0);

    REQUIRE(write(fd, "lalala", 6) == -1);
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Reading from writeonly file") {
    int fd = open("poll.h", O_WRONLY | O_CREAT, 0600);
    REQUIRE(fd >= 0);

    char *out;
    REQUIRE(read(fd, out, 6) == -1);
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Create, write and read written") {
    int fd = open("new.txt", O_WRONLY | O_CREAT, 0600);
    REQUIRE(fd >= 0);

    REQUIRE(write(fd, "lalala", 6) == 6);
    REQUIRE(close(fd) == 0);

    fd = open("new.txt", O_RDONLY | O_CREAT, 0400);
    REQUIRE(fd >= 0);

    char out[6];
    REQUIRE(read(fd, out, 6) == 6);
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Canwrite in readonly file") {
    int fd = open("poll.h", O_RDONLY | O_CREAT, 0400);
    REQUIRE(fd >= 0);

    REQUIRE(canRead(fd));
    REQUIRE(!canWrite(fd));
    REQUIRE(close(fd) == 0);
}
TEST_CASE("Canwrite, canread") {
    int fd = open("poll.h", O_RDWR);
    REQUIRE(fd >= 0);

    REQUIRE(canWrite(fd));
    REQUIRE(canRead(fd));
    REQUIRE(close(fd) == 0);
}
