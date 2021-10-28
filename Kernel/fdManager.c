#include <fdManager.h>

typedef struct fdCDT
{
    int number;
    int pipeId;
} fdCDT;

static fdADT fdList[MAX_FD] = {NULL};
static memoryManagerADT memoryManager;
static schedulerADT scheduler;

int initFdManager(memoryManagerADT mm, schedulerADT sch)
{
    memoryManager = mm;
    scheduler = sch;
    newFd(-1); // STDOUT
    newFd(-1); // STDIN
}

int newFd(int pipeId)
{
    int i;
    while (i < MAX_FD && fdList[i] != NULL)
        i++;
    if (i > MAX_FD)
        return -1;

    fdADT aux = allocMem(memoryManager, sizeof(fdCDT));
    aux->number = i;
    aux->pipeId = pipeId;

    fdList[i] = aux;

    return i;
}

int fdRead(int fd, char *dest, int count)
{
    if (fd > MAX_FD || fdList[fd] == NULL || fd == STDOUT)
        return -1;

    fdADT fdAux = fdList[fd];
    if (fdAux->number == STDIN)
    {
        unsigned char *KeyBuffer = getBuffer();
        unsigned int i;
        if (KeyBuffer[0] == '\0')
        {
            blockProcess(scheduler, 1);
        }
        dest[0] = 0;
        for (i = 0; KeyBuffer[i] != 0 && i < count; i++)
        {
            dest[i] = KeyBuffer[i];
        }
        removeBuffer();
        return i;
    }
    else
    {
        return pipeRead(fdAux->pipeId, dest, count);
    }
}

int fdWrite(int fd, char *src, int color)
{
    if (fd > MAX_FD || fdList[fd] == NULL || fd == STDIN)
        return -1;
    fdADT fdAux = fdList[fd];
    if (fd == STDOUT)
    {
        ncPrint(src, color);
        return 0;
    }
    else
    {
        return pipeWrite(fdAux->pipeId, src);
    }
}

int freeFd(int fd)
{
    if (fd > MAX_FD || fd < 0)
        return -1;

    fdADT fdAux = fdList[fd];
    if (fdAux == NULL)
        return -1;

    if (fd > 1)
    {
        return freePipe(fdAux->pipeId);
    }
    else
    {
        fdList[fd] = NULL;
        return freeMem(memoryManager, fdAux);
    }
}

void closeFd(int fd)
{
    if (fd <= STDOUT || fd > MAX_FD)
        return;
    fdADT aux = fdList[fd];
    closePipe(aux->pipeId);
}