# Philosophers

## Concurrent programming

### Concurrent programming allows programs to execute different tasks simultaneously. For example, a program must not wait the result of a specific task to launch another one. A way to implement concurrent programming is thanks to threads. A thread is a sequence of instructions inside a process. Each thread has his own context but they share the same virtual memory address space. Unlike processes, communication between threads is made easier.

## POSIX

### POSIX is a multithreading mechanism with a library in order to create and manage threads in our code. Here is the functions allowed in the project.

## Allowed functions :
* int pthread_/create(pthread_/t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);
### create a new execution thread
* int pthread_join(pthread_t thread, void **retval);
### get back the result of a execution thread in the main thread
* int pthread_detach(pthread_t thread);
### detach an execution thread from the main thread to allow him to get his ressources at the end of his execution

## Shared memory and critical section
### The greater strenght of threads is the capability of easily sharing memory between them but if multiple threads access the same memory address at the same time, data races appear and it could cause output problems. We could called the critical section the instructions section that cannot be execute simultaneously by two threads.

## Coordination between threads
### With the principe of mutual exclusion, threads have three different states from the operating system : running, blocked and ready. But how the OS can chose which thread can launch itself? We called it the scheduling politic and it depend of blocking system call of the thread, OS directly or regular timer (usleep) of the OS. To coordinate ressources access between threads we use mutexes. A mutex has two states : unlock or lock. Here is the functions allowed in the project.

## Allowed functions :
* int pthread_mutex_init(pthread_mutex_t *mutex,
                        const pthread_mutexattr_t *mutexattr);
### initialize a mutex
* int pthread_/mutex_/lock(pthread_/mutex_/t *mutex));
### lock a ressource
* int pthread_mutex_unlock(pthread_mutex_t *mutex);
### unlock a ressource
* int pthread_/mutex_/destroy(pthread_/mutex_/t *mutex);
### free the ressources associate to a mutex that is not more needed

## Deadlocks
### Program blocking because of mutexes. When two threads require two identical ressources, they first begin to lock a different ressource and then they both want to access the ressource that is lock by the other. Several solutions exists, one is to define an order that threads must comply when they want to get access of a ressource. For example, they first begin to lock the same ressource.

## Logic to resolve the Philosophers problem
