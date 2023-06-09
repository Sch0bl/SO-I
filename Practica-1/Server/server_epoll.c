#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#define MAX_EVENTS 10



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

void handle_conn(int csock, int epoll_fd, struct epoll_event *ev)
{
	char buf[200];
	int rc;

		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, csock, ev);
			close(csock);
			return;
		}

		if (!strcmp(buf, "NUEVO")) {
			char reply[20];
			sprintf(reply, "%d\n", U);
			U++;
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			epoll_ctl(epoll_fd, EPOLL_CTL_DEL, csock, ev);
			close(csock);
			return;
	}
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

	struct epoll_event ev, events[MAX_EVENTS];
	lsock = mk_lsock();
	int epoll_fd = epoll_create1(0);
	ev.data.fd = lsock;
	ev.events = EPOLLIN;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, lsock, &ev);
	while(1){
		int cant_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		for(int i = 0; i < cant_events; i++){
			if(events[i].data.fd == lsock){
				int csock = accept(lsock, NULL, NULL);
				printf("conexion exitosa\n");
				ev.data.fd = csock;
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, csock, &ev);
			}
			else{
				printf("manejando evento\n");
				handle_conn(events[i].data.fd,epoll_fd, &ev);
			}
		}
	}
}
