    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr r22, r3
    li r25, 0x1
    li r3, 0x1e
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    lis r3, lbl_803A9F08@ha
    lis r5, 0x1
    addi r3, r3, lbl_803A9F08@l
    mr r4, r22
    addi r3, r3, 0x150
    subi r5, r5, 0x33d4
    bl memcpy
    bl fn_8025DA88
    mr r30, r3
    bl fn_8025DA3C
    lis r4, lbl_803A9F08@ha
    li r0, -0x1
    addi r27, r4, lbl_803A9F08@l
    lfs f0, lbl_8047BFE8(r2)
    li r4, 0x1
    li r29, 0x0
    addis r26, r27, 0x1
    mr r31, r3
    stb r4, -0x31a8(r26)
    mr r28, r27
    stw r0, -0x31a4(r26)
    stfs f0, -0x31b4(r26)
    stw r29, -0x31b0(r26)
    stw r29, 0x2c(r27)
    stw r29, -0x3280(r26)
    stw r29, 0x0(r27)
    stw r29, 0xc(r27)
    b @L_80063E38
@L_80063DAC:
    mr r3, r29
    bl fn_8006B09C
    mr r24, r3
    bl fn_8006A814
    mr r22, r3
    mr r3, r29
    bl fn_8006B0F8
    li r0, 0x0
    mr r4, r3
    stb r0, 0x4(r28)
    lwz r0, 0x4(r24)
    cmpwi r0, 0x0
    beq @L_80063E30
    mr r3, r22
    bl fn_8008AB4C
    cmpwi r30, 0x1
    bne @L_80063DF8
    li r24, 0x2
    b @L_80063DFC
@L_80063DF8:
    li r24, 0x1
@L_80063DFC:
    bl fn_8006B1D4
    clrlwi r23, r3, 16
    mr r3, r29
    bl fn_8025D89C
    clrlwi r0, r3, 16
    mr r3, r22
    cmplw r0, r23
    bge @L_80063E20
    b @L_80063E24
@L_80063E20:
    mr r0, r23
@L_80063E24:
    mr r5, r24
    clrlwi r4, r0, 16
    bl fn_8008AB20
@L_80063E30:
    addi r28, r28, 0x1
    addi r29, r29, 0x1
@L_80063E38:
    cmpw r29, r31
    blt @L_80063DAC
    li r22, 0x0
@L_80063E44:
    mr r3, r22
    bl fn_8025D744
    addi r22, r22, 0x1
    cmpwi r22, 0x4
    blt @L_80063E44
    mr r29, r27
    addi r24, r1, 0x8
    li r23, 0x0
    li r28, -0x1
@L_80063E68:
    mr r3, r23
    bl fn_8006ACCC
    stw r3, 0x0(r24)
    lwz r3, 0x0(r24)
    cmplwi r3, 0x0
    beq @L_80063E90
    lwz r0, 0x28(r3)
    extsb r0, r0
    stb r0, 0x8(r29)
    b @L_80063E94
@L_80063E90:
    stb r28, 0x8(r29)
@L_80063E94:
    addi r24, r24, 0x4
    addi r29, r29, 0x1
    addi r23, r23, 0x1
    cmpwi r23, 0x4
    blt @L_80063E68
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80063ED0
    bge @L_80063EDC
    cmpwi r3, 0x0
    bge @L_80063EC4
    b @L_80063EDC
@L_80063EC4:
    li r0, 0x136
    stw r0, -0x3280(r26)
    b @L_80063EE4
@L_80063ED0:
    li r0, 0x0
    stw r0, -0x3280(r26)
    b @L_80063EE4
@L_80063EDC:
    li r0, 0x0
    stw r0, -0x3280(r26)
@L_80063EE4:
    lis r3, lbl_803A9F08@ha
    addi r31, r3, lbl_803A9F08@l
@L_80063EEC:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    beq @L_80063FF0
    bge @L_80063F0C
    cmpwi r0, 0x0
    beq @L_80063F18
    bge @L_80063F84
    b @L_80064070
@L_80063F0C:
    cmpwi r0, 0x4
    bge @L_80064070
    b @L_8006404C
@L_80063F18:
    li r3, 0x0
    bl fn_80103CC0
    li r3, 0xc6
    li r4, 0x1
    bl fn_8010264C
    mr r28, r3
    li r3, 0x1
    bl fn_80103CC0
    cmpwi r28, 0x0
    bne @L_80063F54
    lis r3, lbl_803A9F08@ha
    li r0, 0x2
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_80063F54:
    cmpwi r28, 0x1
    bne @L_80063F70
    lis r3, lbl_803A9F08@ha
    li r0, 0x1
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_80063F70:
    lis r3, lbl_803A9F08@ha
    li r0, 0x3
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_80063F84:
    li r3, 0xc5
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    bne @L_80063FC4
    li r0, 0x1
    li r3, 0xc5
    stb r0, 0x4(r31)
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A9F08@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_80063FC4:
    li r3, 0x0
    bl fn_8025D744
    li r3, 0xc5
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A9F08@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_80063FF0:
    li r3, 0xc7
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    blt @L_80064028
    li r3, 0xc7
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A9F08@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_80064028:
    li r3, 0xc7
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A9F08@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x0(r3)
    b @L_80064070
@L_8006404C:
    li r25, 0x0
    li r3, 0xc6
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0xdf
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
@L_80064070:
    cmpwi r25, 0x0
    bne @L_80063EEC
    lbz r0, -0x31a8(r26)
    cmplwi r0, 0x0
    bne @L_8006422C
    bl fn_8025DA88
    cmpwi r3, 0x2
    bne @L_800640B4
    li r3, 0x0
    bl fn_8025D9F0
    clrlwi r0, r3, 16
    cmpwi r0, 0x0
    beq @L_800640AC
    li r0, 0x1
    b @L_800640B8
@L_800640AC:
    li r0, 0x0
    b @L_800640B8
@L_800640B4:
    li r0, 0x0
@L_800640B8:
    clrlwi r0, r0, 16
    cmplwi r0, 0x0
    bne @L_800640E8
    lwz r4, -0x31a4(r26)
    li r3, 0x30
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x44dc
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    b @L_80064160
@L_800640E8:
    li r3, 0x2
    li r4, 0x44e7
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    li r24, 0x1
@L_80064100:
    li r3, 0x1
    bl fn_800F7EF8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80064134
    li r3, 0x1
    bl fn_800F7C28
    cmpwi r3, 0x0
    bne @L_8006412C
    li r0, 0x1
    b @L_80064138
@L_8006412C:
    li r0, 0x0
    b @L_80064138
@L_80064134:
    li r0, 0x0
@L_80064138:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8006414C
    li r24, 0x0
    b @L_80064150
@L_8006414C:
    bl fn_800F0308
@L_80064150:
    cmpwi r24, 0x0
    bne @L_80064100
    li r3, 0x1
    bl fn_801069FC
@L_80064160:
    li r0, 0x0
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x2c(r27)
    addis r3, r3, 0x1
    stb r0, -0x327c(r3)
    li r0, 0x2
    mtctr r0
@L_80064180:
    addi r3, r27, 0x30
    li r0, 0x0
    stb r0, 0x0(r3)
    addi r8, r3, 0xc
    addi r7, r3, 0x18
    addi r6, r3, 0x24
    stw r0, 0x4(r3)
    addi r5, r3, 0x30
    addi r4, r3, 0x3c
    addi r27, r27, 0x48
    stb r0, 0x0(r8)
    addi r3, r27, 0x30
    addi r27, r27, 0x48
    stw r0, 0x4(r8)
    addi r8, r3, 0xc
    stb r0, 0x0(r7)
    stw r0, 0x4(r7)
    addi r7, r3, 0x18
    stb r0, 0x0(r6)
    stw r0, 0x4(r6)
    addi r6, r3, 0x24
    stb r0, 0x0(r5)
    stw r0, 0x4(r5)
    addi r5, r3, 0x30
    stb r0, 0x0(r4)
    stw r0, 0x4(r4)
    addi r4, r3, 0x3c
    stb r0, 0x0(r3)
    stw r0, 0x4(r3)
    stb r0, 0x0(r8)
    stw r0, 0x4(r8)
    stb r0, 0x0(r7)
    stw r0, 0x4(r7)
    stb r0, 0x0(r6)
    stw r0, 0x4(r6)
    stb r0, 0x0(r5)
    stw r0, 0x4(r5)
    stb r0, 0x0(r4)
    stw r0, 0x4(r4)
    bdnz @L_80064180
    bl fn_80062834
    li r3, 0xb3
    b @L_80064364
@L_8006422C:
    bl fn_8025DA88
    mr r25, r3
    bl fn_8025D9CC
    cmpwi r3, 0x4
    bne @L_80064264
    cmpwi r25, 0x2
    bge @L_8006425C
    cmpwi r25, 0x0
    bge @L_80064254
    b @L_8006425C
@L_80064254:
    li r23, 0x2
    b @L_80064284
@L_8006425C:
    li r23, 0x4
    b @L_80064284
@L_80064264:
    cmpwi r25, 0x2
    bge @L_80064280
    cmpwi r25, 0x0
    bge @L_80064278
    b @L_80064280
@L_80064278:
    li r23, 0x2
    b @L_80064284
@L_80064280:
    li r23, 0x1
@L_80064284:
    li r22, 0x0
    b @L_80064298
@L_8006428C:
    mr r3, r22
    bl fn_8025D3F4
    addi r22, r22, 0x1
@L_80064298:
    cmpw r22, r23
    blt @L_8006428C
    li r0, 0x0
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    stw r0, 0x2c(r27)
    addis r3, r3, 0x1
    stb r0, -0x327c(r3)
    li r0, 0x2
    mtctr r0
@L_800642C0:
    addi r3, r27, 0x30
    li r0, 0x0
    stb r0, 0x0(r3)
    addi r8, r3, 0xc
    addi r7, r3, 0x18
    addi r6, r3, 0x24
    stw r0, 0x4(r3)
    addi r5, r3, 0x30
    addi r4, r3, 0x3c
    addi r27, r27, 0x48
    stb r0, 0x0(r8)
    addi r3, r27, 0x30
    addi r27, r27, 0x48
    stw r0, 0x4(r8)
    addi r8, r3, 0xc
    stb r0, 0x0(r7)
    stw r0, 0x4(r7)
    addi r7, r3, 0x18
    stb r0, 0x0(r6)
    stw r0, 0x4(r6)
    addi r6, r3, 0x24
    stb r0, 0x0(r5)
    stw r0, 0x4(r5)
    addi r5, r3, 0x30
    stb r0, 0x0(r4)
    stw r0, 0x4(r4)
    addi r4, r3, 0x3c
    stb r0, 0x0(r3)
    stw r0, 0x4(r3)
    stb r0, 0x0(r8)
    stw r0, 0x4(r8)
    stb r0, 0x0(r7)
    stw r0, 0x4(r7)
    stb r0, 0x0(r6)
    stw r0, 0x4(r6)
    stb r0, 0x0(r5)
    stw r0, 0x4(r5)
    stb r0, 0x0(r4)
    stw r0, 0x4(r4)
    bdnz @L_800642C0
    li r3, 0xb8
@L_80064364:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
