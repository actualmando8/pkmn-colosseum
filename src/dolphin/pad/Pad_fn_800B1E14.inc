    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr. r30, r4
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    mulli r5, r29, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    blt @L_800B1E88
    lwz r3, 0x80(r31)
    lwz r0, 0x84(r31)
    addi r4, r3, 0x2000
    cmplw r0, r4
    addi r0, r3, 0x4000
    bne @L_800B1E74
    stw r0, 0x84(r31)
    mr r3, r0
    li r5, 0x2000
    bl memcpy
    b @L_800B1E88
    @L_800B1E74:
    stw r4, 0x84(r31)
    addi r3, r4, 0x0
    mr r4, r0
    li r5, 0x2000
    bl memcpy
    @L_800B1E88:
    lwz r0, 0xd0(r31)
    cmplwi r0, 0x0
    bne @L_800B1EA0
    addi r3, r31, 0x0
    addi r4, r30, 0x0
    bl fn_800B02F4
    @L_800B1EA0:
    lwz r0, 0xd8(r31)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800B1EC8
    li r0, 0x0
    mtlr r12
    stw r0, 0xd8(r31)
    addi r3, r29, 0x0
    addi r4, r30, 0x0
    blrl
    @L_800B1EC8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
