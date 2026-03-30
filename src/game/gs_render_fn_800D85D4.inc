    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r4
    beq @L_800D8790
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D8620
    mr r5, r3
    mr r6, r29
    li r3, 0x26
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D8790
    @L_800D8620:
    slwi r31, r3, 2
    add r3, r4, r31
    lwz r0, 0x28(r3)
    cmplw r0, r29
    beq @L_800D8790
    lbz r0, 0x7(r29)
    cmplwi r0, 0x0
    beq @L_800D873C
    lwz r4, 0x10(r29)
    lis r3, lbl_80314530@ha
    lwz r0, 0x14(r29)
    addi r5, r3, lbl_80314530@l
    slwi r4, r4, 2
    addi r3, r29, 0x54
    slwi r0, r0, 2
    lwzx r4, r5, r4
    lwzx r5, r5, r0
    bl fn_800BAE34
    lwz r0, 0x20(r29)
    cmpwi r0, 0x1
    beq @L_800D86AC
    bge @L_800D8684
    cmpwi r0, 0x0
    bge @L_800D8690
    b @L_800D86E0
    @L_800D8684:
    cmpwi r0, 0x3
    bge @L_800D86E0
    b @L_800D86C8
    @L_800D8690:
    lwz r0, 0x18(r29)
    cmpwi r0, 0x2
    bne @L_800D86A4
    li r30, 0x1
    b @L_800D86E0
    @L_800D86A4:
    li r30, 0x0
    b @L_800D86E0
    @L_800D86AC:
    lwz r0, 0x18(r29)
    cmpwi r0, 0x2
    bne @L_800D86C0
    li r30, 0x3
    b @L_800D86E0
    @L_800D86C0:
    li r30, 0x2
    b @L_800D86E0
    @L_800D86C8:
    lwz r0, 0x18(r29)
    cmpwi r0, 0x2
    bne @L_800D86DC
    li r30, 0x5
    b @L_800D86E0
    @L_800D86DC:
    li r30, 0x4
    @L_800D86E0:
    lbz r3, 0x5(r29)
    lis r0, 0x4330
    lwz r5, 0x1c(r29)
    mr r4, r30
    subi r3, r3, 0x1
    lfs f1, lbl_8047CA40(r2)
    xoris r3, r3, 0x8000
    subfic r5, r5, 0x2
    stw r3, 0xc(r1)
    cntlzw r5, r5
    lfd f2, lbl_8047CA48(r2)
    fmr f3, f1
    stw r0, 0x8(r1)
    addi r3, r29, 0x54
    srwi r5, r5, 5
    li r6, 0x0
    lfd f0, 0x8(r1)
    li r7, 0x0
    li r8, 0x0
    fsubs f2, f0, f2
    bl fn_800BACA0
    li r0, 0x0
    stb r0, 0x7(r29)
    @L_800D873C:
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r31
    stw r29, 0x28(r3)
    lwz r0, 0x48(r29)
    cmplwi r0, 0x0
    beq @L_800D8768
    lis r4, lbl_80314510@ha
    addi r3, r29, 0x74
    addi r4, r4, lbl_80314510@l
    lwzx r4, r4, r31
    bl fn_800BB098
    @L_800D8768:
    lis r4, lbl_803144F0@ha
    addi r3, r29, 0x54
    addi r4, r4, lbl_803144F0@l
    lwzx r4, r4, r31
    bl fn_800BAFFC
    lis r3, lbl_804001F0@ha
    addi r4, r3, lbl_804001F0@l
    lwz r3, 0x24(r4)
    addi r0, r3, 0x1
    stw r0, 0x24(r4)
    @L_800D8790:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
