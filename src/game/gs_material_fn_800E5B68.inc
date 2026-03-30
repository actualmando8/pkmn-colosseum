    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    lhz r30, 0x150(r3)
    lwz r29, 0x14c(r3)
    b @L_800E5BB4
    @L_800E5B98:
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800E5BAC
    mr r4, r28
    bl fn_800DF1E4
    @L_800E5BAC:
    addi r31, r31, 0x1
    addi r29, r29, 0x4
    @L_800E5BB4:
    clrlwi r0, r31, 16
    cmplw r0, r30
    blt @L_800E5B98
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
