    nofralloc
    mflr r0
    lis r4, 0x8048
    stw r0, 0x4(r1)
    slwi r3, r3, 8
    addi r0, r4, -31776
    add r3, r0, r3
    stwu r1, -0x8(r1)
    addi r3, r3, 0x24
    bl fn_800A2478
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
