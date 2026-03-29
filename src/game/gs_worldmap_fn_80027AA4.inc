    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    lwz r4, 0x28(r28)
    li r30, 0x0
    lwz r3, 0x2c(r28)
    li r29, 0x0
    lwz r27, 0x0(r4)
    lwz r5, 0x0(r3)
    cmpwi r27, 0xf
    bge @L_80027BA0
    lwz r3, 0x24(r28)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    blt @L_80027AF0
    cmpwi r0, 0x2
    blt @L_80027AF8
    @L_80027AF0:
    li r3, 0x0
    b @L_80027B5C
    @L_80027AF8:
    cmpwi r5, 0x0
    blt @L_80027B08
    cmpwi r5, 0x4
    blt @L_80027B10
    @L_80027B08:
    li r3, 0x0
    b @L_80027B5C
    @L_80027B10:
    mulli r3, r0, 0x18
    lis r4, lbl_80266E18@ha
    slwi r0, r5, 2
    addi r4, r4, lbl_80266E18@l
    add r3, r4, r3
    add r3, r3, r0
    lwz r31, 0x8(r3)
    mr r3, r31
    bl fn_800FA314
    cmpwi r27, 0x0
    blt @L_80027B44
    cmpw r27, r3
    blt @L_80027B4C
    @L_80027B44:
    li r3, 0x0
    b @L_80027B5C
    @L_80027B4C:
    mr r3, r31
    bl fn_800FA280
    slwi r0, r27, 1
    lhzx r3, r3, r0
    @L_80027B5C:
    clrlwi r0, r3, 16
    mr r31, r3
    cmplwi r0, 0x0
    bne @L_80027B78
    li r3, 0x2ef9
    bl fn_800FA280
    lhz r31, 0x0(r3)
    @L_80027B78:
    li r3, 0x2efc
    bl fn_800FA280
    lhz r0, 0x0(r3)
    clrlwi r3, r31, 16
    cmplw r3, r0
    bne @L_80027B98
    li r0, 0x0
    b @L_80027BCC
    @L_80027B98:
    li r0, 0x6
    b @L_80027BCC
    @L_80027BA0:
    cmpwi r5, 0x3
    beq @L_80027BC0
    bge @L_80027BC8
    cmpwi r5, 0x0
    beq @L_80027BB8
    b @L_80027BC8
    @L_80027BB8:
    li r0, 0x3
    b @L_80027BCC
    @L_80027BC0:
    li r0, 0x5
    b @L_80027BCC
    @L_80027BC8:
    li r0, 0x4
    @L_80027BCC:
    cmpwi r0, 0x3
    beq @L_80027C9C
    bge @L_80027BE8
    cmpwi r0, 0x0
    beq @L_80027CC0
    bge @L_80027BF8
    b @L_80027CCC
    @L_80027BE8:
    cmpwi r0, 0x5
    beq @L_80027C94
    bge @L_80027CCC
    b @L_80027C48
    @L_80027BF8:
    cmpwi r0, 0x1
    bne @L_80027C08
    li r4, 0x1
    b @L_80027C0C
    @L_80027C08:
    li r4, 0x1
    @L_80027C0C:
    lwz r3, 0x34(r28)
    lwz r3, 0x0(r3)
    subi r0, r3, 0x1
    cmpwi r0, 0x0
    blt @L_80027C40
    lwz r29, 0x18(r28)
    slwi r28, r0, 1
    lhzx r3, r29, r28
    bl fn_80027960
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80027C40
    sthx r3, r29, r28
    @L_80027C40:
    li r29, 0x24
    b @L_80027D30
    @L_80027C48:
    lwz r3, 0x34(r28)
    lwz r3, 0x0(r3)
    cmpwi r3, 0x0
    bgt @L_80027C60
    li r0, 0x0
    b @L_80027C80
    @L_80027C60:
    subi r4, r3, 0x1
    lwz r5, 0x18(r28)
    slwi r3, r4, 1
    li r0, 0x0
    sthx r0, r5, r3
    li r0, 0x1
    lwz r3, 0x34(r28)
    stw r4, 0x0(r3)
    @L_80027C80:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_80027D30
    li r29, 0x25
    b @L_80027D30
    @L_80027C94:
    li r30, 0x1
    b @L_80027D30
    @L_80027C9C:
    lwz r4, 0x24(r28)
    lwz r3, 0x0(r4)
    addi r3, r3, 0x1
    cmpwi r3, 0x2
    blt @L_80027CB4
    li r3, 0x0
    @L_80027CB4:
    stw r3, 0x0(r4)
    li r29, 0x27
    b @L_80027D30
    @L_80027CC0:
    li r3, 0x2ef9
    bl fn_800FA280
    lhz r31, 0x0(r3)
    @L_80027CCC:
    lwz r0, 0x1c(r28)
    lis r4, lbl_80266DD8@ha
    lwz r3, 0x34(r28)
    addi r4, r4, lbl_80266DD8@l
    slwi r0, r0, 4
    add r4, r4, r0
    lwz r5, 0x0(r3)
    lwz r6, 0x4(r4)
    cmpw r5, r6
    blt @L_80027CF8
    subi r5, r6, 0x1
    @L_80027CF8:
    lwz r4, 0x18(r28)
    slwi r3, r5, 1
    addi r5, r5, 0x1
    addi r0, r6, 0x1
    add r4, r4, r3
    li r3, 0x0
    sth r31, 0x0(r4)
    cmpw r5, r0
    sth r3, 0x2(r4)
    blt @L_80027D24
    mr r5, r6
    @L_80027D24:
    lwz r3, 0x34(r28)
    li r29, 0x24
    stw r5, 0x0(r3)
    @L_80027D30:
    cmplwi r29, 0x0
    beq @L_80027D40
    mr r3, r29
    bl fn_80166A28
    @L_80027D40:
    mr r3, r30
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
