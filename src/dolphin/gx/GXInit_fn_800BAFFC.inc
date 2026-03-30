    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r4, 0x0
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    lwz r5, -29632(r2)
    lwz r12, 0x410(r5)
    mtlr r12
    blrl
    addi r4, r3, 0x0
    addi r3, r30, 0x0
    addi r5, r31, 0x0
    bl fn_800BAE64
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
