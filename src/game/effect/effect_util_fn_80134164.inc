    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    ble @L_801341A0
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80134190
    li r3, 0x0
    b @L_80134198
    @L_80134190:
    mtctr r12
    bctrl
    @L_80134198:
    cmpw r3, r31
    bgt @L_801341A8
    @L_801341A0:
    li r3, 0x0
    b @L_80134214
    @L_801341A8:
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_801341BC
    li r3, 0x0
    b @L_801341C8
    @L_801341BC:
    mr r3, r31
    mtctr r12
    bctrl
    @L_801341C8:
    cmplwi r3, 0x0
    bne @L_801341D8
    li r31, 0x0
    b @L_801341DC
    @L_801341D8:
    lha r31, 0x2(r3)
    @L_801341DC:
    extsh. r0, r31
    ble @L_8013420C
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_801341F8
    li r3, 0x0
    b @L_80134200
    @L_801341F8:
    mtctr r12
    bctrl
    @L_80134200:
    extsh r0, r31
    cmpw r3, r0
    bgt @L_80134210
    @L_8013420C:
    li r31, 0x0
    @L_80134210:
    mr r3, r31
    @L_80134214:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
