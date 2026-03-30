    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    lis r3, lbl_80272088@ha
    addi r31, r3, lbl_80272088@l
    bl fn_801171C8
    bl fn_80117D14
    bl fn_80177A64
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80113B70
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80113B70
    li r3, 0x0
    bl fn_8010D8D4
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_80113B70
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80113B70
    li r3, 0x1
    bl fn_800D9ED8
    li r30, 0x1e
    li r29, 0x0
    lis r4, 0x6666
    lis r3, lbl_8035B818@ha
    addi r27, r4, 0x6667
    addi r28, r3, lbl_8035B818@l
@L_80113A98:
    lbz r0, lbl_8047AD60(r13)
    extsb r0, r0
    add r4, r0, r29
    mulhw r0, r27, r4
    srawi r0, r0, 2
    rlwinm r3, r0, 1, 31, 31
    add r0, r0, r3
    mulli r0, r0, 0xa
    subf r0, r0, r4
    rlwinm r0, r0, 3, 0, 28
    add r4, r28, r0
    lbz r0, 0x0(r4)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80113B14
    bge @L_80113AE4
    cmpwi r0, 0x1
    bge @L_80113AF0
    b @L_80113B58
@L_80113AE4:
    cmpwi r0, 0x4
    bge @L_80113B58
    b @L_80113B38
@L_80113AF0:
    lis r3, 0x8081
    lwz r7, 0x4(r4)
    mr r4, r30
    addi r6, r31, 0xa4
    subi r5, r3, 0x1
    li r3, 0x1e
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_80113B58
@L_80113B14:
    lis r3, 0x8081
    lwz r7, 0x4(r4)
    mr r4, r30
    addi r6, r31, 0xb4
    subi r5, r3, 0x1
    li r3, 0x1e
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_80113B58
@L_80113B38:
    lis r3, 0x8081
    lwz r7, 0x4(r4)
    mr r4, r30
    addi r6, r31, 0xc8
    subi r5, r3, 0x1
    li r3, 0x1e
    crxor 6, 6, 6
    bl fn_800FAEF8
@L_80113B58:
    addi r30, r30, 0xc
    addi r29, r29, 0x1
    cmpwi r29, 0xa
    blt @L_80113A98
    li r3, 0x0
    bl fn_800D9ED8
@L_80113B70:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

