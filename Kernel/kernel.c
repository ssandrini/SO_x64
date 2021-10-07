#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <exceptions.h>
#include <sysHandler.h>
#include <naiveConsole.h>
#include <memoryManager.h>
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

	memoryManagerADT mm = newMemoryManager((void*) 0x0000000000700000 , 8);

	int i = 0;
	char * p = (char *) 0x0000000000700000;
	char * asd = (char * ) allocMem(mm, 2);
	for(i = 0; i < 2; i++) {
		asd[i] = i;
	}
	char * asd2 = (char * ) allocMem(mm, 2);
	for(i = 0; i < 2; i++) {
		asd[i] = i+2;
	}
	char * asd3 = (char * ) allocMem(mm, 2);
	for(i = 0; i < 2; i++) {
		asd[i] = i+4;
	}

	load_idt();
	backAddresses((uint64_t *)sampleCodeModuleAddress, _getRSP()); // exceptions
	ncClear();

	((EntryPoint)sampleCodeModuleAddress)();
	
	return 0;
}
