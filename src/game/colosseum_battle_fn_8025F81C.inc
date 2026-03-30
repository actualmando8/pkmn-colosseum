    nofralloc
    mflr r0
    lis r4, -32696
    stw r0, 0x4(r1)
    rlwinm r3, r3, 8, 0, 23
    addi r0, r4, -31776
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    add r31, r0, r3
    stw r30, 0x10(r1)
    bl OSDisableInterrupts
    mr r30, r3
    b @L_8025F854
@L_8025F84C:
    addi r3, r31, 0x24
    bl fn_800A238C
@L_8025F854:
    lwz r0, 0x1c(r31)
    cmplwi r0, 0x0
    bne @L_8025F84C
    lwz r31, 0x20(r31)
    mr r3, r30
    bl OSRestoreInterrupts
    mr r3, r31
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
