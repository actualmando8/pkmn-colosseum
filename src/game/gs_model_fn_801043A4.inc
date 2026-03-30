    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    cmpwi r29, 0x0
    bgt @L_801043D0
    li r31, 0x0
    b @L_80104400
@L_801043D0:
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r31, 0xc(r3)
    b @L_801043F4
@L_801043E0:
    lwz r0, 0x4(r31)
    cmpw r0, r29
    bne @L_801043F0
    b @L_80104400
@L_801043F0:
    lwz r31, 0x10(r31)
@L_801043F4:
    cmplwi r31, 0x0
    bne @L_801043E0
    li r31, 0x0
@L_80104400:
    cmplwi r31, 0x0
    beq @L_801044AC
    lbz r0, 0x99(r31)
    cmplwi r0, 0x0
    beq @L_8010441C
    li r30, -0x1
    b @L_801044B0
@L_8010441C:
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    cmplwi r3, 0x0
    beq @L_80104494
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 30, 31
    cmpwi r0, 0x2
    beq @L_8010447C
    bge @L_80104450
    cmpwi r0, 0x0
    beq @L_8010445C
    bge @L_80104464
    b @L_801044B0
@L_80104450:
    cmpwi r0, 0x4
    bge @L_801044B0
    b @L_80104484
@L_8010445C:
    li r30, 0x0
    b @L_801044B0
@L_80104464:
    lbz r3, 0x94(r31)
    lbz r0, 0x95(r31)
    extsb r3, r3
    extsb r0, r0
    add r30, r3, r0
    b @L_801044B0
@L_8010447C:
    lwz r30, 0x80(r31)
    b @L_801044B0
@L_80104484:
    mr r3, r29
    bl fn_801022B8
    mr r30, r3
    b @L_801044B0
@L_80104494:
    lbz r3, 0x94(r31)
    lbz r0, 0x95(r31)
    extsb r3, r3
    extsb r0, r0
    add r30, r3, r0
    b @L_801044B0
@L_801044AC:
    li r30, -0x1
@L_801044B0:
    mr r3, r30
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
