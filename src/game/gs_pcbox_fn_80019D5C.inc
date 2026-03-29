    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r3
    lis r3, lbl_803A1D40@ha
    cmpwi r4, 0x2
    addi r29, r3, lbl_803A1D40@l
    lbz r3, 0x6(r29)
    beq @L_80019D9C
    bge @L_80019DA4
    cmpwi r4, 0x1
    bge @L_80019D94
    b @L_80019DA4
    @L_80019D94:
    subi r31, r3, 0x1
    b @L_80019DAC
    @L_80019D9C:
    addi r31, r3, 0x1
    b @L_80019DAC
    @L_80019DA4:
    mr r3, r30
    b @L_80019F58
    @L_80019DAC:
    extsb r0, r31
    cmpwi r0, 0x6
    blt @L_80019DBC
    li r31, 0x5
    @L_80019DBC:
    extsb r0, r31
    cmpwi r0, 0x0
    bge @L_80019DCC
    li r31, 0x0
    @L_80019DCC:
    extsb r28, r31
    extsb r0, r3
    cmpw r28, r0
    beq @L_80019F54
    lis r3, lbl_803A1D40@ha
    li r27, 0x0
    addi r3, r3, lbl_803A1D40@l
    lwz r4, 0x8(r3)
    lwz r3, 0xc(r3)
    cmpwi r4, 0x1
    beq @L_80019EBC
    bge @L_80019E08
    cmpwi r4, 0x0
    bge @L_80019E14
    b @L_80019F2C
    @L_80019E08:
    cmpwi r4, 0x3
    bge @L_80019F2C
    b @L_80019F1C
    @L_80019E14:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_80019E24
    b @L_80019F44
    @L_80019E24:
    cmpwi r4, 0x1
    beq @L_80019E64
    bge @L_80019E98
    cmpwi r4, 0x0
    bge @L_80019E3C
    b @L_80019E98
    @L_80019E3C:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_80019E5C
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_80019E9C
    @L_80019E5C:
    bl fn_8006AEEC
    b @L_80019E9C
    @L_80019E64:
    cmplwi r3, 0x0
    bne @L_80019E74
    li r3, 0x0
    bl fn_801F2A7C
    @L_80019E74:
    cmplwi r3, 0x0
    bne @L_80019E84
    li r3, 0x0
    b @L_80019E9C
    @L_80019E84:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_80019E9C
    @L_80019E98:
    li r3, 0x0
    @L_80019E9C:
    cmplwi r3, 0x0
    bne @L_80019EAC
    li r27, 0x0
    b @L_80019F44
    @L_80019EAC:
    mr r4, r28
    bl fn_8012AC08
    mr r27, r3
    b @L_80019F2C
    @L_80019EBC:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_80019ECC
    b @L_80019F44
    @L_80019ECC:
    cmplwi r3, 0x0
    bne @L_80019EDC
    li r3, 0x0
    bl fn_801F2A7C
    @L_80019EDC:
    cmplwi r3, 0x0
    bne @L_80019EEC
    li r27, 0x0
    b @L_80019F44
    @L_80019EEC:
    mr r4, r28
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_80019F04
    li r27, 0x0
    b @L_80019F44
    @L_80019F04:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    b @L_80019F2C
    @L_80019F1C:
    clrlwi r0, r28, 16
    cmplwi r0, 0x1e
    blt @L_80019F2C
    b @L_80019F44
    @L_80019F2C:
    mr r3, r27
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80019F44
    li r27, 0x0
    @L_80019F44:
    cmplwi r27, 0x0
    beq @L_80019F54
    mr r30, r27
    stb r31, 0x6(r29)
    @L_80019F54:
    mr r3, r30
    @L_80019F58:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
