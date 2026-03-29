    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    lis r4, lbl_803A6498@ha
    lis r3, lbl_802E51C8@ha
    addi r30, r4, lbl_803A6498@l
    li r24, 0x0
    addi r31, r3, lbl_802E51C8@l
    li r25, 0x1
    addi r28, r30, 0x48
    @L_80038A38:
    cmpwi r24, 0x3
    beq @L_80038DD4
    bge @L_80038A5C
    cmpwi r24, 0x1
    beq @L_80038A90
    bge @L_80038B2C
    cmpwi r24, 0x0
    bge @L_80038A74
    b @L_80038DD4
    @L_80038A5C:
    cmpwi r24, 0x64
    beq @L_80038DC0
    bge @L_80038DD4
    cmpwi r24, 0x5
    bge @L_80038DD4
    b @L_80038D04
    @L_80038A74:
    li r0, 0x3c01
    li r3, 0x28
    stw r0, lbl_8047A480(r13)
    li r4, 0x0
    bl fn_8010264C
    li r24, 0x1
    b @L_80038DD4
    @L_80038A90:
    bl fn_80005748
    lwz r0, lbl_8047A498(r13)
    mr r24, r3
    cmplwi r0, 0x0
    beq @L_80038AA8
    mr r24, r0
    @L_80038AA8:
    bl fn_801EF214
    stw r3, lbl_8047A49C(r13)
    bl fn_801EF274
    stw r3, lbl_8047A4A0(r13)
    mr r4, r24
    li r3, 0x4c
    bl fn_80132A38
    bl fn_801EF274
    mr r4, r3
    li r3, 0x2f
    bl fn_80132A38
    bl fn_801EF214
    mr r4, r3
    li r3, 0x30
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x3c
    li r5, 0x118
    li r6, 0x0
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80038B0C
    li r24, 0x2
    b @L_80038DD4
    @L_80038B0C:
    cmpwi r0, 0x1
    bne @L_80038B20
    li r24, 0x64
    li r26, 0x1
    b @L_80038DD4
    @L_80038B20:
    li r24, 0x64
    li r26, 0x1
    b @L_80038DD4
    @L_80038B2C:
    li r27, 0x1
    bl fn_801D036C
    mr r29, r3
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r29, 0x4
    subi r4, r3, 0x4
    mtctr r0
    @L_80038B54:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80038B54
    li r3, 0x3
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    cmpwi r3, 0x3
    bne @L_80038C7C
    li r3, 0x0
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    beq @L_80038C7C
    mr r3, r29
    li r4, 0x2
    bl fn_80129280
    li r4, 0x2
    li r5, 0x0
    bl fn_8012A5B0
    mr r24, r3
    li r3, 0x0
    li r4, 0x2
    li r5, 0x0
    bl fn_8012A5B0
    cmplw r24, r3
    beq @L_80038BDC
    mr r3, r29
    li r4, 0x4
    bl fn_80135168
    cmplwi r3, 0x0
    bne @L_80038C7C
    @L_80038BDC:
    bl fn_80005748
    stw r3, lbl_8047A498(r13)
    bl fn_80005748
    lwz r0, lbl_8047A498(r13)
    mr r24, r3
    cmplwi r0, 0x0
    beq @L_80038BFC
    mr r24, r0
    @L_80038BFC:
    bl fn_801EF214
    stw r3, lbl_8047A49C(r13)
    bl fn_801EF274
    stw r3, lbl_8047A4A0(r13)
    mr r4, r24
    li r3, 0x4c
    bl fn_80132A38
    bl fn_801EF274
    mr r4, r3
    li r3, 0x2f
    bl fn_80132A38
    bl fn_801EF214
    mr r4, r3
    li r3, 0x30
    bl fn_80132A38
    li r0, 0x1b60
    li r3, 0x0
    stw r0, lbl_8047A480(r13)
    li r4, 0x3c
    li r5, 0x118
    li r6, 0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_80038C68
    li r27, 0x0
    b @L_80038C6C
    @L_80038C68:
    li r27, 0x1
    @L_80038C6C:
    bl fn_80005748
    lwz r0, lbl_8047A498(r13)
    subf r0, r0, r3
    stw r0, lbl_8047A498(r13)
    @L_80038C7C:
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r3, 0x4
    subi r4, r29, 0x4
    mtctr r0
    @L_80038C98:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80038C98
    bl fn_80005748
    lwz r4, lbl_8047A498(r13)
    lis r0, 0x4330
    stw r0, 0x18(r1)
    add r0, r4, r3
    lfd f1, lbl_8047BA88(r2)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f1, f0, f1
    bl fn_800056EC
    li r0, 0x0
    mr r3, r29
    stw r0, lbl_8047A498(r13)
    bl fn_801D0314
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_80038CFC
    li r24, 0x4
    li r26, 0x1
    b @L_80038DD4
    @L_80038CFC:
    li r24, 0x0
    b @L_80038DD4
    @L_80038D04:
    li r3, 0x1
    bl fn_8012F11C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80038D20
    li r0, 0x1
    b @L_80038D24
    @L_80038D20:
    li r0, 0x0
    @L_80038D24:
    clrlwi r5, r0, 24
    li r3, 0x0
    li r4, 0x18
    bl fn_8012A450
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    li r3, 0x0
    li r4, 0x1
    bl fn_80129280
    bl fn_80135CD0
    mr r24, r3
    bl fn_80135C28
    stw r3, 0x48(r30)
    mr r3, r24
    bl fn_80135BF8
    stw r3, 0x4(r28)
    mr r3, r24
    bl fn_80135BE0
    stb r3, 0x8(r28)
    addi r3, r1, 0xc
    addi r4, r1, 0x10
    addi r5, r1, 0x8
    bl fn_8011418C
    lwz r4, 0xc(r1)
    mr r3, r24
    bl fn_80135B6C
    lwz r4, 0x10(r1)
    mr r3, r24
    bl fn_80135B4C
    lbz r4, 0x8(r1)
    mr r3, r24
    bl fn_80135B3C
    li r3, 0x4
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    li r24, 0x64
    b @L_80038DD4
    @L_80038DC0:
    li r3, 0x28
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r25, 0x0
    @L_80038DD4:
    cmpwi r25, 0x0
    bne @L_80038A38
    li r0, 0x1b5a
    addi r24, r31, 0x0
    stw r0, lbl_8047A480(r13)
    addi r27, r30, 0xb4
    li r25, 0x0
    @L_80038DF0:
    mr r3, r27
    mr r4, r24
    li r5, 0x18
    bl memcpy
    addi r24, r24, 0x18
    addi r27, r27, 0x18
    addi r25, r25, 0x1
    cmpwi r25, 0x4
    blt @L_80038DF0
    addi r25, r31, 0x60
    addi r24, r30, 0x54
    li r27, 0x0
    @L_80038E20:
    mr r3, r24
    mr r4, r25
    li r5, 0x18
    bl memcpy
    addi r25, r25, 0x18
    addi r24, r24, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80038E20
    lfs f0, 0xe0(r31)
    li r0, 0x0
    stb r0, lbl_8047A47C(r13)
    mr r3, r26
    stfs f0, lbl_8047A488(r13)
    stfs f0, lbl_8047A484(r13)
    stb r0, lbl_8047A47D(r13)
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
