    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    stw r30, 0x8(r1)
    beq @L_801A4C34
    b @L_801A4C28
    @L_801A4B20:
    cmplwi r31, 0x0
    beq @L_801A4C24
    la r30, lbl_8047B2B4(r13)
    b @L_801A4C18
    @L_801A4B30:
    lwz r3, 0x0(r30)
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_801A4C14
    li r6, 0x0
    b @L_801A4B80
    @L_801A4B48:
    lis r3, lbl_804655E0@ha
    slwi r4, r6, 2
    addi r0, r3, lbl_804655E0@l
    add r3, r0, r4
    lwz r0, 0x0(r3)
    cmplw r31, r0
    bne @L_801A4B7C
    lis r3, lbl_804655E0@ha
    slwi r4, r6, 2
    addi r0, r3, lbl_804655E0@l
    li r5, 0x0
    add r3, r0, r4
    stw r5, 0x0(r3)
    @L_801A4B7C:
    addi r6, r6, 0x1
    @L_801A4B80:
    cmpwi r6, 0x9
    blt @L_801A4B48
    lwz r3, 0x0(r30)
    bl fn_801A3E64
    cmplwi r31, 0x0
    stw r3, 0x0(r30)
    beq @L_801A4C24
    lis r3, 0x1
    lhz r4, 0x4(r31)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A4BC0
    b @L_801A4BD8
    @L_801A4BC0:
    lhz r0, 0x4(r31)
    lhz r3, 0x4(r31)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r31)
    srwi r3, r4, 5
    @L_801A4BD8:
    cmpwi r3, 0x0
    beq @L_801A4C24
    cmplwi r31, 0x0
    beq @L_801A4C24
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_801A4C24
    @L_801A4C14:
    lwz r30, 0x0(r30)
    @L_801A4C18:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    bne @L_801A4B30
    @L_801A4C24:
    lwz r31, 0xc(r31)
    @L_801A4C28:
    cmplwi r31, 0x0
    bne @L_801A4B20
    b @L_801A4D08
    @L_801A4C34:
    li r6, 0x0
    b @L_801A4C58
    @L_801A4C3C:
    lis r3, lbl_804655E0@ha
    slwi r4, r6, 2
    addi r0, r3, lbl_804655E0@l
    li r5, 0x0
    add r3, r0, r4
    addi r6, r6, 0x1
    stw r5, 0x0(r3)
    @L_801A4C58:
    cmpwi r6, 0x9
    blt @L_801A4C3C
    li r0, 0x0
    stw r0, lbl_8047B2B8(r13)
    b @L_801A4CFC
    @L_801A4C6C:
    lwz r3, lbl_8047B2B4(r13)
    lwz r30, 0x4(r3)
    cmplwi r30, 0x0
    beq @L_801A4CF0
    lis r3, 0x1
    lhz r4, 0x4(r30)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A4CA0
    b @L_801A4CB8
    @L_801A4CA0:
    lhz r0, 0x4(r30)
    lhz r3, 0x4(r30)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r30)
    srwi r3, r4, 5
    @L_801A4CB8:
    cmpwi r3, 0x0
    beq @L_801A4CF0
    cmplwi r30, 0x0
    beq @L_801A4CF0
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r30)
    mr r3, r30
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A4CF0:
    lwz r3, lbl_8047B2B4(r13)
    bl fn_801A3E64
    stw r3, lbl_8047B2B4(r13)
    @L_801A4CFC:
    lwz r0, lbl_8047B2B4(r13)
    cmplwi r0, 0x0
    bne @L_801A4C6C
    @L_801A4D08:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
