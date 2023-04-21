#include "clients.h"
#include <malloc.h>
#include "shops.h"

/* Инициализация массива клиентов */
unsigned *clients_init(const unsigned count, const unsigned needs) {
	unsigned *clients = NULL;
	if (count) {
		clients = (unsigned *)malloc(count * sizeof(unsigned));
		for (unsigned iter = 0; iter < count; ++iter)
			clients[iter] = needs;
	}
	return clients;
}