#include "print.h"
#include "port.h"

#define IDT_ENTRIES 256

typedef struct {
    uint16_t low_offset;
    uint16_t selector;
    uint8_t always0;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    // data segment selector
    uint32_t ds;
    // general purpose registers pushed by pusha
    uint32_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
    // pushed by isr procedure
    uint32_t int_no, err_code;
    // pushed by CPU automatically
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;


#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

idt_gate_t idt[256];

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].selector = 0x08; // see GDT
    idt[n].always0 = 0;
    // 0x8E = 1  00 0 1  110
    //        P DPL D   Type
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

char* exception_messages[] = {
    "Division by zero",
    "Debug",
    // ...
    "Reserved"
};

void isr_handler(registers_t* r) {
   print_str(exception_messages[r -> int_no]);
   print_nl();
}

    extern int isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17,isr18,isr19,isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29,isr30,isr31, isr31;
    extern int irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15;

void isr_install() {
    

    // internal ISRs
    set_idt_gate(0, (uint32_t) isr0);
    set_idt_gate(1, (uint32_t) isr1);
    set_idt_gate(2, (uint32_t) isr2);
    set_idt_gate(3, (uint32_t) isr3);
    set_idt_gate(4, (uint32_t) isr4);
    set_idt_gate(5, (uint32_t) isr5);
    set_idt_gate(6, (uint32_t) isr6);
    set_idt_gate(7, (uint32_t) isr7);
    set_idt_gate(8, (uint32_t) isr8);
    set_idt_gate(9, (uint32_t) isr9);
    set_idt_gate(10, (uint32_t) isr10);
    set_idt_gate(11, (uint32_t) isr11);
    set_idt_gate(12, (uint32_t) isr12);
    set_idt_gate(13, (uint32_t) isr13);
    set_idt_gate(14, (uint32_t) isr14);
    set_idt_gate(15, (uint32_t) isr15);
    set_idt_gate(16, (uint32_t) isr16);
    set_idt_gate(17, (uint32_t) isr17);
    set_idt_gate(18, (uint32_t) isr18);
    set_idt_gate(19, (uint32_t) isr19);
    set_idt_gate(20, (uint32_t) isr20);
    set_idt_gate(21, (uint32_t) isr21);
    set_idt_gate(22, (uint32_t) isr22);
    set_idt_gate(23, (uint32_t) isr23);
    set_idt_gate(24, (uint32_t) isr24);
    set_idt_gate(25, (uint32_t) isr25);
    set_idt_gate(26, (uint32_t) isr26);
    set_idt_gate(27, (uint32_t) isr27);
    set_idt_gate(28, (uint32_t) isr28);
    set_idt_gate(29, (uint32_t) isr29);
    set_idt_gate(30, (uint32_t) isr30);
    set_idt_gate(31, (uint32_t) isr31);

    // PIV remapping

    // ICW1
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    // ICW2
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x29);

    // ICW3
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);

    // ICW4
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    // OCW1
    port_byte_out(0x20, 0x0);
    port_byte_out(0xA0, 0x0);

    // IRQ ISRs (primary PIC)
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(32, (uint32_t)irq1);
    set_idt_gate(32, (uint32_t)irq2);
    set_idt_gate(32, (uint32_t)irq3);
    set_idt_gate(32, (uint32_t)irq4);
    set_idt_gate(32, (uint32_t)irq5);
    set_idt_gate(32, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);

    // IRQ ISRs (secondary PIC)
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(40, (uint32_t)irq9);
    set_idt_gate(40, (uint32_t)irq10);
    set_idt_gate(40, (uint32_t)irq11);
    set_idt_gate(40, (uint32_t)irq12);
    set_idt_gate(40, (uint32_t)irq13);
    set_idt_gate(40, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    load_idt();
}

typedef void (*isr_t)(registers_t *);

isr_t interrupt_handlers[256];

void irq_handler(registers_t *r) {
    if (interrupt_handlers[r ->int_no] != 0) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
    port_byte_out(0x20, 0x20); // primary EOI
    
    if(r->int_no < 40) {
        port_byte_out(0xA0, 0x20); // secondary EOI
    }
}

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

idt_register_t idt_reg;

void load_idt() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_reg));
}

void kernel_main() {
    print_clear();
  
    print_str("Installing interrupt service routines (ISRs).\n");

    isr_install();

    print_str("Enabling external interrupts.\n");
  // asm volatile("sti"); <- this crashes somehow

    print_str("Initializing keyboard (IRQ 1).\n");
    init_keyboard();

    
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Welcome to ");
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    print_str("Krysis");
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("!");
}

void print_letter(uint8_t scancode) {
    switch (scancode)
    {
    case 0x0:
        print_str("INVALID SCANCODE");
        break;
    
    default:
    if (scancode <= 0x7f) {
        print_str("Unknown key down");
    } else if (scancode <= 0x39+0x80) {
        // key up
    } else {
        // ukn key up
    }
        break;
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

static void keyboard_callback(registers_t* regs) {
    uint8_t scancode = port_byte_in(0x60);
    print_letter(scancode);
    print_nl();
}

#define IRQ1 33

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}