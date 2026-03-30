    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    beq @L_801DA4CC
    lwz r31, 0x68(r3)
    b @L_801DA4C4
@L_801DA474:
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_801DA4C0
    lwz r30, 0x24(r31)
    b @L_801DA4B8
@L_801DA488:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x3
    bne @L_801DA4B4
    lwz r0, 0x18(r30)
    cmpwi r0, 0x0
    bne @L_801DA4B4
    lwz r3, 0x88(r30)
    cmplwi r3, 0x0
    beq @L_801DA4B4
    mr r4, r29
    bl fn_80118C20
@L_801DA4B4:
    lwz r30, 0xa8(r30)
@L_801DA4B8:
    cmplwi r30, 0x0
    bne @L_801DA488
@L_801DA4C0:
    lwz r31, 0x34(r31)
@L_801DA4C4:
    cmplwi r31, 0x0
    bne @L_801DA474
@L_801DA4CC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
