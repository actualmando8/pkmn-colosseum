    nofralloc
    stwu r1, -0xa40(r1)
    mflr r0
    stw r0, 0xa44(r1)
    stmw r22, 0xa18(r1)
    mr r23, r3
    mr r22, r4
    mr r25, r5
    lis r4, lbl_80268DC0@ha
    li r31, 0x1
    addi r30, r4, lbl_80268DC0@l
    li r4, 0x0
    li r5, 0xb20
    bl memset
    addi r5, r30, 0x0
    li r26, 0x0
    lwz r24, 0x4(r5)
    stw r23, 0x8(r1)
    cmpwi r24, 0x10
    stw r22, 0xc(r1)
    stw r25, 0x10(r1)
    stw r26, 0x14(r1)
    bge @L_80080410
    lwz r22, 0x8(r5)
    b @L_80080404
@L_80080370:
    lwz r7, 0x14(r1)
    li r5, 0x0
    lwz r6, 0xc(r1)
    add r3, r7, r24
    subf r0, r7, r3
    la r4, lbl_80478948(r13)
    mtctr r0
    cmpw r7, r3
    bge @L_800803D8
@L_80080394:
    srawi r0, r7, 3
    rlwinm r3, r7, 0, 29, 31
    addze r0, r0
    rlwinm r5, r5, 0, 16, 31
    lbzx r3, r4, r3
    rlwinm r5, r5, 1, 0, 30
    lbzx r0, r6, r0
    and r0, r3, r0
    cmpwi r0, 0x0
    beq @L_800803C4
    li r0, 0x1
    b @L_800803C8
@L_800803C4:
    li r0, 0x0
@L_800803C8:
    or r0, r5, r0
    addi r7, r7, 0x1
    rlwinm r5, r0, 0, 16, 31
    bdnz @L_80080394
@L_800803D8:
    lwz r0, 0x14(r1)
    mr r8, r26
    addi r3, r1, 0x8
    addi r4, r30, 0x0
    add r0, r0, r24
    rlwinm r6, r5, 0, 16, 31
    stw r0, 0x14(r1)
    li r5, -0x1
    li r7, 0x0
    bl fn_8008102C
    addi r26, r26, 0x1
@L_80080404:
    cmpw r26, r22
    blt @L_80080370
    b @L_8008053C
@L_80080410:
    addi r4, r1, 0x818
    li r3, 0x0
    b @L_80080490
@L_8008041C:
    mr r9, r3
    li r8, 0x0
    addi r6, r3, 0x10
    la r7, lbl_80478948(r13)
    subf r0, r3, r6
    mtctr r0
    cmpw r3, r6
    bge @L_80080480
@L_8008043C:
    srawi r0, r9, 3
    rlwinm r6, r9, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r22, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_8008046C
    li r0, 0x1
    b @L_80080470
@L_8008046C:
    li r0, 0x0
@L_80080470:
    or r0, r8, r0
    addi r9, r9, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_8008043C
@L_80080480:
    sth r8, 0x0(r4)
    addi r4, r4, 0x2
    addi r3, r3, 0x10
    subi r24, r24, 0x10
@L_80080490:
    cmpwi r24, 0x10
    bgt @L_8008041C
    cmpwi r24, 0x0
    beq @L_80080508
    add r6, r3, r24
    li r8, 0x0
    subf r0, r3, r6
    la r7, lbl_80478948(r13)
    mtctr r0
    cmpw r3, r6
    bge @L_80080500
@L_800804BC:
    srawi r0, r3, 3
    rlwinm r6, r3, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r22, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_800804EC
    li r0, 0x1
    b @L_800804F0
@L_800804EC:
    li r0, 0x0
@L_800804F0:
    or r0, r8, r0
    addi r3, r3, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_800804BC
@L_80080500:
    sth r8, 0x0(r4)
    addi r4, r4, 0x2
@L_80080508:
    lwz r6, 0x14(r1)
    li r7, 0x0
    lwz r0, 0x4(r5)
    addi r3, r1, 0x8
    sth r7, 0x0(r4)
    addi r4, r30, 0x0
    add r0, r6, r0
    addi r7, r1, 0x818
    stw r0, 0x14(r1)
    li r5, -0x1
    li r6, 0x0
    li r8, -0x1
    bl fn_8008102C
@L_8008053C:
    li r27, 0x0
    stw r27, 0x14(r1)
    lwz r0, 0x0(r23)
    cmpwi r0, 0x1
    beq @L_80080C4C
    bge @L_80080E8C
    cmpwi r0, 0x0
    bge @L_80080560
    b @L_80080E8C
@L_80080560:
    addi r28, r30, 0x0
    li r22, 0x0
@L_80080568:
    lwz r9, 0x4(r28)
    li r27, 0x1
    cmpwi r9, 0x10
    bge @L_80080640
    addi r24, r28, 0x4
    addi r25, r28, 0x8
    li r26, 0x0
    b @L_80080630
@L_80080588:
    lwz r8, 0x14(r1)
    li r6, 0x0
    lwz r0, 0x0(r24)
    lwz r7, 0xc(r1)
    add r4, r8, r0
    subf r3, r8, r4
    la r5, lbl_80478948(r13)
    mtctr r3
    cmpw r8, r4
    bge @L_800805F4
@L_800805B0:
    srawi r3, r8, 3
    rlwinm r4, r8, 0, 29, 31
    addze r3, r3
    rlwinm r6, r6, 0, 16, 31
    lbzx r4, r5, r4
    rlwinm r6, r6, 1, 0, 30
    lbzx r3, r7, r3
    and r3, r4, r3
    cmpwi r3, 0x0
    beq @L_800805E0
    li r3, 0x1
    b @L_800805E4
@L_800805E0:
    li r3, 0x0
@L_800805E4:
    or r3, r6, r3
    addi r8, r8, 0x1
    rlwinm r6, r3, 0, 16, 31
    bdnz @L_800805B0
@L_800805F4:
    lwz r5, 0x14(r1)
    mr r4, r28
    mr r8, r26
    addi r3, r1, 0x8
    add r0, r5, r0
    rlwinm r6, r6, 0, 16, 31
    stw r0, 0x14(r1)
    li r5, -0x1
    li r7, 0x0
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008062C
    li r27, 0x0
@L_8008062C:
    addi r26, r26, 0x1
@L_80080630:
    lwz r0, 0x0(r25)
    cmpw r26, r0
    blt @L_80080588
    b @L_80080780
@L_80080640:
    lwz r4, 0x14(r1)
    addi r5, r1, 0x618
    lwz r3, 0xc(r1)
    b @L_800806C4
@L_80080650:
    mr r10, r4
    li r8, 0x0
    addi r6, r4, 0x10
    la r7, lbl_80478948(r13)
    subf r0, r4, r6
    mtctr r0
    cmpw r4, r6
    bge @L_800806B4
@L_80080670:
    srawi r0, r10, 3
    rlwinm r6, r10, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_800806A0
    li r0, 0x1
    b @L_800806A4
@L_800806A0:
    li r0, 0x0
@L_800806A4:
    or r0, r8, r0
    addi r10, r10, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_80080670
@L_800806B4:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
    addi r4, r4, 0x10
    subi r9, r9, 0x10
@L_800806C4:
    cmpwi r9, 0x10
    bgt @L_80080650
    cmpwi r9, 0x0
    beq @L_8008073C
    add r6, r4, r9
    li r8, 0x0
    subf r0, r4, r6
    la r7, lbl_80478948(r13)
    mtctr r0
    cmpw r4, r6
    bge @L_80080734
@L_800806F0:
    srawi r0, r4, 3
    rlwinm r6, r4, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_80080720
    li r0, 0x1
    b @L_80080724
@L_80080720:
    li r0, 0x0
@L_80080724:
    or r0, r8, r0
    addi r4, r4, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_800806F0
@L_80080734:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
@L_8008073C:
    lwz r6, 0x14(r1)
    li r3, 0x0
    lwz r0, 0x4(r28)
    mr r4, r28
    sth r3, 0x0(r5)
    addi r3, r1, 0x8
    add r0, r6, r0
    addi r7, r1, 0x618
    stw r0, 0x14(r1)
    li r5, -0x1
    li r6, 0x0
    li r8, -0x1
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080780
    li r27, 0x0
@L_80080780:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080790
    li r31, 0x0
@L_80080790:
    addi r28, r28, 0xc
    addi r22, r22, 0x1
    cmplwi r22, 0x28
    blt @L_80080568
    li r26, 0x0
@L_800807A4:
    mr r28, r26
    addi r29, r30, 0x1e0
    li r27, 0x0
@L_800807B0:
    lwz r9, 0x4(r29)
    li r24, 0x1
    cmpwi r9, 0x10
    bge @L_80080888
    addi r23, r29, 0x4
    addi r22, r29, 0x8
    li r25, 0x0
    b @L_80080878
@L_800807D0:
    lwz r4, 0x14(r1)
    li r9, 0x0
    lwz r0, 0x0(r23)
    lwz r3, 0xc(r1)
    add r6, r4, r0
    subf r5, r4, r6
    la r7, lbl_80478948(r13)
    mtctr r5
    cmpw r4, r6
    bge @L_8008083C
@L_800807F8:
    srawi r5, r4, 3
    rlwinm r6, r4, 0, 29, 31
    addze r5, r5
    rlwinm r8, r9, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r5, r3, r5
    and r5, r6, r5
    cmpwi r5, 0x0
    beq @L_80080828
    li r5, 0x1
    b @L_8008082C
@L_80080828:
    li r5, 0x0
@L_8008082C:
    or r5, r8, r5
    addi r4, r4, 0x1
    rlwinm r9, r5, 0, 16, 31
    bdnz @L_800807F8
@L_8008083C:
    lwz r3, 0x14(r1)
    mr r4, r29
    mr r5, r28
    mr r8, r25
    add r0, r3, r0
    addi r3, r1, 0x8
    stw r0, 0x14(r1)
    rlwinm r6, r9, 0, 16, 31
    li r7, 0x0
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080874
    li r24, 0x0
@L_80080874:
    addi r25, r25, 0x1
@L_80080878:
    lwz r0, 0x0(r22)
    cmpw r25, r0
    blt @L_800807D0
    b @L_800809C8
@L_80080888:
    lwz r4, 0x14(r1)
    addi r5, r1, 0x418
    lwz r3, 0xc(r1)
    b @L_8008090C
@L_80080898:
    mr r10, r4
    li r8, 0x0
    addi r6, r4, 0x10
    la r7, lbl_80478948(r13)
    subf r0, r4, r6
    mtctr r0
    cmpw r4, r6
    bge @L_800808FC
@L_800808B8:
    srawi r0, r10, 3
    rlwinm r6, r10, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_800808E8
    li r0, 0x1
    b @L_800808EC
@L_800808E8:
    li r0, 0x0
@L_800808EC:
    or r0, r8, r0
    addi r10, r10, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_800808B8
@L_800808FC:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
    addi r4, r4, 0x10
    subi r9, r9, 0x10
@L_8008090C:
    cmpwi r9, 0x10
    bgt @L_80080898
    cmpwi r9, 0x0
    beq @L_80080984
    add r6, r4, r9
    li r8, 0x0
    subf r0, r4, r6
    la r7, lbl_80478948(r13)
    mtctr r0
    cmpw r4, r6
    bge @L_8008097C
@L_80080938:
    srawi r0, r4, 3
    rlwinm r6, r4, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_80080968
    li r0, 0x1
    b @L_8008096C
@L_80080968:
    li r0, 0x0
@L_8008096C:
    or r0, r8, r0
    addi r4, r4, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_80080938
@L_8008097C:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
@L_80080984:
    lwz r3, 0x14(r1)
    li r6, 0x0
    lwz r0, 0x4(r29)
    mr r4, r29
    sth r6, 0x0(r5)
    mr r5, r28
    add r0, r3, r0
    addi r3, r1, 0x8
    stw r0, 0x14(r1)
    addi r7, r1, 0x418
    li r6, 0x0
    li r8, -0x1
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800809C8
    li r24, 0x0
@L_800809C8:
    rlwinm r0, r24, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800809D8
    li r31, 0x0
@L_800809D8:
    addi r29, r29, 0xc
    addi r27, r27, 0x1
    cmplwi r27, 0x8
    blt @L_800807B0
    addi r26, r26, 0x1
    cmpwi r26, 0x9
    blt @L_800807A4
    li r27, 0x0
@L_800809F8:
    addi r28, r30, 0x240
    li r29, 0x0
@L_80080A00:
    lwz r9, 0x4(r28)
    li r25, 0x1
    cmpwi r9, 0x10
    bge @L_80080ADC
    mr r23, r27
    addi r22, r28, 0x4
    addi r26, r28, 0x8
    li r24, 0x0
    b @L_80080ACC
@L_80080A24:
    lwz r4, 0x14(r1)
    li r9, 0x0
    lwz r0, 0x0(r22)
    lwz r3, 0xc(r1)
    add r6, r4, r0
    subf r5, r4, r6
    la r7, lbl_80478948(r13)
    mtctr r5
    cmpw r4, r6
    bge @L_80080A90
@L_80080A4C:
    srawi r5, r4, 3
    rlwinm r6, r4, 0, 29, 31
    addze r5, r5
    rlwinm r8, r9, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r5, r3, r5
    and r5, r6, r5
    cmpwi r5, 0x0
    beq @L_80080A7C
    li r5, 0x1
    b @L_80080A80
@L_80080A7C:
    li r5, 0x0
@L_80080A80:
    or r5, r8, r5
    addi r4, r4, 0x1
    rlwinm r9, r5, 0, 16, 31
    bdnz @L_80080A4C
@L_80080A90:
    lwz r3, 0x14(r1)
    mr r4, r28
    mr r5, r23
    mr r8, r24
    add r0, r3, r0
    addi r3, r1, 0x8
    stw r0, 0x14(r1)
    rlwinm r6, r9, 0, 16, 31
    li r7, 0x0
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080AC8
    li r25, 0x0
@L_80080AC8:
    addi r24, r24, 0x1
@L_80080ACC:
    lwz r0, 0x0(r26)
    cmpw r24, r0
    blt @L_80080A24
    b @L_80080C1C
@L_80080ADC:
    lwz r4, 0x14(r1)
    addi r5, r1, 0x218
    lwz r3, 0xc(r1)
    b @L_80080B60
@L_80080AEC:
    mr r10, r4
    li r8, 0x0
    addi r6, r4, 0x10
    la r7, lbl_80478948(r13)
    subf r0, r4, r6
    mtctr r0
    cmpw r4, r6
    bge @L_80080B50
@L_80080B0C:
    srawi r0, r10, 3
    rlwinm r6, r10, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_80080B3C
    li r0, 0x1
    b @L_80080B40
@L_80080B3C:
    li r0, 0x0
@L_80080B40:
    or r0, r8, r0
    addi r10, r10, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_80080B0C
@L_80080B50:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
    addi r4, r4, 0x10
    subi r9, r9, 0x10
@L_80080B60:
    cmpwi r9, 0x10
    bgt @L_80080AEC
    cmpwi r9, 0x0
    beq @L_80080BD8
    add r6, r4, r9
    li r8, 0x0
    subf r0, r4, r6
    la r7, lbl_80478948(r13)
    mtctr r0
    cmpw r4, r6
    bge @L_80080BD0
@L_80080B8C:
    srawi r0, r4, 3
    rlwinm r6, r4, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_80080BBC
    li r0, 0x1
    b @L_80080BC0
@L_80080BBC:
    li r0, 0x0
@L_80080BC0:
    or r0, r8, r0
    addi r4, r4, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_80080B8C
@L_80080BD0:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
@L_80080BD8:
    lwz r3, 0x14(r1)
    li r6, 0x0
    lwz r0, 0x4(r28)
    mr r4, r28
    sth r6, 0x0(r5)
    mr r5, r27
    add r0, r3, r0
    addi r3, r1, 0x8
    stw r0, 0x14(r1)
    addi r7, r1, 0x218
    li r6, 0x0
    li r8, -0x1
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080C1C
    li r25, 0x0
@L_80080C1C:
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080C2C
    li r31, 0x0
@L_80080C2C:
    addi r28, r28, 0xc
    addi r29, r29, 0x1
    cmplwi r29, 0x18
    blt @L_80080A00
    addi r27, r27, 0x1
    cmpwi r27, 0x24
    blt @L_800809F8
    b @L_80080E90
@L_80080C4C:
    addi r26, r30, 0x360
@L_80080C50:
    lwz r9, 0x4(r26)
    li r25, 0x1
    cmpwi r9, 0x10
    bge @L_80080D28
    addi r22, r26, 0x4
    addi r23, r26, 0x8
    li r24, 0x0
    b @L_80080D18
@L_80080C70:
    lwz r8, 0x14(r1)
    li r6, 0x0
    lwz r0, 0x0(r22)
    lwz r7, 0xc(r1)
    add r4, r8, r0
    subf r3, r8, r4
    la r5, lbl_80478948(r13)
    mtctr r3
    cmpw r8, r4
    bge @L_80080CDC
@L_80080C98:
    srawi r3, r8, 3
    rlwinm r4, r8, 0, 29, 31
    addze r3, r3
    rlwinm r6, r6, 0, 16, 31
    lbzx r4, r5, r4
    rlwinm r6, r6, 1, 0, 30
    lbzx r3, r7, r3
    and r3, r4, r3
    cmpwi r3, 0x0
    beq @L_80080CC8
    li r3, 0x1
    b @L_80080CCC
@L_80080CC8:
    li r3, 0x0
@L_80080CCC:
    or r3, r6, r3
    addi r8, r8, 0x1
    rlwinm r6, r3, 0, 16, 31
    bdnz @L_80080C98
@L_80080CDC:
    lwz r5, 0x14(r1)
    mr r4, r26
    mr r8, r24
    addi r3, r1, 0x8
    add r0, r5, r0
    rlwinm r6, r6, 0, 16, 31
    stw r0, 0x14(r1)
    li r5, -0x1
    li r7, 0x0
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080D14
    li r25, 0x0
@L_80080D14:
    addi r24, r24, 0x1
@L_80080D18:
    lwz r0, 0x0(r23)
    cmpw r24, r0
    blt @L_80080C70
    b @L_80080E68
@L_80080D28:
    lwz r4, 0x14(r1)
    addi r5, r1, 0x18
    lwz r3, 0xc(r1)
    b @L_80080DAC
@L_80080D38:
    mr r10, r4
    li r8, 0x0
    addi r6, r4, 0x10
    la r7, lbl_80478948(r13)
    subf r0, r4, r6
    mtctr r0
    cmpw r4, r6
    bge @L_80080D9C
@L_80080D58:
    srawi r0, r10, 3
    rlwinm r6, r10, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_80080D88
    li r0, 0x1
    b @L_80080D8C
@L_80080D88:
    li r0, 0x0
@L_80080D8C:
    or r0, r8, r0
    addi r10, r10, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_80080D58
@L_80080D9C:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
    addi r4, r4, 0x10
    subi r9, r9, 0x10
@L_80080DAC:
    cmpwi r9, 0x10
    bgt @L_80080D38
    cmpwi r9, 0x0
    beq @L_80080E24
    add r6, r4, r9
    li r8, 0x0
    subf r0, r4, r6
    la r7, lbl_80478948(r13)
    mtctr r0
    cmpw r4, r6
    bge @L_80080E1C
@L_80080DD8:
    srawi r0, r4, 3
    rlwinm r6, r4, 0, 29, 31
    addze r0, r0
    rlwinm r8, r8, 0, 16, 31
    lbzx r6, r7, r6
    rlwinm r8, r8, 1, 0, 30
    lbzx r0, r3, r0
    and r0, r6, r0
    cmpwi r0, 0x0
    beq @L_80080E08
    li r0, 0x1
    b @L_80080E0C
@L_80080E08:
    li r0, 0x0
@L_80080E0C:
    or r0, r8, r0
    addi r4, r4, 0x1
    rlwinm r8, r0, 0, 16, 31
    bdnz @L_80080DD8
@L_80080E1C:
    sth r8, 0x0(r5)
    addi r5, r5, 0x2
@L_80080E24:
    lwz r6, 0x14(r1)
    li r3, 0x0
    lwz r0, 0x4(r26)
    mr r4, r26
    sth r3, 0x0(r5)
    addi r3, r1, 0x8
    add r0, r6, r0
    addi r7, r1, 0x18
    stw r0, 0x14(r1)
    li r5, -0x1
    li r6, 0x0
    li r8, -0x1
    bl fn_8008102C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080E68
    li r25, 0x0
@L_80080E68:
    rlwinm r0, r25, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080E78
    li r31, 0x0
@L_80080E78:
    addi r26, r26, 0xc
    addi r27, r27, 0x1
    cmplwi r27, 0x3
    blt @L_80080C50
    b @L_80080E90
@L_80080E8C:
    li r31, 0x0
@L_80080E90:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80080EA4
    li r3, 0x0
    b @L_80080EC4
@L_80080EA4:
    lwz r0, 0x10(r1)
    lwz r3, 0x14(r1)
    rlwinm r4, r0, 3, 0, 28
    subf r0, r3, r4
    orc r3, r4, r3
    rlwinm r0, r0, 31, 1, 31
    subf r0, r0, r3
    rlwinm r3, r0, 1, 31, 31
@L_80080EC4:
    lmw r22, 0xa18(r1)
    lwz r0, 0xa44(r1)
    mtlr r0
    addi r1, r1, 0xa40
    blr

