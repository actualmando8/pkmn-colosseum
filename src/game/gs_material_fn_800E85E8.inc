    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    lhz r30, 0x150(r3)
    lwz r29, 0x14c(r3)
    b @L_800E8630
    @L_800E8618:
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800E8628
    bl fn_800DF608
    @L_800E8628:
    addi r31, r31, 0x1
    addi r29, r29, 0x4
    @L_800E8630:
    cmpw r31, r30
    blt @L_800E8618
    lhz r31, 0x154(r28)
    cmplwi r31, 0x0
    beq @L_800E8654
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_800E8654:
    li r0, 0x0
    sth r0, 0x154(r28)
    sth r0, 0x150(r28)
    stw r0, 0x14c(r28)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
