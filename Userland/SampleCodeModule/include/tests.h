#ifndef __TESTS__
#define __TESTS__

#define TEST_MEMORY "testmemory"
#define TEST_PROCESS_A "a"
#define TEST_PROCESS_B "b"
#define TEST_MUTEX_C "c"
#define TEST_MUTEX_D "d"
#define TEST_MUTEX_Z "z"
#define TEST_MUTEX_E "e"
#define TEST_SHARED_MEMORY "testshm"
#define TEST_BLOCK "block"
#define TEST_UNBLOCK "unblock"
#define TEST_PIPE_F "pipef"
#define TEST_PIPE_G "pipeg"
#define PHI_TEST "testphi"
#define PHI "phi"

void testMemory();
void testProcessA();
void testProcessB();
void testMutexC();
void testMutexD();
void testMutexZ();
void testMutexE();
void testSharedMemory();
void testBlock();
void testUnblock();
void testPipeF();
void testPipeG();
void testPhi();
void phi();

void putFork(uint8_t pid, uint8_t* state);
void takeFork(uint8_t pid, uint8_t* state);

#endif // __TESTS__
