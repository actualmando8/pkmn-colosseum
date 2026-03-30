    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r24, 0x60(r1)
    mr. r24, r3
    li r31, 0x0
    li r30, 0x0
    li r29, 0x10
    stw r24, lbl_8047B37C(r13)
    beq @L_801BC328
    mr r3, r24
    bl fn_801BC33C
    cmpwi r3, 0x0
    ble @L_801BC320
    subi r0, r3, 0x1
    cmplwi r0, 0x7
    bgt @L_801BC080
    lis r3, jumptable_8036D5D0@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8036D5D0@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r3, 0x0
    b @L_801BC094
    li r3, 0x1
    b @L_801BC094
    li r3, 0x2
    b @L_801BC094
    li r3, 0x3
    b @L_801BC094
    li r3, 0x4
    b @L_801BC094
    li r3, 0x5
    b @L_801BC094
    li r3, 0x6
    b @L_801BC094
    li r3, 0x7
    b @L_801BC094
@L_801BC080:
    addi r3, r2, -22512
    li r4, 0x794
    addi r5, r2, -22480
    bl fn_80196E10
    li r3, 0x0
@L_801BC094:
    bl fn_801B38C0
    b @L_801BC320
@L_801BC09C:
    lwz r0, 0xc(r24)
    lwz r27, 0x58(r24)
    cmpwi r0, 0xff
    beq @L_801BC31C
    mr r3, r24
    bl fn_801BDD74
    cmplwi r27, 0x0
    bne @L_801BC0D0
    lis r4, lbl_80275688@ha
    addi r3, r2, -22512
    addi r5, r4, lbl_80275688@l
    li r4, 0x6b6
    bl fn_80196E10
@L_801BC0D0:
    lwz r0, 0x0(r27)
    cmplwi r0, 0x0
    bne @L_801BC0F0
    lis r4, lbl_80275694@ha
    addi r3, r2, -22512
    addi r5, r4, lbl_80275694@l
    li r4, 0x6b7
    bl fn_80196E10
@L_801BC0F0:
    lwz r0, 0x60(r24)
    cmplwi r0, 0x0
    beq @L_801BC104
    lwz r28, 0x60(r24)
    b @L_801BC110
@L_801BC104:
    lis r3, lbl_8036D594@ha
    addi r0, r3, lbl_8036D594@l
    mr r28, r0
@L_801BC110:
    lwz r0, 0x8(r27)
    lwz r26, 0x0(r28)
    cmpwi r0, 0xb
    bge @L_801BC138
    cmpwi r0, 0x7
    beq @L_801BC2C8
    bge @L_801BC144
    cmpwi r0, 0x0
    bge @L_801BC294
    b @L_801BC2C8
@L_801BC138:
    cmpwi r0, 0xe
    beq @L_801BC294
    b @L_801BC2C8
@L_801BC144:
    lbz r0, 0x70(r24)
    cmplwi r0, 0xff
    beq @L_801BC164
    lbz r0, 0x70(r24)
    lwz r3, 0x6c(r24)
    rlwinm r0, r0, 2, 0, 29
    lwzx r25, r3, r0
    b @L_801BC168
@L_801BC164:
    lwz r25, 0x5c(r24)
@L_801BC168:
    cmplwi r25, 0x0
    bne @L_801BC180
    addi r3, r2, -22512
    li r4, 0x6c7
    addi r5, r2, -22492
    bl fn_80196E10
@L_801BC180:
    li r5, 0x0
    b @L_801BC1B8
@L_801BC188:
    rlwinm r0, r5, 2, 0, 29
    addi r3, r1, 0x1c
    lwzx r3, r3, r0
    li r4, 0x1
    lhz r0, 0xc(r25)
    lhz r3, 0xc(r3)
    cmplw r3, r0
    bne @L_801BC1AC
    li r4, 0x0
@L_801BC1AC:
    cmpwi r4, 0x0
    beq @L_801BC1C0
    addi r5, r5, 0x1
@L_801BC1B8:
    cmpw r5, r31
    blt @L_801BC188
@L_801BC1C0:
    cmpw r5, r31
    bge @L_801BC1E0
    rlwinm r0, r5, 2, 0, 29
    addi r3, r1, 0x1c
    lwzx r3, r3, r0
    lwz r0, 0x8(r3)
    stw r0, 0x8(r25)
    b @L_801BC24C
@L_801BC1E0:
    cmpwi r31, 0x8
    bge @L_801BC244
    lhz r0, 0xc(r25)
    cmplwi r0, 0x100
    ble @L_801BC204
    mr r0, r29
    addi r29, r29, 0x1
    stw r0, 0x8(r25)
    b @L_801BC210
@L_801BC204:
    mr r0, r30
    addi r30, r30, 0x1
    stw r0, 0x8(r25)
@L_801BC210:
    lwz r4, 0x0(r25)
    addi r3, r1, 0x10
    lwz r5, 0x4(r25)
    lhz r6, 0xc(r25)
    bl fn_800BB050
    lwz r4, 0x8(r25)
    addi r3, r1, 0x10
    bl fn_800BB098
    rlwinm r0, r31, 2, 0, 29
    addi r3, r1, 0x1c
    stwx r25, r3, r0
    addi r31, r31, 0x1
    b @L_801BC24C
@L_801BC244:
    li r0, 0x0
    stw r0, 0x8(r25)
@L_801BC24C:
    lwz r0, 0x8(r25)
    addi r3, r1, 0x3c
    stw r0, 0x8(r1)
    lwz r6, 0xc(r27)
    lwz r4, 0x0(r27)
    neg r0, r6
    lhz r5, 0x4(r27)
    or r0, r0, r6
    lhz r6, 0x6(r27)
    lwz r7, 0x8(r27)
    rlwinm r10, r0, 1, 31, 31
    lwz r8, 0x40(r24)
    lwz r9, 0x44(r24)
    bl fn_800BAC58
    cmpwi r26, 0x5
    bne @L_801BC2D8
    li r26, 0x3
    b @L_801BC2D8
@L_801BC294:
    lwz r6, 0xc(r27)
    addi r3, r1, 0x3c
    lwz r4, 0x0(r27)
    neg r0, r6
    lhz r5, 0x4(r27)
    or r0, r0, r6
    lhz r6, 0x6(r27)
    lwz r7, 0x8(r27)
    rlwinm r10, r0, 1, 31, 31
    lwz r8, 0x40(r24)
    lwz r9, 0x44(r24)
    bl fn_800BA9E4
    b @L_801BC2D8
@L_801BC2C8:
    addi r3, r2, -22512
    li r4, 0x703
    addi r5, r2, -22480
    bl fn_80196E10
@L_801BC2D8:
    lwz r0, 0xc(r27)
    cmplwi r0, 0x0
    bne @L_801BC2E8
    rlwinm r26, r26, 0, 31, 31
@L_801BC2E8:
    lwz r5, 0x54(r24)
    mr r4, r26
    lfs f1, 0x10(r27)
    addi r3, r1, 0x3c
    lfs f2, 0x14(r27)
    lfs f3, 0x4(r28)
    lbz r6, 0x8(r28)
    lbz r7, 0x9(r28)
    lwz r8, 0xc(r28)
    bl fn_800BACA0
    lwz r4, 0xc(r24)
    addi r3, r1, 0x3c
    bl fn_800BAFFC
@L_801BC31C:
    lwz r24, 0x8(r24)
@L_801BC320:
    cmplwi r24, 0x0
    bne @L_801BC09C
@L_801BC328:
    lmw r24, 0x60(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr

