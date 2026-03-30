    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r3, 0x0
    lis r3, lbl_803FC620@ha
    stw r30, 0x20(r1)
    mulli r5, r31, 0x110
    stw r29, 0x1c(r1)
    addi r0, r3, lbl_803FC620@l
    mr. r29, r4
    add r30, r0, r5
    blt @L_800B1B50
    lwz r5, 0x88(r30)
    lis r3, fn_800B1A10@ha
    lwz r0, 0x80(r30)
    addi r7, r3, fn_800B1A10@l
    lwz r3, 0xc(r30)
    subf r0, r0, r5
    srwi r0, r0, 13
    mullw r4, r3, r0
    addi r6, r5, 0x0
    addi r3, r31, 0x0
    li r5, 0x2000
    bl fn_800B19A4
    mr. r29, r3
    bge @L_800B1B90
    @L_800B1B50:
    lwz r0, 0xd0(r30)
    cmplwi r0, 0x0
    bne @L_800B1B68
    addi r3, r30, 0x0
    addi r4, r29, 0x0
    bl fn_800B02F4
    @L_800B1B68:
    lwz r0, 0xd8(r30)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800B1B90
    li r0, 0x0
    mtlr r12
    stw r0, 0xd8(r30)
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    blrl
    @L_800B1B90:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
