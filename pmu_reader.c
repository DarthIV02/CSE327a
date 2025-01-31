// UCSD CSE237A - WI23

// Important! TODO: SUMBIT this file in the part1 project.
// You only need to change the "get_single_event()" function.
// For more details, please see the instructions in the class website.

// pmu_reader.h: PMU Reading interface

#include "pmu_reader.h"

void reset_counters(void) {
    // Reset all cycle counter and event counters
   
    asm volatile("msr pmcr_el0, %0" : : "r" ((u64)ARMV8_PMCR_P | ARMV8_PMCR_C|ARMV8_PMCR_E));//reset all counter, reset cycle counter

    uint32_t r = 0;
    asm volatile("mrs %0, pmcntenset_el0" : "=r" (r));
	asm volatile("msr pmcntenset_el0, %0" : : "r" (r|ARMV8_PMCNTENSET_EL0_ENABLE_ALL_EVENTS));





}

unsigned int get_cyclecount(void) {
    uint64_t value;
    //asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
    asm volatile("mrs %0, pmccntr_el0" : "=r" (value));


    return (unsigned int)value;
}

// Fill your code in this function to read an event counter.
// One of the following numbers will be given to "cnt_index" 
// to specify the counter index:
// (these are defined in pmu_reader.h)
//#define CNT_INST_EXE 0x00
//#define CNT_L1D_ACSS 0x01
//#define CNT_L1D_MISS 0x02
//#define CNT_L2D_ACSS 0x03
//#define CNT_L2D_MISS 0x04
//#define CNT_L3D_MISS 0x05

unsigned int get_single_event(unsigned int cnt_index) {
    unsigned int value = 0;
    // Implement your code here
    
    asm volatile("msr pmselr_el0, %0" : : "r"(cnt_index)); // Select event counter cnt_index
    asm volatile("mrs %0, pmxevcntr_el0" : "=r"(value)); // Read the counter value

    return value;



}

