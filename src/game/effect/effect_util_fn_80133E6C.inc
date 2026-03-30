    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r0, 0x4(r3)
    li r3, -0x1
    lwz r4, lbl_80478848(r13)
    cmpw r0, r4
    blt @L_80133EA0
    subf r3, r4, r0
    @L_80133EA0:
    cmpwi r3, 0x0
    bge @L_80133EB0
    li r3, 0x0
    b @L_80134148
    @L_80133EB0:
    bne @L_80133EE0
    bl fn_800057A8
    cmpwi r3, 0x1
    beq @L_80133ED8
    bge @L_80133EC8
    b @L_80133ED8
    @L_80133EC8:
    cmpwi r3, 0x3
    bge @L_80133ED8
    li r0, 0x115
    b @L_80134144
    @L_80133ED8:
    li r0, 0x2
    b @L_80134144
    @L_80133EE0:
    subi r0, r3, 0x1
    add r3, r3, r4
    srawi r4, r0, 31
    subi r0, r3, 0x1
    andc r3, r0, r4
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80133F30
    bl fn_800057A8
    cmpwi r3, 0x1
    beq @L_80133F24
    bge @L_80133F14
    b @L_80133F24
    @L_80133F14:
    cmpwi r3, 0x3
    bge @L_80133F24
    li r0, 0x115
    b @L_80133F28
    @L_80133F24:
    li r0, 0x2
    @L_80133F28:
    mr r29, r0
    b @L_801340A4
    @L_80133F30:
    lwz r7, 0x4(r3)
    li r4, -0x1
    lwz r6, lbl_80478848(r13)
    lbz r5, 0x94(r3)
    lbz r0, 0x95(r3)
    cmpw r7, r6
    extsb r3, r5
    extsb r0, r0
    add r30, r3, r0
    blt @L_80133F5C
    subf r4, r6, r7
    @L_80133F5C:
    cmpwi r4, 0x0
    bge @L_80133F6C
    li r0, 0x0
    b @L_801340A0
    @L_80133F6C:
    bne @L_80133F9C
    bl fn_800057A8
    cmpwi r3, 0x1
    beq @L_80133F94
    bge @L_80133F84
    b @L_80133F94
    @L_80133F84:
    cmpwi r3, 0x3
    bge @L_80133F94
    li r0, 0x115
    b @L_8013409C
    @L_80133F94:
    li r0, 0x2
    b @L_8013409C
    @L_80133F9C:
    subi r0, r4, 0x1
    srawi r3, r0, 31
    add r0, r6, r0
    andc r3, r0, r3
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80133FE4
    bl fn_800057A8
    cmpwi r3, 0x1
    beq @L_80133FDC
    bge @L_80133FCC
    b @L_80133FDC
    @L_80133FCC:
    cmpwi r3, 0x3
    bge @L_80133FDC
    li r29, 0x115
    b @L_80134054
    @L_80133FDC:
    li r29, 0x2
    b @L_80134054
    @L_80133FE4:
    lbz r4, 0x94(r3)
    lbz r0, 0x95(r3)
    extsb r4, r4
    extsb r0, r0
    add r29, r4, r0
    bl fn_80134258
    cmpwi r3, 0x0
    bge @L_8013400C
    li r29, 0x0
    b @L_80134054
    @L_8013400C:
    bne @L_80134018
    bl fn_80134274
    b @L_80134050
    @L_80134018:
    subi r3, r3, 0x1
    bl fn_80134228
    cmplwi r3, 0x0
    bne @L_80134030
    bl fn_80134274
    b @L_80134048
    @L_80134030:
    lbz r4, 0x94(r3)
    lbz r0, 0x95(r3)
    extsb r4, r4
    extsb r0, r0
    add r4, r4, r0
    bl fn_80133E6C
    @L_80134048:
    bl fn_80134164
    extsh r3, r3
    @L_80134050:
    add r29, r3, r29
    @L_80134054:
    cmpwi r29, 0x0
    ble @L_80134068
    bl fn_80134304
    cmpw r3, r29
    bgt @L_80134070
    @L_80134068:
    li r29, 0x0
    b @L_80134098
    @L_80134070:
    mr r3, r29
    bl fn_801342B8
    mr r29, r3
    extsh. r0, r29
    ble @L_80134094
    bl fn_80134304
    extsh r0, r29
    cmpw r3, r0
    bgt @L_80134098
    @L_80134094:
    li r29, 0x0
    @L_80134098:
    extsh r0, r29
    @L_8013409C:
    add r0, r0, r30
    @L_801340A0:
    mr r29, r0
    @L_801340A4:
    cmpwi r29, 0x0
    ble @L_801340D0
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_801340C0
    li r3, 0x0
    b @L_801340C8
    @L_801340C0:
    mtctr r12
    bctrl
    @L_801340C8:
    cmpw r3, r29
    bgt @L_801340D8
    @L_801340D0:
    li r30, 0x0
    b @L_80134140
    @L_801340D8:
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_801340EC
    li r3, 0x0
    b @L_801340F8
    @L_801340EC:
    mr r3, r29
    mtctr r12
    bctrl
    @L_801340F8:
    cmplwi r3, 0x0
    bne @L_80134108
    li r30, 0x0
    b @L_8013410C
    @L_80134108:
    lha r30, 0x2(r3)
    @L_8013410C:
    extsh. r0, r30
    ble @L_8013413C
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80134128
    li r3, 0x0
    b @L_80134130
    @L_80134128:
    mtctr r12
    bctrl
    @L_80134130:
    extsh r0, r30
    cmpw r3, r0
    bgt @L_80134140
    @L_8013413C:
    li r30, 0x0
    @L_80134140:
    extsh r0, r30
    @L_80134144:
    add r3, r0, r31
    @L_80134148:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
