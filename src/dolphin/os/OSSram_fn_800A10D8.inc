    nofralloc
    mflr r0
    lis r4, -32704
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r4, -18368
    stw r30, 0x18(r1)
    addi r30, r3, 0x0
    bl OSDisableInterrupts
    lwz r0, 0x48(r31)
    addi r4, r31, 0x48
    cmpwi r0, 0x0
    beq @L_800A1118
    bl OSRestoreInterrupts
    li r3, 0x0
    b @L_800A1128
@L_800A1118:
    stw r3, 0x44(r31)
    li r0, 0x1
    addi r3, r31, 0x14
    stw r0, 0x0(r4)
@L_800A1128:
    rlwinm r0, r30, 1, 0, 30
    add r3, r3, r0
    lhz r31, 0x1c(r3)
    li r3, 0x0
    li r4, 0x14
    bl fn_800A09B0
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
