    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r4
    stw r28, 0x10(r1)
    addi r28, r3, 0x0
    mulli r5, r28, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    lwz r30, 0xd0(r31)
    li r0, 0x0
    stw r0, 0xd0(r31)
    blt @L_800B51E0
    lhz r4, 0xbe(r31)
    addi r3, r28, 0x0
    addi r5, r30, 0x0
    bl fn_800B1CC4
    mr. r29, r3
    bge @L_800B5208
    @L_800B51E0:
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    bl fn_800B02F4
    cmplwi r30, 0x0
    beq @L_800B5208
    addi r12, r30, 0x0
    mtlr r12
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    blrl
    @L_800B5208:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
