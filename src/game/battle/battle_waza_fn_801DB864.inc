    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, 0x8046
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    addi r3, r4, 31936
    lwz r31, 0x0(r3)
    lhz r29, 0x4(r3)
    b @L_801DB8D4
@L_801DB89C:
    cmplw r28, r31
    beq @L_801DB8CC
    lbz r0, 0x74(r31)
    cmplwi r0, 0x0
    beq @L_801DB8CC
    lwz r0, 0x24(r31)
    cmplwi r0, 0x0
    beq @L_801DB8CC
    mr r3, r31
    li r4, 0x1
    li r5, 0x0
    bl fn_801DEF0C
@L_801DB8CC:
    addi r30, r30, 0x1
    addi r31, r31, 0x8c
@L_801DB8D4:
    cmpw r30, r29
    blt @L_801DB89C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
