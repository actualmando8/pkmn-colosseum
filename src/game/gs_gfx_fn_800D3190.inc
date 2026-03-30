    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r3, lbl_8047AA80(r13)
    lwz r3, 0xc(r3)
    addis r0, r3, 0x102
    cmplwi r0, 0xfefe
    beq @L_800D33F4
    li r0, 0x0
    stb r0, lbl_8047AA91(r13)
    bl fn_800B8920
    bl fn_800B8D10
    bl OSGetTick
    mr r29, r3
    lis r31, 0x8000
    li r30, 0x1
    b @L_800D3210
    @L_800D31E0:
    bl fn_800A1990
    lbz r0, lbl_8047AA91(r13)
    cmplwi r0, 0x0
    bne @L_800D3210
    bl OSGetTick
    lwz r0, 0xf8(r31)
    subf r3, r29, r3
    srwi r0, r0, 2
    divwu r0, r3, r0
    cmplwi r0, 0x3
    ble @L_800D3210
    stb r30, lbl_8047AA91(r13)
    @L_800D3210:
    lbz r0, lbl_8047AA91(r13)
    cmplwi r0, 0x0
    beq @L_800D31E0
    bl fn_8019C6FC
    lwz r4, lbl_8047AA80(r13)
    lwz r3, 0xc(r4)
    cmplwi r3, 0x0
    beq @L_800D32A8
    addis r0, r3, 0x102
    li r29, 0x1
    cmplwi r0, 0xfefe
    beq @L_800D32D8
    lbz r0, 0x49d(r4)
    cmplwi r0, 0x0
    bne @L_800D3264
    bl fn_800DC560
    li r3, 0x0
    bl fn_801BF8A0
    bl fn_801E16F0
    bl fn_801BF6AC
    b @L_800D3278
    @L_800D3264:
    cmplwi r3, 0x0
    beq @L_800D3278
    li r4, 0x1
    bl fn_800EF1E8
    li r29, 0x0
    @L_800D3278:
    lwz r3, lbl_8047AA80(r13)
    li r4, 0x1
    clrlwi. r0, r29, 24
    stb r4, 0x49d(r3)
    beq @L_800D32D8
    bl fn_800B8E74
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_800D32D8
    bl fn_800BB29C
    b @L_800D32D8
    @L_800D32A8:
    lis r3, lbl_804001F0@ha
    li r0, 0x0
    addi r6, r3, lbl_804001F0@l
    lwz r3, 0x10(r6)
    lwz r5, 0x4(r6)
    lwz r4, 0xc(r6)
    addi r3, r3, 0x1
    stw r5, 0x0(r6)
    stw r4, 0x8(r6)
    stw r3, 0x10(r6)
    stw r0, 0x4(r6)
    stw r0, 0xc(r6)
    @L_800D32D8:
    lis r4, 0xfeff
    lwz r3, lbl_8047AA80(r13)
    subi r0, r4, 0x102
    stw r0, 0xc(r3)
    lbz r0, lbl_8047AA90(r13)
    cmplwi r0, 0x0
    beq @L_800D33F4
    bl OSGetTick
    lwz r4, lbl_8047AA80(r13)
    mr r30, r3
    lwz r3, 0x54(r4)
    bl fn_800D1070
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r30, r3
    addi r4, r4, lbl_804001F0@l
    lwz r3, 0x2c(r4)
    add r0, r3, r0
    stw r0, 0x2c(r4)
    bl OSGetTick
    lwz r4, lbl_8047AA80(r13)
    mr r30, r3
    lwz r3, 0x54(r4)
    bl fn_800DC6D8
    bl OSGetTick
    lis r4, lbl_804001F0@ha
    subf r0, r30, r3
    addi r4, r4, lbl_804001F0@l
    lwz r3, 0x30(r4)
    add r0, r3, r0
    stw r0, 0x30(r4)
    bl OSGetTick
    lwz r5, lbl_8047AA80(r13)
    mr r30, r3
    li r4, 0x0
    lwz r3, 0x54(r5)
    bl fn_800E3884
    bl OSGetTick
    lis r5, lbl_804001F0@ha
    subf r4, r30, r3
    addi r30, r5, lbl_804001F0@l
    lwz r3, lbl_8047AA80(r13)
    lwz r5, 0x34(r30)
    li r0, 0x0
    add r4, r5, r4
    stw r4, 0x34(r30)
    lwz r29, 0x54(r3)
    stw r0, 0x38(r30)
    b @L_800D33E8
    @L_800D339C:
    bl OSGetTick
    mr r31, r3
    li r3, 0x1
    li r4, 0x1
    bl fn_800E3884
    bl OSGetTick
    lwz r4, 0x34(r30)
    subf r0, r31, r3
    add r0, r4, r0
    stw r0, 0x34(r30)
    bl OSGetTick
    mr r31, r3
    li r3, 0x1
    bl fn_801181B0
    bl OSGetTick
    lwz r4, 0x38(r30)
    subf r0, r31, r3
    add r0, r4, r0
    stw r0, 0x38(r30)
    @L_800D33E8:
    cmpwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_800D339C
    @L_800D33F4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
