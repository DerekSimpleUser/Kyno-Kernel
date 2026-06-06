/* =======================================================================
   KYNO KERNEL - Versión Blindada (Todo en un bloque secuencial)
   ======================================================================= */

// Forzamos un bloque único donde el arranque y el flujo principal están pegados
__asm__(
    ".section .text\n\t"
    ".global _start\n\t"
    "_start:\n\t"
    "    jmp multiboot_entry\n\t"
    
    ".align 4\n\t"
    "multiboot_header:\n\t"
    "    .long 0x1BADB002\n\t"            // Número mágico Multiboot
    "    .long 0x00\n\t"                  // Flags
    "    .long -(0x1BADB002 + 0x00)\n\t"  // Checksum

    "multiboot_entry:\n\t"
    "    cli\n\t"                         // Desactivar interrupciones
    "    mov $stack_space, %esp\n\t"      // Configurar la pila
    "    call kernel_main_c\n\t"          // Saltar directo a nuestra lógica
    
    "halt_loop:\n\t"
    "    hlt\n\t"                         // Detener la CPU si algo regresa
    "    jmp halt_loop\n\t"
    
    ".section .bss\n\t"
    ".comm stack_space, 16384, 32\n\t"
);

// Puntero volátil para que GCC no intente optimizar la escritura en memoria
volatile char *video_memory = (volatile char *) 0xB8000;

void kernel_main_c(void) {
    // 1. Limpiar pantalla de forma ultra-directa
    int i;
    for (i = 0; i < 4000; i += 2) {
        video_memory[i] = ' ';     
        video_memory[i+1] = 0x07;   
    }

    // 2. Pintar mensaje "KYNO KERNEL" en la fila 1, columna 1 (Amarillo 0x0E)
    const char *msg1 = "KYNO KERNEL ACTIVO - SUCESO";
    int idx1 = 162;
    int j = 0;
    while (msg1[j] != '\0') {
        video_memory[idx1] = msg1[j];
        video_memory[idx1+1] = 0x0E;
        idx1 += 2;
        j++;
    }

    // 3. Pintar mensaje de éxito en la fila 3, columna 1 (Verde 0x0A)
    const char *msg2 = "[OK] Modo C nativo corriendo en Linux puro!";
    int idx2 = 482;
    j = 0;
    while (msg2[j] != '\0') {
        video_memory[idx2] = msg2[j];
        video_memory[idx2+1] = 0x0A;
        idx2 += 2;
        j++;
    }

    // Bucle de espera seguro para congelar la ejecución manteniendo la pantalla
    while(1) {
        __asm__ __volatile__("hlt");
    }
}
