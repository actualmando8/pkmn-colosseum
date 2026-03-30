    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    bl fn_8005DA18
    cmplwi r3, 0x0
    bne @L_80102168
    li r3, -0x3
    b @L_801021DC
@L_80102168:
    lha r0, 0x4(r3)
    li r30, 0x0
    mr r31, r0
@L_80102174:
    extsh r3, r31
    bl fn_8005D934
    extsh r0, r31
    cmplw r0, r29
    bne @L_801021A8
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801021A0
    mr r3, r30
    b @L_801021DC
@L_801021A0:
    li r3, -0x1
    b @L_801021DC
@L_801021A8:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801021BC
    addi r30, r30, 0x1
@L_801021BC:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_801021D8
    lha r0, 0x18(r3)
    mr r31, r0
    b @L_80102174
@L_801021D8:
    li r3, -0x2
@L_801021DC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
