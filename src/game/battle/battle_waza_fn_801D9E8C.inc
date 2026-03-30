    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr. r30, r3
    beq @L_801D9FFC
    lis r3, -32698
    addi r3, r3, 31936
    lwz r3, 0xc(r3)
    cmplwi r3, 0x0
    beq @L_801D9FFC
    lwz r0, 0x84(r30)
    cmplwi r0, 0x0
    bne @L_801D9FFC
    li r4, 0x0
    li r5, 0x0
    bl fn_801190DC
    stw r3, 0x84(r30)
    lwz r3, 0x84(r30)
    cmplwi r3, 0x0
    beq @L_801D9FFC
    lbz r0, 0x18(r30)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    beq @L_801D9EFC
    li r4, 0x0
    bl fn_80118C88
@L_801D9EFC:
    lhz r0, 0x32(r30)
    lwz r4, 0x2c(r30)
    mulli r3, r0, 0xd4
    addi r0, r3, 0x50
    lwzx r4, r4, r0
    cmpwi r4, 0x0
    ble @L_801D9F24
    lwz r3, 0x24(r30)
    bl fn_800EE150
    b @L_801D9F28
@L_801D9F24:
    li r3, 0x0
@L_801D9F28:
    cmplwi r3, 0x0
    mr r31, r3
    beq @L_801D9FFC
    cmplwi r30, 0x0
    beq @L_801D9F44
    lwz r0, 0x10(r30)
    b @L_801D9F48
@L_801D9F44:
    li r0, 0x0
@L_801D9F48:
    cmpwi r0, 0x1
    beq @L_801D9F8C
    bge @L_801D9F6C
    cmpwi r0, -0x1
    beq @L_801D9F84
    bge @L_801D9FA4
    cmpwi r0, -0x2
    bge @L_801D9F7C
    b @L_801D9FA4
@L_801D9F6C:
    cmpwi r0, 0x3
    beq @L_801D9F9C
    bge @L_801D9FA4
    b @L_801D9F94
@L_801D9F7C:
    lfs f1, -21272(r2)
    b @L_801D9FA8
@L_801D9F84:
    lfs f1, -21268(r2)
    b @L_801D9FA8
@L_801D9F8C:
    lfs f1, -21264(r2)
    b @L_801D9FA8
@L_801D9F94:
    lfs f1, -21260(r2)
    b @L_801D9FA8
@L_801D9F9C:
    lfs f1, -21256(r2)
    b @L_801D9FA8
@L_801D9FA4:
    lfs f1, -21252(r2)
@L_801D9FA8:
    fmr f2, f1
    addi r3, r1, 0x8
    fmr f3, f1
    bl fn_800E01F4
    lwz r3, 0x84(r30)
    mr r4, r31
    li r5, 0x4
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_80118FB0
    lwz r3, 0x84(r30)
    li r4, 0x1
    bl fn_80118D18
    lwz r3, 0x84(r30)
    addi r4, r1, 0x8
    li r5, 0x1
    li r6, 0x0
    bl fn_80118DE0
    mr r3, r31
    bl fn_800EE828
@L_801D9FFC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
