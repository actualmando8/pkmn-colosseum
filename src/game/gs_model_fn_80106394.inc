    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r30, r3
    mr r31, r4
    mr r27, r5
    li r5, 0x50
    li r29, 0x0
    cmpwi r5, 0x0
    bne @L_801063C4
    b @L_801064C8
@L_801063C4:
    lbz r0, lbl_80478B30(r13)
    extsb r4, r0
    cmpwi r4, 0x4
    beq @L_80106474
    lbz r0, lbl_8047AD10(r13)
    cmplwi r0, 0x0
    bne @L_80106474
    cmplwi r4, 0x9
    mr r28, r29
    bgt @L_80106450
    lis r3, jumptable_8035B1E8@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8035B1E8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r28, 0x40
    b @L_80106450
    li r28, 0xf
    b @L_80106450
    li r28, 0x10
    b @L_80106450
    li r28, 0x40
    b @L_80106450
    mr r28, r5
    b @L_80106450
    li r28, 0x51
    b @L_80106450
    li r28, 0x10c
    b @L_80106450
    li r28, 0xe6
    b @L_80106450
    li r28, 0xe8
    b @L_80106450
    li r28, 0x107
@L_80106450:
    mr r3, r28
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106474
    mr r3, r28
    mr r5, r31
    li r4, 0x2
    bl fn_80102568
@L_80106474:
    li r0, 0x4
    stb r0, lbl_80478B30(r13)
    bl fn_801046B8
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106494
    ori r0, r29, 0x1
    rlwinm r29, r0, 0, 24, 31
@L_80106494:
    li r0, 0x0
    mr r4, r3
    stw r0, 0x8(r1)
    mr r7, r31
    mr r9, r30
    rlwinm r10, r29, 0, 24, 31
    li r3, 0x50
    li r5, 0x0
    li r6, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
    mr r29, r3
@L_801064C8:
    mr r3, r29
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

