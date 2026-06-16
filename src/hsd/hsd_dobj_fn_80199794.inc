    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr. r25, r3
    beq @L_80199A6C
    lis r3, lbl_80465378@ha
    addi r3, r3, lbl_80465378@l
    bl HSD_ObjAlloc
    mr. r22, r3
    bne @L_801997D0
    la r3, -23664(r2)
    li r4, 0x2f3
    la r5, -23656(r2)
    bl __assert
@L_801997D0:
    mr r3, r22
    li r4, 0x0
    li r5, 0x30
    bl memset
    lwz r28, 0x0(r25)
    mr r27, r22
    cmplwi r28, 0x0
    beq @L_80199A18
    lis r3, lbl_80465378@ha
    addi r3, r3, lbl_80465378@l
    bl HSD_ObjAlloc
    mr. r31, r3
    bne @L_80199814
    la r3, -23664(r2)
    li r4, 0x2f3
    la r5, -23656(r2)
    bl __assert
@L_80199814:
    mr r3, r31
    li r4, 0x0
    li r5, 0x30
    bl memset
    lwz r30, 0x0(r28)
    cmplwi r30, 0x0
    beq @L_801999C8
    lis r3, lbl_80465378@ha
    addi r3, r3, lbl_80465378@l
    bl HSD_ObjAlloc
    mr. r26, r3
    bne @L_80199854
    la r3, -23664(r2)
    li r4, 0x2f3
    la r5, -23656(r2)
    bl __assert
@L_80199854:
    mr r3, r26
    li r4, 0x0
    li r5, 0x30
    bl memset
    lwz r29, 0x0(r30)
    cmplwi r29, 0x0
    beq @L_80199978
    lis r3, lbl_80465378@ha
    addi r3, r3, lbl_80465378@l
    bl HSD_ObjAlloc
    lis r5, lbl_80465378@ha
    lis r4, lbl_80274758@ha
    addi r0, r5, lbl_80465378@l
    mr r22, r3
    addi r4, r4, lbl_80274758@l
    mr r3, r0
    bl fn_80199A84
    cmplwi r22, 0x0
    bne @L_801998B0
    la r3, -23664(r2)
    li r4, 0x2f3
    la r5, -23656(r2)
    bl __assert
@L_801998B0:
    mr r3, r22
    li r4, 0x0
    li r5, 0x30
    bl memset
    lwz r23, 0x0(r29)
    cmplwi r23, 0x0
    beq @L_80199928
    bl HSD_FObjAlloc
    mr r24, r3
    lwz r3, 0x0(r23)
    bl fn_80199794
    stw r3, 0x0(r24)
    li r0, 0x0
    lfs f0, 0x8(r23)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    sth r3, 0x18(r24)
    lbz r3, 0xc(r23)
    stb r3, 0x13(r24)
    lbz r3, 0xd(r23)
    stb r3, 0x14(r24)
    lbz r3, 0xe(r23)
    stb r3, 0x15(r24)
    lwz r3, 0x10(r23)
    stw r3, 0x8(r24)
    lwz r3, 0x4(r23)
    stw r3, 0xc(r24)
    stb r0, 0x10(r24)
    b @L_8019992C
@L_80199928:
    li r24, 0x0
@L_8019992C:
    stw r24, 0x0(r22)
    li r0, 0x0
    lfs f0, 0x8(r29)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    sth r3, 0x18(r22)
    lbz r3, 0xc(r29)
    stb r3, 0x13(r22)
    lbz r3, 0xd(r29)
    stb r3, 0x14(r22)
    lbz r3, 0xe(r29)
    stb r3, 0x15(r22)
    lwz r3, 0x10(r29)
    stw r3, 0x8(r22)
    lwz r3, 0x4(r29)
    stw r3, 0xc(r22)
    stb r0, 0x10(r22)
    b @L_8019997C
@L_80199978:
    li r22, 0x0
@L_8019997C:
    stw r22, 0x0(r26)
    li r0, 0x0
    lfs f0, 0x8(r30)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    sth r3, 0x18(r26)
    lbz r3, 0xc(r30)
    stb r3, 0x13(r26)
    lbz r3, 0xd(r30)
    stb r3, 0x14(r26)
    lbz r3, 0xe(r30)
    stb r3, 0x15(r26)
    lwz r3, 0x10(r30)
    stw r3, 0x8(r26)
    lwz r3, 0x4(r30)
    stw r3, 0xc(r26)
    stb r0, 0x10(r26)
    b @L_801999CC
@L_801999C8:
    li r26, 0x0
@L_801999CC:
    stw r26, 0x0(r31)
    li r0, 0x0
    lfs f0, 0x8(r28)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r3, 0xc(r1)
    sth r3, 0x18(r31)
    lbz r3, 0xc(r28)
    stb r3, 0x13(r31)
    lbz r3, 0xd(r28)
    stb r3, 0x14(r31)
    lbz r3, 0xe(r28)
    stb r3, 0x15(r31)
    lwz r3, 0x10(r28)
    stw r3, 0x8(r31)
    lwz r3, 0x4(r28)
    stw r3, 0xc(r31)
    stb r0, 0x10(r31)
    b @L_80199A1C
@L_80199A18:
    li r31, 0x0
@L_80199A1C:
    stw r31, 0x0(r27)
    li r0, 0x0
    mr r3, r27
    lfs f0, 0x8(r25)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r4, 0xc(r1)
    sth r4, 0x18(r27)
    lbz r4, 0xc(r25)
    stb r4, 0x13(r27)
    lbz r4, 0xd(r25)
    stb r4, 0x14(r27)
    lbz r4, 0xe(r25)
    stb r4, 0x15(r27)
    lwz r4, 0x10(r25)
    stw r4, 0x8(r27)
    lwz r4, 0x4(r25)
    stw r4, 0xc(r27)
    stb r0, 0x10(r27)
    b @L_80199A70
@L_80199A6C:
    li r3, 0x0
@L_80199A70:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
