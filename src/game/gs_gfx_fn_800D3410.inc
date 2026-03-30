    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    lwz r5, lbl_8047AA80(r13)
    lwz r3, 0xc(r5)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    bne @L_800D3600
    stw r31, 0xc(r5)
    li r30, 0x0
    lwz r4, lbl_8047AA80(r13)
    lwz r3, 0xc(r4)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    beq @L_800D34C4
    lbz r0, 0x49d(r4)
    cmplwi r0, 0x0
    bne @L_800D3484
    bl fn_800DC560
    li r3, 0x0
    bl fn_801BF8A0
    bl fn_801E16F0
    bl fn_801BF6AC
    b @L_800D3498
    @L_800D3484:
    cmplwi r3, 0x0
    beq @L_800D3498
    li r4, 0x1
    bl fn_800EF1E8
    li r30, 0x0
    @L_800D3498:
    lwz r3, lbl_8047AA80(r13)
    li r4, 0x1
    clrlwi. r0, r30, 24
    stb r4, 0x49d(r3)
    beq @L_800D34C4
    bl fn_800B8E74
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_800D34C4
    bl fn_800BB29C
    @L_800D34C4:
    clrlwi. r0, r29, 24
    stb r29, lbl_8047AA90(r13)
    beq @L_800D3570
    bl OSGetTick
    lwz r4, lbl_8047AA80(r13)
    mr r30, r3
    lwz r3, 0x54(r4)
    bl fn_800D13C4
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r30, r3
    addi r3, r4, lbl_804001F0@l
    stw r0, 0x2c(r3)
    bl OSGetTick
    lwz r4, lbl_8047AA80(r13)
    mr r30, r3
    lwz r3, 0x54(r4)
    bl fn_800DC874
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r30, r3
    addi r3, r4, lbl_804001F0@l
    stw r0, 0x30(r3)
    bl OSGetTick
    lwz r4, lbl_8047AA80(r13)
    mr r30, r3
    lwz r3, 0x54(r4)
    bl fn_800E3928
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r30, r3
    addi r3, r4, lbl_804001F0@l
    stw r0, 0x34(r3)
    bl OSGetTick
    lwz r4, lbl_8047AA80(r13)
    mr r30, r3
    lwz r3, 0x54(r4)
    bl fn_801183EC
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r30, r3
    addi r3, r4, lbl_804001F0@l
    stw r0, 0x38(r3)
    @L_800D3570:
    cmplwi r31, 0x0
    bne @L_800D35F8
    bl OSGetTick
    mr r31, r3
    bl fn_800E8684
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r31, r3
    addi r4, r4, lbl_804001F0@l
    li r3, 0x0
    stw r0, 0x3c(r4)
    bl fn_8019C708
    lwz r6, lbl_8047AA80(r13)
    lbz r0, 0x19(r6)
    cmplwi r0, 0x0
    beq @L_800D35E8
    lbz r3, 0x1c(r6)
    lbz r4, 0x1d(r6)
    lbz r5, 0x1e(r6)
    lbz r6, 0x1f(r6)
    bl fn_8019731C
    lfs f1, lbl_8047CA00(r2)
    li r3, 0x1
    lfs f2, lbl_8047CA08(r2)
    li r4, 0x1
    fmr f3, f1
    lfs f4, lbl_8047CA04(r2)
    fmr f5, f1
    li r5, 0x0
    bl fn_80196EF8
    @L_800D35E8:
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x0
    stb r0, 0x49d(r3)
    b @L_800D3600
    @L_800D35F8:
    li r3, 0x3
    bl fn_8019C708
    @L_800D3600:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
