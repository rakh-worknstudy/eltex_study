#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//	========================================================
//	                     SHOPS DEFINES
//	========================================================
#define SHOPS_COUNT		3					//	Число магазинов
#define SHOPS_VALUE		1000				//	Вместимость магазинов
unsigned shops[SHOPS_COUNT];				//	Массив магазинов
unsigned shops_busy;
pthread_mutex_t shops_mutex[SHOPS_COUNT];	//	Мютексы магазинов

void shops_refill(void);
void shops_print(void);

//	========================================================
//	                    CLIENTS DEFINES
//	========================================================
#define CLIENTS_COUNT	5			//	Число покупателей
#define CLIENTS_NEEDS	10000		//	Нужды покупателей
#define CLIENTS_ONCE	250			//	Сколько может "унести" за раз
unsigned clients[CLIENTS_COUNT];	//	Массив клиентов
pthread_t clients_threads[CLIENTS_COUNT];

/* Инициализация массива покупателей */
void clients_init(void);
/* Ходим по магазинам (не ждем) */
void *client_shopping(void *args);
void clients_print(void);

//	========================================================
//	                         MAIN
//	========================================================
void mutex_init(void);
void mutex_destroy(void);
void main_loop(void);
int main(void) {
	clients_init();
	main_loop();
	return 0;
}
void main_loop(void) {
	unsigned day = 0;
	unsigned have_needs = 1;
	while (have_needs) {
		++day;
		have_needs = 0;
		shops_refill();
		printf("=====================================\n");
		printf("DAY %d\n", day);
		clients_print();
		printf("\n");
		for (unsigned client = 0; client < CLIENTS_COUNT; ++client) {
			if (clients[client]) {
				pthread_create(&clients_threads[have_needs], NULL, client_shopping, &clients[client]);
				++have_needs;
			}
		}
		for (unsigned client = 0; client < have_needs; ++client)
			pthread_join(clients_threads[client], NULL);
	}
	printf("=====================================\n");
}
void mutex_init(void) {
	for (unsigned iter = 0; iter < SHOPS_COUNT; ++iter)
		pthread_mutex_init(&shops_mutex[iter], NULL);
}
void mutex_destroy(void) {
	for (unsigned iter = 0; iter < SHOPS_COUNT; ++iter)
		pthread_mutex_destroy(&shops_mutex[iter]);
}
//	========================================================
//	                     SHOPS FUNCTIONS
//	========================================================
/* Ежедневное поступление "товаров" */
void shops_refill(void) {
	for (unsigned iter = 0; iter < SHOPS_COUNT; ++iter)
			shops[iter] = SHOPS_VALUE;
	shops_busy = SHOPS_COUNT;
}
void shops_print(void) {
	for (unsigned iter = 0; iter < SHOPS_COUNT; ++iter)
		printf("%u\t", shops[iter]);
}
unsigned shop_buy(unsigned shop, unsigned needs) {
	/* CAN TAKE AT ONCE */
	unsigned needs_once;
	if (needs > CLIENTS_ONCE)
		needs_once = CLIENTS_ONCE;
	else
		needs_once = needs;
	/* CAN GIVE AT ONCE */
	if (shops[shop] > needs_once) {
		shops[shop] -= needs_once;
		needs -= needs_once;
	} else {
		needs -= shops[shop];
		shops[shop] = 0;
		--shops_busy;
	}
	return needs;
}

//	========================================================
//	                    CLIENTS FUNCTIONS
//	========================================================
/* Инициализация массива покупателей */
void clients_init(void) {
	for (unsigned iter = 0; iter < CLIENTS_COUNT; ++iter)
		clients[iter] = CLIENTS_NEEDS;
}
/* Ходим по магазинам (не ждем) */
void *client_shopping(void *args) {
	unsigned needs = *(unsigned *)args;
	unsigned shop = 0;
	while (needs && shops_busy) {
		/* TRY LOCK */
		if (!pthread_mutex_trylock(&shops_mutex[shop])) {
			if (shops[shop]) {
				needs = shop_buy(shop, needs);
				pthread_mutex_unlock(&shops_mutex[shop]);
				sleep(1);
			} else {
				pthread_mutex_unlock(&shops_mutex[shop]);
			}			
		}
		shop = (shop + 1) % SHOPS_COUNT;
	}
	*(unsigned *)args = needs;
	return NULL;
}

void clients_print(void) {
	for (unsigned iter = 0; iter < CLIENTS_COUNT; ++iter)
		printf("%u\t", clients[iter]);
}