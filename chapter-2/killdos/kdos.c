// kdos.c
// https://www.youtube.com/watch?v=8VRwTVdT9s8
// typed out by iamismael

#include <stdio.h>

#define WORD unsigned short

#define IDT_001_ADDR 0 // start address of the first IVT vector
#define IDT_255_ADDR 1020 // start address of the last IVT vector
#define IDT_VECTOR_SZ 4 // size of each interrupt vector

#define BP __asm{ int 0x3 } // break point

void main()
{
    WORD csAddr;  // code segment of given interrupt
    WORD ipAddr;  // starting IP for fiven interrupt
    short address; // address in memory (0 - 1020)
    WORD vector;
    char dummy; // strictly to help pause program execution

    vector = 0x0;
    
    printf("\n---Dumping IVT from bottom up---\n");
    printf("Vector\tAddress\t\n");

    for
    (
      address=IDT_001_ADDR; 
      address<=IDT_255_ADDR; 
      address=address+IDT_VECTOR_SZ,vector++
    ) 
    {
        printf("%03d\t%08p\t", vector, address);

        // IVT starts at bottom of memory, so CS is always 0x0
        __asm {
            PUSH ES 
            MOV AX, 0 
            MOV ES, AX 
            MOV BX, address 
            MOV AX, ES:[BX] 
            MOV ipAddr, AX 
            INC BX
            INC BX
            MOV AX, ES:[BX] 
            mov csAddr, AX 
            POP ES 
        };
        printf("[CS:IP]=[%04X,%04X]\n", csAddr,ipAddr);
    }

    printf("Press [ENTER] key to continue...");
    scanf("%c", &dummy);

    printf("\n---Overwriting IVT from top down---\n");
    
    /*
        program will die somewhere around 0x4*
        Note: can get same results via DOS debug.exe -e command
    */
    
    for
    (
      address=IDT_255_ADDR; 
      address>=IDT_001_ADDR; 
      address=address-IDT_VECTOR_SZ,vector--
    ) 
    {
        printf("Nulling %03d\t%08p\n", vector, address);
        __asm {
            PUSH ES 
            MOV AX, 0 
            MOV ES, AX
            MOV BX, address 
            MOV ES:[BX], AX 
            INC BX
            INC BX
            MOV ES:[BX], AX 
            POP ES 
        };
    }
    return;
} /* end main() ---------------------------------------------------*/
