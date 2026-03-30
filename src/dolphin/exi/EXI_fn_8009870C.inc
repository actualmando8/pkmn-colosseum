    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stmw r28, 0x10(r1)
    mr r30, r3
    stw r4, 0xc(r1)
    rlwinm r4, r3, 6, 0, 25
    lis r3, -32704
    addi r0, r3, -19512
    add r31, r0, r4
    bl OSDisableInterrupts
    mr r28, r3
    lwz r29, 0x0(r31)
    lwz r0, 0xc(r1)
    stw r0, 0x0(r31)
    cmpwi r30, 0x2
    beq @L_80098760
    addi r3, r30, 0x0
    addi r4, r31, 0x0
    bl fn_80098110
    b @L_80098770
@L_80098760:
    li r3, 0x0
    lis r4, -32704
    addi r4, r4, -19512
    bl fn_80098110
@L_80098770:
    mr r3, r28
    bl OSRestoreInterrupts
    mr r3, r29
    lwz r0, 0x24(r1)
    lmw r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
