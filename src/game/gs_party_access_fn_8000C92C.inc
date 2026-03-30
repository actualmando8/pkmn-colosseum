    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    b @L_8000C9F4
    @L_8000C948:
    lwz r3, lbl_80478E28(r13)
    lwz r0, 0x0(r3)
    cmplw r31, r0
    bge @L_8000C9F4
    li r29, 0x0
    li r30, 0x0
    b @L_8000C9A0
    @L_8000C964:
    cmpw r31, r29
    beq @L_8000C998
    lwz r3, lbl_80478E2C(r13)
    lhzx r3, r3, r30
    bl fn_801666BC
    cmpwi r3, 0x4
    bge @L_8000C998
    cmpwi r3, 0x1
    bge @L_8000C98C
    b @L_8000C998
    @L_8000C98C:
    lwz r3, lbl_80478E2C(r13)
    lhzx r3, r3, r30
    bl fn_80166B18
    @L_8000C998:
    addi r30, r30, 0x2
    addi r29, r29, 0x1
    @L_8000C9A0:
    lwz r3, lbl_80478E28(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    blt @L_8000C964
    lwz r3, lbl_80478E2C(r13)
    slwi r0, r31, 1
    lhzx r31, r3, r0
    mr r3, r31
    bl fn_801666BC
    cmpwi r3, 0x0
    beq @L_8000C9E4
    blt @L_8000C9E4
    cmpwi r3, 0x4
    bge @L_8000C9E4
    mr r3, r31
    bl fn_80166B18
    b @L_8000C9F4
    @L_8000C9E4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
    @L_8000C9F4:
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    mr r31, r3
    cmpwi r31, -0x1
    bne @L_8000C948
    li r3, 0x2
    bl fn_80102510
    li r3, -0x1
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
