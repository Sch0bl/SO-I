#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define FILE_DATA "server.data"

/*
 * Para probar, usar netcat. Ej:
 *
 *      $ nc localhost 4040 
 *      NUEVO
 *      0
 *      NUEVO
 *      1
 *      CHAU
 */

void quit(char *s)
{
	perror(s);
	abort();
}

int read_data(char* file){ //función para registro de pedidos
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();

	int request_n;

	int fd;
	if ((fd = open(file, O_RDONLY)) < 0)
		quit("fd read_lock");


	fcntl(fd, F_GETLK, &lock);	
	while (lock.l_type != F_UNLCK) {
		sleep(1);
		fcntl(fd, F_GETLK, &lock);	
	}

	read(fd, &request_n, sizeof(request_n));

	lock.l_type = F_UNLCK;

	if (fcntl(fd, F_SETLK, &lock) < 0)
		quit("Failed to unlock");

	close(fd);

	return request_n;
}

void write_data(char* file, int* request_n){  //función para registro de pedidos
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 4;
	lock.l_pid = getpid();

	int fd;

	if ((fd = open(file, O_RDWR | O_CREAT, 0666)) < 0)
		quit("fd write_lock");

	if (fcntl(fd, F_SETLK, &lock) < 0)
		quit("Failed to lock");

	write(fd, request_n, sizeof(*request_n));

	lock.l_type = F_UNLCK;

	if (fcntl(fd, F_SETLK, &lock) < 0)
		quit("Failed to unlock");

	close(fd);
}

int U = 0;

int fd_readline(int fd, char *buf)
{
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0) //Error
		return rc;

	buf[i] = 0; //NULL
	return i;
}

void handle_conn(int csock)
{
	char buf[200];
	int rc;
	int request_n = read_data(FILE_DATA);
	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			write_data(FILE_DATA, &request_n);
			close(csock);
			return;
		}

		if (!strcmp(buf, "NUEVO")) {
			char reply[20];
			sprintf(reply, "%d\n", request_n);
			request_n++;
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			write_data(FILE_DATA, &request_n);
			close(csock);
			return;
		}
	}
}

void wait_for_clients(int lsock)
{
	int csock;
	/* Esperamos una conexión, no nos interesa de donde viene */
	csock = accept(lsock, NULL, NULL);
	if (csock < 0)
		quit("accept");
	pid_t pid = fork();

	if (pid == 0){
        /* Atendemos al cliente */
	    handle_conn(csock);
      exit(EXIT_SUCCESS);
  }
	wait_for_clients(lsock);
}

/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc; //Para detectar errores
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET; //Dominio
	sa.sin_port = htons(4040); //Puerto
	sa.sin_addr.s_addr = htonl(INADDR_ANY); //Direccion

	/* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa); 
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10); //Aca se bloquea para escucha clientes
    //Maxima cantidad de clientes que pueden esperar: 10
	if (rc < 0)
		quit("listen");

	return lsock;
}

int main()
{
	int lsock;
	int init_req = 0;
	write_data(FILE_DATA, &init_req);

	lsock = mk_lsock();
	
  wait_for_clients(lsock);
}
