    nofralloc
    mflr r0
    lis r4, -32704
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r4, -18368
    stw r30, 0x18(r1)
    clrlslwi r30, r3, 31, 2
    bl OSDisableInterrupts
    lwz r0, 0x48(r31)
    addi r4, r31, 0x48
    cmpwi r0, 0x0
    beq @L_800A0EF4
    bl OSRestoreInterrupts
    li r31, 0x0
    b @L_800A0F00
@L_800A0EF4:
    stw r3, 0x44(r31)
    li r0, 0x1
    stw r0, 0x0(r4)
@L_800A0F00:
    lbz r3, 0x13(r31)
    rlwinm r0, r3, 0, 29, 29
    cmplw r30, r0
    bne @L_800A0F20
    li r3, 0x0
    li r4, 0x0
    bl fn_800A09B0
    b @L_800A0F40
@L_800A0F20:
    rlwinm r0, r3, 0, 30, 28
    stb r0, 0x13(r31)
    li r3, 0x1
    li r4, 0x0
    lbz r0, 0x13(r31)
    or r0, r0, r30
    stb r0, 0x13(r31)
    bl fn_800A09B0
@L_800A0F40:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
