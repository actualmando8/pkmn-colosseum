    nofralloc
    mflr r0
    lis r3, -32704
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r3, -18368
    bl OSDisableInterrupts
    lwz r0, 0x48(r31)
    addi r4, r31, 0x48
    cmpwi r0, 0x0
    beq @L_800A10A4
    bl OSRestoreInterrupts
    li r31, 0x0
    b @L_800A10B0
@L_800A10A4:
    stw r3, 0x44(r31)
    li r0, 0x1
    stw r0, 0x0(r4)
@L_800A10B0:
    lbz r31, 0x12(r31)
    li r3, 0x0
    li r4, 0x0
    bl fn_800A09B0
    mr r3, r31
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
