/***************************************************************
 * This confidential and  proprietary  software may be used only
 * as authorised  by  a licensing  agreement  from  ARM  Limited
 *
 *             (C) COPYRIGHT 2013-2015 ARM Limited
 *                      ALL RIGHTS RESERVED
 *
 *  The entire notice above must be reproduced on all authorised
 *  copies and copies  may only be made to the  extent permitted
 *  by a licensing agreement from ARM Limited.
 *
 ***************************************************************/
#ifndef __VMPU_EXPORTS_H__
#define __VMPU_EXPORTS_H__

/* supervisor user access modes */
#define UVISOR_TACL_UEXECUTE        0x0001UL
#define UVISOR_TACL_UWRITE          0x0002UL
#define UVISOR_TACL_UREAD           0x0004UL
#define UVISOR_TACL_UACL            (UVISOR_TACL_UREAD          |\
                                     UVISOR_TACL_UWRITE         |\
                                     UVISOR_TACL_UEXECUTE)

/* supervisor access modes */
#define UVISOR_TACL_SEXECUTE        0x0008UL
#define UVISOR_TACL_SWRITE          0x0010UL
#define UVISOR_TACL_SREAD           0x0020UL
#define UVISOR_TACL_SACL            (UVISOR_TACL_SREAD          |\
                                     UVISOR_TACL_SWRITE         |\
                                     UVISOR_TACL_SEXECUTE)

/* all possible access control flags */
#define UVISOR_TACL_ACCESS          (UVISOR_TACL_UACL | UVISOR_TACL_SACL)

/* various modes */
#define UVISOR_TACL_STACK           0x0040UL
#define UVISOR_TACL_SIZE_ROUND_UP   0x0080UL
#define UVISOR_TACL_SIZE_ROUND_DOWN 0x0100UL
#define UVISOR_TACL_PERIPHERAL      0x0200UL
#define UVISOR_TACL_SHARED          0x0400UL
#define UVISOR_TACL_USER            0x0800UL
#define UVISOR_TACL_IRQ             0x1000UL

#define UVISOR_TACLDEF_SECURE_BSS   (UVISOR_TACL_UREAD          |\
                                     UVISOR_TACL_UWRITE         |\
                                     UVISOR_TACL_SREAD          |\
                                     UVISOR_TACL_SWRITE         |\
                                     UVISOR_TACL_SIZE_ROUND_UP)

#define UVISOR_TACLDEF_SECURE_CONST (UVISOR_TACL_UREAD          |\
                                     UVISOR_TACL_SREAD          |\
                                     UVISOR_TACL_SIZE_ROUND_UP)

#define UVISOR_TACLDEF_DATA         UVISOR_TACLDEF_SECURE_BSS

#define UVISOR_TACLDEF_PERIPH       (UVISOR_TACL_PERIPHERAL     |\
                                     UVISOR_TACL_UREAD          |\
                                     UVISOR_TACL_UWRITE         |\
                                     UVISOR_TACL_SREAD          |\
                                     UVISOR_TACL_SWRITE         |\
                                     UVISOR_TACL_SIZE_ROUND_UP)

#define UVISOR_TACLDEF_STACK        (UVISOR_TACL_STACK          |\
                                     UVISOR_TACL_UREAD          |\
                                     UVISOR_TACL_UWRITE         |\
                                     UVISOR_TACL_SREAD          |\
                                     UVISOR_TACL_SWRITE)

#define UVISOR_TO_STR(x)            #x
#define UVISOR_TO_STRING(x)         UVISOR_TO_STR(x)
#define UVISOR_ARRAY_COUNT(x)       (sizeof(x)/sizeof(x[0]))
#define UVISOR_PAD32(x)             (32 - (sizeof(x) & ~0x1FUL))
#define UVISOR_BOX_MAGIC            0x42CFB66FUL
#define UVISOR_BOX_VERSION          100
#define UVISOR_STACK_BAND_SIZE      128
#define UVISOR_MEM_SIZE_ROUND(x)    UVISOR_REGION_ROUND_UP(x)

#define UVISOR_MIN_STACK_SIZE       1024
#define UVISOR_MIN_STACK(x)         (((x)<UVISOR_MIN_STACK_SIZE)?UVISOR_MIN_STACK_SIZE:(x))

#ifdef  ARCH_MK64F
#define UVISOR_REGION_ROUND_DOWN(x) ((x) & ~0x1FUL)
#define UVISOR_REGION_ROUND_UP(x)   UVISOR_ROUND32_DOWN((x)+31UL)
#define UVISOR_STACK_SIZE_ROUND(x)  UVISOR_REGION_ROUND_UP((x) + \
                                    (UVISOR_STACK_BAND_SIZE * 2))
#else /*ARM MPU*/
#define UVISOR_REGION_ROUND_DOWN(x) ((x) & ~((1UL<<UVISOR_REGION_BITS(x))-1))
#define UVISOR_REGION_ROUND_UP(x)   (1UL<<UVISOR_REGION_BITS(x))
#define UVISOR_STACK_SIZE_ROUND(x)  UVISOR_REGION_ROUND_UP(x)
#endif/*ARCH_MK64F*/

#ifndef UVISOR_BOX_STACK_SIZE
#define UVISOR_BOX_STACK_SIZE UVISOR_MIN_STACK_SIZE
#endif/*UVISOR_BOX_STACK*/

typedef uint32_t UvisorBoxAcl;

typedef struct
{
    void* param1;
    uint32_t param2;
    UvisorBoxAcl acl;
} UVISOR_PACKED UvisorBoxAclItem;

typedef struct
{
    uint32_t magic;
    uint32_t version;
    uint32_t stack_size;
    uint32_t context_size;

    const UvisorBoxAclItem* const acl_list;
    uint32_t acl_count;
} UVISOR_PACKED UvisorBoxConfig;

/*
 * only use this macro for rounding const values during compile time:
 * for variables please use uvisor_region_bits(x) instead
 */
#define UVISOR_REGION_BITS(x)       (((x)<=32UL)?5:(((x)<=64UL)?\
    6:(((x)<=128UL)?7:(((x)<=256UL)?8:(((x)<=512UL)?9:(((x)<=1024UL)?\
    10:(((x)<=2048UL)?11:(((x)<=4096UL)?12:(((x)<=8192UL)?\
    13:(((x)<=16384UL)?14:(((x)<=32768UL)?15:(((x)<=65536UL)?\
    16:(((x)<=131072UL)?17:(((x)<=262144UL)?18:(((x)<=524288UL)?\
    19:(((x)<=1048576UL)?20:(((x)<=2097152UL)?21:(((x)<=4194304UL)?\
    22:(((x)<=8388608UL)?23:(((x)<=16777216UL)?24:(((x)<=33554432UL)?\
    25:(((x)<=67108864UL)?26:(((x)<=134217728UL)?27:(((x)<=268435456UL)?\
    28:(((x)<=536870912UL)?29:(((x)<=1073741824UL)?30:(((x)<=2147483648UL)?\
    31:32)))))))))))))))))))))))))))

static inline int vmpu_bits(uint32_t size)
{
    int bits=0;
    /* find highest bit */
    while(size)
    {
        size>>=1;
        bits++;
    }
    return bits;
}

static inline int vmpu_region_bits(uint32_t size)
{
    int bits;

    bits = vmpu_bits(size)-1;

    /* minimum region size is 32 bytes */
    if(bits<5)
        bits=5;

    /* round up if needed */
    if((1UL << bits) != size)
        bits++;

    return bits;
}

#endif/*__VMPU_EXPORTS_H__*/
