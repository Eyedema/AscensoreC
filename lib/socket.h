#define DEFAULT_PROTOCOL 0 

typedef struct {
    int fd;
    struct sockaddr* sockAddrPtr;
    int len;
} Socket;

Socket creaSocketServer(char name[]){
    Socket s;
    
    int serverLen;

    struct sockaddr_un serverUNIXAddress; /* Server address */
    struct sockaddr_un clientUNIXAddress; /* Client address */
    struct sockaddr* serverSockAddrPtr; /* Ptr to server address */

    /* Ignore death-of-child signals to prevent zombies */
    signal(SIGCHLD, SIG_IGN);

    serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
    serverLen = sizeof (serverUNIXAddress);

    s.sockAddrPtr = (struct sockaddr*) &clientUNIXAddress;
    s.len = sizeof (clientUNIXAddress);

    /* Create a UNIX socket, bidirectional, default protocol */
    s.fd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);

    serverUNIXAddress.sun_family = AF_UNIX; /* Set domain type */

    strcpy(serverUNIXAddress.sun_path, name); /* Set name */

    unlink(name); /* Remove file if it already exists */

    bind(s.fd, serverSockAddrPtr, serverLen); /* Create file */

    listen(s.fd, 10); /* Maximum pending connection length */
    
    return s;
}

int creaSocketClient(char name[]){
    Socket s;
    int result;

    struct sockaddr_un serverUNIXAddress;
    s.sockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
    s.len = sizeof (serverUNIXAddress);
    s.fd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    serverUNIXAddress.sun_family = AF_UNIX;
    strcpy(serverUNIXAddress.sun_path, name);
    
    do {
        result = connect(s.fd, s.sockAddrPtr, s.len);
    } while (result == -1);
    
    return s.fd;
}
