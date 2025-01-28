#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivannia Gomez");
MODULE_DESCRIPTION("PMUON");

#define ARMV8_PMUSERENR_EN_EL0  (1 << 0) /* EL0 access enable */
#define ARMV8_PMUSERENR_CR      (1 << 2) /* Cycle counter read enable */
#define ARMV8_PMUSERENR_ER      (1 << 3) /* Event counter read enable */
#define ARMV8_PMCR_E            (1 << 0) /*  Enable all counters */
#define ARMV8_PMCR_P            (1 << 1) /*  Reset all counters */
#define ARMV8_PMCR_C            (1 << 2) /*  Cycle counter reset */
#define ARMV8_PMCNTENSET_EL0_ENABLE (1<<31) /* *< Enable Perf count reg */
#define INSTRUCTIONS_EXECUTED_EVENT 0x08 /* Event of Instruction architecturally executed */
#define L1_ACCESS_EVENT 0x4 /* L1 access */
#define L1_MISS_EVENT 0x3 /* L1 miss */
#define L2_ACCESS_EVENT 0x16 /* L2 access */
#define L2_MISS_EVENT 0x17 /* L2 miss */
#define L3_MISS_EVENT 0x37 /* L3 miss */

void set_pmu(void* dummy) {
    //unsigned int v;
    uint64_t v;
    printk("Turn PMU on Core %d\n", smp_processor_id());

    // 1. Enable "User Enable Register"
    asm volatile("msr pmuserenr_el0, %0" : : "r"(ARMV8_PMUSERENR_EN_EL0|ARMV8_PMUSERENR_ER|ARMV8_PMUSERENR_CR));
    
    // 2. Reset i) Performance Monitor Control Register(PMCR),
    // ii) Count Enable Set Register, and
    // iii) Overflow Flag Status Register
    asm volatile("msr pmcntenset_el0, %0" : : "r" (ARMV8_PMCNTENSET_EL0_ENABLE));//enable performance counter
    asm volatile("msr pmcr_el0, %0" : : "r" (ARMV8_PMCR_P | ARMV8_PMCR_C));//reset all counter, reset cycle counter

    // 3. Disable Interrupt Enable Clear Register
    asm volatile("msr pmintenset_el1, %0" : : "r" ((0 << 31)));//disable cycle counter interrupt

//start
	asm volatile("mrs %0, pmcr_el0" : "=r" (v));
    asm volatile("msr pmcr_el0, %0" : : "r" (v|ARMV8_PMCR_E));
//

    // 4. Read how many event counters exist 
    //asm volatile("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r" (v)); // Read PMCR
    asm volatile("mrs %0, pmcr_el0" : "=r" (v));
    printk("We have %lld configurable event counters on Core %d\n",
            (v >> 11) & 0x1f, smp_processor_id());

    // 5. Set six event counter registers (Project Assignment you need to IMPLEMENT)

     // ***** YOUR CODE STARTS HERE *****
    // set event for # of instructions

    // Configure Event Counter 0
    asm volatile("msr pmselr_el0, %0" : : "r"(0)); // Select event counter to 0
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(INSTRUCTIONS_EXECUTED_EVENT)); // 
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << 0)); // Enable counter 0
    
    asm volatile("msr pmselr_el0, %0" : : "r"(1)); // Select event counter to 1
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(L1_ACCESS_EVENT)); // 
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << 1)); // Enable counter 1

    asm volatile("msr pmselr_el0, %0" : : "r"(2)); // Select event counter to 2
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(L1_MISS_EVENT)); // L1 miss
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << 2)); // Enable counter 2

    asm volatile("msr pmselr_el0, %0" : : "r"(3)); // Select event counter to 3
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(L2_ACCESS_EVENT)); // L2 access
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << 3)); // Enable counter 3
    
    asm volatile("msr pmselr_el0, %0" : : "r"(4)); // Select event counter to 4
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(L2_MISS_EVENT)); // L2 miss
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << 4)); // Enable counter 4

    asm volatile("msr pmselr_el0, %0" : : "r"(5)); // Select event counter to 5
    asm volatile("msr pmxevtyper_el0, %0" : : "r"(L3_MISS_EVENT)); // L3 miss
    asm volatile("msr pmcntenset_el0, %0" : : "r"(1 << 5)); // Enable counter 5


    //asm volatile("mrs %0, pmxevcntr_el0" : "=r"(v)); // Read the counter value
    //printk("[Core %d] Instructions: %lld\n", smp_processor_id(), v);


    // ***** YOUR CODE ENDS HERE *******

}


int init_module(void) {
    // Set Performance Monitoring Unit (aka Performance Counter) across all cores 
    on_each_cpu(set_pmu, NULL, 1);
    printk("Ready to use PMU\n");
    return 0;
}

void cleanup_module(void) {
    printk("PMU Kernel Module Off\n");
}

