    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x84(r1)
    stmw r26, 0x68(r1)
    mr r31, r4
    mr r27, r5
    mr r26, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r30, r3, 0, 16, 31
    mr r3, r26
    mr r4, r31
    addi r5, r1, 0x54
    addi r6, r1, 0x40
    li r7, 0x1
    bl fn_802367CC
    rlwinm. r0, r3, 0, 16, 31
    mr r28, r3
    bne @L_8023A378
    li r3, 0x0
    b @L_8023A72C
@L_8023A378:
    mr r4, r26
    addi r5, r1, 0x20
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    rlwinm. r0, r3, 0, 16, 31
    mr r29, r3
    bne @L_8023A3A4
    li r3, 0x0
    b @L_8023A72C
@L_8023A3A4:
    mr r4, r30
    li r3, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023A420
    mr r3, r31
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    mr r7, r3
    mr r6, r26
    subi r3, r5, 0x1395
    stw r0, 0xc(r1)
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13d3
    li r8, 0x0
    stw r0, 0x10(r1)
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r29, 0, 16, 31
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r3, r0, r4
    b @L_8023A454
@L_8023A420:
    li r3, 0x0
    li r0, -0x1
    stw r3, 0x1c(r1)
    mr r3, r26
    mr r4, r31
    mr r5, r28
    sth r0, 0x18(r1)
    mr r7, r29
    addi r6, r1, 0x54
    addi r8, r1, 0x20
    addi r9, r1, 0x1c
    addi r10, r1, 0x18
    bl fn_8023B498
@L_8023A454:
    cmpwi r3, 0x0
    bge @L_8023A5B0
    lwz r30, 0x1c(r1)
    cmplwi r30, 0x0
    beq @L_8023A5A8
    lha r0, 0x18(r1)
    cmpwi r0, 0x0
    blt @L_8023A5A8
    mr r3, r30
    mr r4, r27
    bl fn_801F0134
    lha r0, 0x18(r1)
    mr r27, r3
    mr r3, r26
    mr r4, r31
    rlwinm r5, r0, 0, 24, 31
    bl fn_80238270
    rlwinm. r8, r3, 0, 16, 31
    mr r29, r3
    beq @L_8023A5A0
    li r0, 0x0
    lis r3, lbl_80375CA8@ha
    stw r0, 0x8(r1)
    addi r7, r3, lbl_80375CA8@l
    mr r3, r31
    mr r9, r27
    lha r0, 0x18(r1)
    li r4, 0x0
    li r5, 0x13
    li r6, 0x0
    extsb r10, r0
    bl fn_80204F6C
    mr r3, r30
    bl fn_80205B8C
    mr r28, r3
    mr r4, r30
    li r3, 0x0
    bl fn_801F4354
    mr r27, r3
    mr r3, r31
    bl fn_80205B8C
    mr r26, r3
    mr r4, r31
    li r3, 0x0
    bl fn_801F4354
    li r4, 0x0
    li r0, 0x228
    stw r4, 0x8(r1)
    lis r5, 0x1
    mr r6, r3
    mr r7, r26
    stw r0, 0xc(r1)
    mr r8, r27
    mr r9, r28
    subi r3, r5, 0x1394
    stw r4, 0x10(r1)
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13d3
    li r10, 0x0
    bl fn_8023A118
    mr r3, r31
    bl fn_80205B8C
    mr r26, r3
    mr r4, r31
    li r3, 0x0
    bl fn_801F4354
    li r4, 0x0
    li r0, 0x228
    stw r4, 0x8(r1)
    lis r5, 0x1
    mr r6, r3
    mr r7, r26
    stw r0, 0xc(r1)
    mr r10, r29
    subi r3, r5, 0x139c
    li r8, 0x0
    stw r4, 0x10(r1)
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13ce
    li r9, 0x0
    bl fn_8023A118
    li r3, 0x1
    b @L_8023A72C
@L_8023A5A0:
    li r3, 0x0
    b @L_8023A72C
@L_8023A5A8:
    li r3, 0x0
    b @L_8023A72C
@L_8023A5B0:
    rlwinm r0, r3, 2, 0, 29
    addi r3, r1, 0x20
    lwzx r29, r3, r0
    cmplwi r29, 0x0
    bne @L_8023A5CC
    li r3, 0x0
    b @L_8023A72C
@L_8023A5CC:
    mr r4, r30
    li r3, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023A648
    mr r3, r31
    bl fn_80205B8C
    li r0, 0x0
    lis r5, 0x1
    stw r0, 0x8(r1)
    mr r7, r3
    mr r6, r26
    subi r3, r5, 0x1395
    stw r0, 0xc(r1)
    subi r4, r5, 0x13d4
    subi r5, r5, 0x13ce
    li r8, 0x0
    stw r0, 0x10(r1)
    li r9, 0x0
    li r10, 0x0
    bl fn_8023A118
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r28, 0, 16, 31
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r3, r0, r4
    b @L_8023A668
@L_8023A648:
    mr r3, r26
    mr r4, r31
    mr r5, r28
    mr r8, r29
    mr r9, r27
    addi r6, r1, 0x54
    addi r7, r1, 0x40
    bl fn_8023A740
@L_8023A668:
    cmpwi r3, 0x0
    bge @L_8023A678
    li r3, 0x0
    b @L_8023A72C
@L_8023A678:
    rlwinm r0, r3, 1, 0, 30
    addi r3, r1, 0x54
    lhzx r28, r3, r0
    cmplwi r28, 0x0
    bne @L_8023A694
    li r3, 0x0
    b @L_8023A72C
@L_8023A694:
    addi r3, r1, 0x40
    lhax r26, r3, r0
    cmpwi r26, 0x0
    bge @L_8023A6AC
    li r3, 0x0
    b @L_8023A72C
@L_8023A6AC:
    lis r4, -32732
    mr r3, r31
    addi r6, r4, -15512
    mr r5, r27
    mr r4, r28
    li r7, 0x1
    li r8, 0x0
    li r9, -0x1
    bl fn_8022B2CC
    cmplwi r3, 0x0
    beq @L_8023A6DC
    mr r29, r3
@L_8023A6DC:
    cmplwi r29, 0x0
    bne @L_8023A6EC
    li r3, 0x0
    b @L_8023A72C
@L_8023A6EC:
    mr r3, r29
    mr r4, r27
    bl fn_801F0134
    li r0, 0x0
    lis r4, lbl_80375CA8@ha
    stw r0, 0x8(r1)
    mr r9, r3
    addi r7, r4, lbl_80375CA8@l
    mr r3, r31
    mr r8, r28
    extsb r10, r26
    li r4, 0x0
    li r5, 0x13
    li r6, 0x0
    bl fn_80204F6C
    li r3, 0x1
@L_8023A72C:
    lmw r26, 0x68(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
