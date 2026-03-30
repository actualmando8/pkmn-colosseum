    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    b @L_8018DBCC
    @L_8018DB94:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DBC8
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_8018DBC8
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_8018DBC8
    lwz r31, 0x4(r3)
    b @L_8018DC38
    @L_8018DBC8:
    addi r31, r31, 0x1
    @L_8018DBCC:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_8018DB94
    li r30, 0x0
    b @L_8018DC28
    @L_8018DBE0:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DC24
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_8018DC24
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_8018DC38
    @L_8018DC24:
    addi r30, r30, 0x1
    @L_8018DC28:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018DBE0
    li r31, 0x0
    @L_8018DC38:
    li r30, 0x0
    b @L_8018DC68
    @L_8018DC40:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DC64
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_8018DC64
    b @L_8018DC78
    @L_8018DC64:
    addi r30, r30, 0x1
    @L_8018DC68:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018DC40
    li r3, 0x0
    @L_8018DC78:
    cmplwi r3, 0x0
    beq @L_8018DC88
    li r4, 0x1
    bl fn_8018DCA8
    @L_8018DC88:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
