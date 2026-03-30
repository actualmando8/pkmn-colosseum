    nofralloc
    mflr r0
    lis r5, -32704
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r5, -18368
    stw r30, 0x20(r1)
    addi r30, r4, 0x0
    stw r29, 0x1c(r1)
    addi r29, r3, 0x0
    bl OSDisableInterrupts
    lwz r0, 0x48(r31)
    addi r4, r31, 0x48
    cmpwi r0, 0x0
    beq @L_800A11A4
    bl OSRestoreInterrupts
    li r3, 0x0
    b @L_800A11B4
@L_800A11A4:
    stw r3, 0x44(r31)
    li r0, 0x1
    addi r3, r31, 0x14
    stw r0, 0x0(r4)
@L_800A11B4:
    rlwinm r0, r29, 1, 0, 30
    add r4, r3, r0
    lhzu r3, 0x1c(r4)
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    beq @L_800A11E0
    sth r30, 0x0(r4)
    li r3, 0x1
    li r4, 0x14
    bl fn_800A09B0
    b @L_800A11EC
@L_800A11E0:
    li r3, 0x0
    li r4, 0x14
    bl fn_800A09B0
@L_800A11EC:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
