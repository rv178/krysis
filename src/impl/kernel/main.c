#include "print.h"
#include "port.h"
#include "idt.h"
#include "isr.h"
#include "util.h"

void kernel_main()
{
    print_clear();

    print_str("Installing interrupt service routines (ISRs).\n");

    isr_install();
    
    print_str("Enabling external interrupts.\n");
    asm volatile("sti");

    print_str("Initializing keyboard (IRQ 1).\n");

    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Welcome to ");
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    print_str("Krysis");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("!");
}

void execute_command(char *input)
{
    if (compare_string(input, "EXIT") == 0)
    {
        print_str("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    print_str("Unknown command: ");
    print_str(input);
    print_str("\n> ");
}