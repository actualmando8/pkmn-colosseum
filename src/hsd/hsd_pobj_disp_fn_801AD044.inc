    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r5, lbl_80274EE0@ha
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r3
    addi r31, r5, lbl_80274EE0@l
    mr r30, r4
    b @L_801AD1F0
@L_801AD068:
    cmplwi r29, 0x0
    beq @L_801AD1E8
    cmplwi r30, 0x0
    beq @L_801AD1E8
    lhz r0, 0xc(r29)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x2000
    beq @L_801AD098
    bge @L_801AD1E8
    cmpwi r0, 0x0
    beq @L_801AD168
    b @L_801AD1E8
@L_801AD098:
    lwz r26, 0x14(r29)
    lwz r25, 0x14(r30)
    cmplwi r26, 0x0
    beq @L_801AD1E8
    cmplwi r25, 0x0
    bne @L_801AD150
    b @L_801AD1E8
@L_801AD0B4:
    lwz r27, 0x4(r26)
    lwz r28, 0x0(r25)
    b @L_801AD134
@L_801AD0C0:
    lwz r3, 0x4(r27)
    bl fn_801A053C
    lwz r4, 0x0(r28)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x4(r27)
    lwz r0, 0x4(r27)
    cmplwi r0, 0x0
    bne @L_801AD0F8
    addi r5, r31, 0x1f0
    addi r3, r2, -23016
    li r4, 0x2ca
    bl fn_80196E10
@L_801AD0F8:
    lwz r4, 0x4(r27)
    cmplwi r4, 0x0
    beq @L_801AD12C
    lhz r3, 0x6(r4)
    addi r0, r3, 0x1
    sth r0, 0x6(r4)
    lhz r0, 0x6(r4)
    cmplwi r0, 0x0
    bne @L_801AD12C
    addi r3, r31, 0x1fc
    addi r5, r31, 0x208
    li r4, 0x9e
    bl fn_80196E10
@L_801AD12C:
    lwz r27, 0x0(r27)
    addi r28, r28, 0x8
@L_801AD134:
    cmplwi r27, 0x0
    beq @L_801AD148
    lwz r0, 0x0(r28)
    cmplwi r0, 0x0
    bne @L_801AD0C0
@L_801AD148:
    lwz r26, 0x0(r26)
    addi r25, r25, 0x4
@L_801AD150:
    cmplwi r26, 0x0
    beq @L_801AD1E8
    lwz r0, 0x0(r25)
    cmplwi r0, 0x0
    bne @L_801AD0B4
    b @L_801AD1E8
@L_801AD168:
    lwz r3, 0x14(r29)
    bl fn_801A053C
    li r0, 0x0
    stw r0, 0x14(r29)
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_801AD1E8
    lwz r4, 0x14(r30)
    li r3, 0x0
    li r5, 0x0
    bl fn_8019C128
    stw r3, 0x14(r29)
    lwz r0, 0x14(r29)
    cmplwi r0, 0x0
    bne @L_801AD1B4
    addi r5, r31, 0x230
    addi r3, r2, -23016
    li r4, 0x2e5
    bl fn_80196E10
@L_801AD1B4:
    lwz r4, 0x14(r29)
    cmplwi r4, 0x0
    beq @L_801AD1E8
    lhz r3, 0x6(r4)
    addi r0, r3, 0x1
    sth r0, 0x6(r4)
    lhz r0, 0x6(r4)
    cmplwi r0, 0x0
    bne @L_801AD1E8
    addi r3, r31, 0x1fc
    addi r5, r31, 0x208
    li r4, 0x9e
    bl fn_80196E10
@L_801AD1E8:
    lwz r29, 0x4(r29)
    lwz r30, 0x4(r30)
@L_801AD1F0:
    cmplwi r29, 0x0
    beq @L_801AD200
    cmplwi r30, 0x0
    bne @L_801AD068
@L_801AD200:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
