    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lis r4, lbl_803A1D40@ha
    li r31, 0x0
    addi r4, r4, lbl_803A1D40@l
    lwz r5, 0x8(r4)
    lwz r4, 0xc(r4)
    cmpwi r5, 0x1
    beq @L_80019C48
    bge @L_80019B88
    cmpwi r5, 0x0
    bge @L_80019B94
    b @L_80019CC8
    @L_80019B88:
    cmpwi r5, 0x3
    bge @L_80019CC8
    b @L_80019CB4
    @L_80019B94:
    extsb r30, r3
    clrlwi r0, r30, 16
    cmplwi r0, 0x6
    blt @L_80019BA8
    b @L_80019CE0
    @L_80019BA8:
    cmpwi r5, 0x1
    beq @L_80019BE8
    bge @L_80019C24
    cmpwi r5, 0x0
    bge @L_80019BC0
    b @L_80019C24
    @L_80019BC0:
    li r3, 0x8ae
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_80019BE0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    b @L_80019C28
    @L_80019BE0:
    bl fn_8006AEEC
    b @L_80019C28
    @L_80019BE8:
    cmplwi r4, 0x0
    bne @L_80019BFC
    li r3, 0x0
    bl fn_801F2A7C
    mr r4, r3
    @L_80019BFC:
    cmplwi r4, 0x0
    bne @L_80019C0C
    li r3, 0x0
    b @L_80019C28
    @L_80019C0C:
    mr r3, r4
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    b @L_80019C28
    @L_80019C24:
    li r3, 0x0
    @L_80019C28:
    cmplwi r3, 0x0
    bne @L_80019C38
    li r31, 0x0
    b @L_80019CE0
    @L_80019C38:
    mr r4, r30
    bl fn_8012AC08
    mr r31, r3
    b @L_80019CC8
    @L_80019C48:
    extsb r30, r3
    clrlwi r0, r30, 16
    cmplwi r0, 0x6
    blt @L_80019C5C
    b @L_80019CE0
    @L_80019C5C:
    cmplwi r4, 0x0
    bne @L_80019C70
    li r3, 0x0
    bl fn_801F2A7C
    mr r4, r3
    @L_80019C70:
    cmplwi r4, 0x0
    bne @L_80019C80
    li r31, 0x0
    b @L_80019CE0
    @L_80019C80:
    mr r3, r4
    mr r4, r30
    bl fn_801F986C
    cmplwi r3, 0x0
    bne @L_80019C9C
    li r31, 0x0
    b @L_80019CE0
    @L_80019C9C:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    b @L_80019CC8
    @L_80019CB4:
    extsb r0, r3
    clrlwi r0, r0, 16
    cmplwi r0, 0x1e
    blt @L_80019CC8
    b @L_80019CE0
    @L_80019CC8:
    mr r3, r31
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80019CE0
    li r31, 0x0
    @L_80019CE0:
    cmplwi r31, 0x0
    bne @L_80019CF0
    li r3, -0x1
    b @L_80019D44
    @L_80019CF0:
    lis r4, lbl_803A1BD8@ha
    mr r3, r31
    addi r4, r4, lbl_803A1BD8@l
    bl fn_801204A8
    lis r4, lbl_803A1BD8@ha
    li r3, 0x42
    addi r9, r4, lbl_803A1BD8@l
    li r5, 0x0
    li r4, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r0, r3
    li r3, 0x42
    mr r30, r0
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r30
    @L_80019D44:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
