// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <exceptions.h>
#include <sysHandler.h>
#include <naiveConsole.h>
#include <memoryManager.h>
#include <taskManager.h>
#include <semManager.h>
#include <keyboard.h>
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
typedef int (*EntryPoint)();
extern uint64_t *_getRSP();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 //The size of the stack itself, 32KiB
					- sizeof(uint64_t)					  //Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{

	void *moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{
	memoryManagerADT mm = newMemoryManager((void * )0x800000, 1024*1024*64);
	schedulerADT scheduler = newScheduler(mm);
	initTaskManager(scheduler);
	initSysHandler(mm,scheduler);
	initSemManager(mm, scheduler);
	initKeyboard(mm, scheduler);
	initFdManager(mm,scheduler);
	initPipeManager(mm,scheduler);
	/* First process */
	char *argv[] = {"Shell"};
	int fd[2];
	fd[0] = 0;
	fd[1] = 1;
	newProcess(scheduler,50,sampleCodeModuleAddress,argv,1,1, fd);
	backAddresses((uint64_t *)sampleCodeModuleAddress, _getRSP()); // exceptions
	
	load_idt();	
	ncClear();


	

	return 0;
	
}