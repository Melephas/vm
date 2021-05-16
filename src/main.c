#include <stdio.h>
#include <stdint.h>
#include <string.h>


#define MEMORY_SIZE UINT8_MAX
#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25

//#define BOX_DRAW_CHARS "─│┌┐└┘"
#define BOX_DRAW_CHARS "-|++++"

struct registers {
    uint8_t a, b, c, d, sp, ip, flags, bp, ibp;
};

struct video_memory {
    uint8_t bank [MEMORY_SIZE * 8];
    uint8_t* bank_0;
    uint8_t* bank_1;
    uint8_t* bank_2;
    uint8_t* bank_3;
    uint8_t* bank_4;
    uint8_t* bank_5;
    uint8_t* bank_6;
    uint8_t* bank_7;
};

struct cpu {
    struct registers regs;
    struct video_memory video_memory;
    uint8_t memory[MEMORY_SIZE];
};

enum instruction {
    HLT  ,
    MOVRR,
    MOVRM,
    MOVMR,
    MOVIR,
    ADD  ,
    SUB  ,
    CMP  ,
    JEZ  ,
    JGZ  ,
    JMPI ,
    JMPR ,
    CALL ,
    RET  ,
    BSEL ,
    IBSEL,
};

void print_memory(uint8_t* memory) {
    // UINT8_MAX should be 0xFF
    printf("#    00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F\n");
    for (int i = 0; i <= 0xF; i++) { // Upper nibble
        printf("%02X", i<<4);
        for (int j = 0; j <= 0xF; j++) { // Lower nibble
            uint8_t memory_offset = (i<<4)+j;
            printf(" 0x%02X", memory[memory_offset]);
        }
        printf("\n");
    }
}

void print_video_memory(struct video_memory memory) {
    printf("%c", BOX_DRAW_CHARS[2]);
    for (int i = 0; i < VIDEO_WIDTH + 2; i++) {
        printf("%c", BOX_DRAW_CHARS[0]);
    }
    printf("%c\n", BOX_DRAW_CHARS[3]);

    for (int i = 0; i < VIDEO_HEIGHT; i++) {
        printf("%c", BOX_DRAW_CHARS[1]);
        for (int j = 0; j < VIDEO_WIDTH; j++) {
            uint16_t address = (i*VIDEO_WIDTH) + j;
            uint8_t data = memory.bank[address];
            if (data == 0) {
                data = ' ';
            }
            printf("%c", data);
        }
        printf("%c\n", BOX_DRAW_CHARS[1]);
        fflush(stdout);
    }
    
    printf("%c", BOX_DRAW_CHARS[4]);
    for (int i = 0; i < VIDEO_WIDTH + 2; i++) {
        printf("%c", BOX_DRAW_CHARS[0]);
    }
    printf("%c\n", BOX_DRAW_CHARS[5]);
}

void print_registers(struct registers regs) {
    printf("A    B    C    D    SP   IP   FLG  BP   IBP\n");
    printf("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
           regs.a, regs.b, regs.c, regs.d, regs.sp, regs.ip, regs.flags, regs.bp, regs.ibp);
}

void print_cpu_state(struct cpu cpu) {
    print_registers(cpu.regs);
    print_memory(cpu.memory);
    print_video_memory(cpu.video_memory);
}

struct video_memory new_video_memory() {
    struct video_memory memory;
    memset(memory.bank, 0, MEMORY_SIZE * 8);

    memory.bank_0 = memory.bank;
    memory.bank_1 = memory.bank + MEMORY_SIZE;
    memory.bank_2 = memory.bank + (MEMORY_SIZE * 2);
    memory.bank_3 = memory.bank + (MEMORY_SIZE * 3);
    memory.bank_4 = memory.bank + (MEMORY_SIZE * 4);
    memory.bank_5 = memory.bank + (MEMORY_SIZE * 5);
    memory.bank_6 = memory.bank + (MEMORY_SIZE * 6);
    memory.bank_7 = memory.bank + (MEMORY_SIZE * 7);

    return memory;
}

int main() {
    struct cpu default_cpu = {
        .regs = {
            .a = 0,
            .b = 0,
            .c = 0,
            .d = 0,
            .sp = 0,
            .ip = 0,
            .flags = 0,
        },
        .video_memory = new_video_memory(),
    };
    
    // Clear memory
    memset(default_cpu.video_memory.bank, 0, MEMORY_SIZE * 8);
    memset(default_cpu.memory, 0, MEMORY_SIZE);

    // This is only ok here because it's known that the string is shorter than the length of the memory
    char message[] = "Hello, World!";
    memcpy(default_cpu.video_memory.bank, message, sizeof(message));

    print_cpu_state(default_cpu);
    // Process args
    // Load binary
    // Prepare for execution
    return 0;
}
