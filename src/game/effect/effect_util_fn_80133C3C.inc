    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80133D78
    lbz r4, 0x94(r3)
    lbz r0, 0x95(r3)
    extsb r4, r4
    extsb r0, r0
    add r4, r4, r0
    bl fn_80133E6C
    mr. r31, r3
    ble @L_80133CA4
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80133C94
    li r3, 0x0
    b @L_80133C9C
    @L_80133C94:
    mtctr r12
    bctrl
    @L_80133C9C:
    cmpw r3, r31
    bgt @L_80133CAC
    @L_80133CA4:
    li r31, 0x0
    b @L_80133D14
    @L_80133CAC:
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133CC0
    li r3, 0x0
    b @L_80133CCC
    @L_80133CC0:
    mr r3, r31
    mtctr r12
    bctrl
    @L_80133CCC:
    cmplwi r3, 0x0
    bne @L_80133CDC
    li r31, 0x0
    b @L_80133CE0
    @L_80133CDC:
    lha r31, 0x2(r3)
    @L_80133CE0:
    extsh. r0, r31
    ble @L_80133D10
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80133CFC
    li r3, 0x0
    b @L_80133D04
    @L_80133CFC:
    mtctr r12
    bctrl
    @L_80133D04:
    extsh r0, r31
    cmpw r3, r0
    bgt @L_80133D14
    @L_80133D10:
    li r31, 0x0
    @L_80133D14:
    extsh r31, r31
    li r29, 0x0
    li r30, 0x0
    b @L_80133D6C
    @L_80133D24:
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133D38
    li r3, 0x0
    b @L_80133D44
    @L_80133D38:
    mr r3, r30
    mtctr r12
    bctrl
    @L_80133D44:
    cmplwi r3, 0x0
    bne @L_80133D54
    li r0, 0x1
    b @L_80133D5C
    @L_80133D54:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    @L_80133D5C:
    clrlwi. r0, r0, 24
    beq @L_80133D68
    addi r29, r29, 0x1
    @L_80133D68:
    addi r30, r30, 0x1
    @L_80133D6C:
    cmpw r30, r31
    blt @L_80133D24
    b @L_80133DFC
    @L_80133D78:
    li r29, 0x0
    li r31, 0x0
    b @L_80133DCC
    @L_80133D84:
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133D98
    li r3, 0x0
    b @L_80133DA4
    @L_80133D98:
    mr r3, r31
    mtctr r12
    bctrl
    @L_80133DA4:
    cmplwi r3, 0x0
    bne @L_80133DB4
    li r0, 0x1
    b @L_80133DBC
    @L_80133DB4:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    @L_80133DBC:
    clrlwi. r0, r0, 24
    beq @L_80133DC8
    addi r29, r29, 0x1
    @L_80133DC8:
    addi r31, r31, 0x1
    @L_80133DCC:
    bl fn_800057A8
    cmpwi r3, 0x1
    beq @L_80133DF0
    bge @L_80133DE0
    b @L_80133DF0
    @L_80133DE0:
    cmpwi r3, 0x3
    bge @L_80133DF0
    li r0, 0x115
    b @L_80133DF4
    @L_80133DF0:
    li r0, 0x2
    @L_80133DF4:
    cmpw r31, r0
    blt @L_80133D84
    @L_80133DFC:
    lwz r0, 0x24(r1)
    mr r3, r29
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
