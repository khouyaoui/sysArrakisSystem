#include <sys/signalfd.h>
#include<stdlib.h>
#include <semaphore.h>


int 
main () {

    //semf
    sem_t semaforo;
    
    sem_init(&semaforo,1,1);


}
