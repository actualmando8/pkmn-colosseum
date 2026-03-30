    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x10(r1)
    stw r31, 0xc(r1)
    bl OSDisableInterrupts
    mr r31, r3
    b @L_800B8DCC
@L_800B8DC4:
    addi r3, r13, -24148
    bl fn_800A238C
@L_800B8DCC:
    lbz r0, -24152(r13)
    cmplwi r0, 0x0
    beq @L_800B8DC4
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr
