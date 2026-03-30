    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    bl fn_800A8B34
    lis r3, lbl_80466BC0@ha
    li r0, 0xa
    addi r3, r3, lbl_80466BC0@l
    subi r4, r28, 0x4
    subi r6, r3, 0x4
    mtctr r0
    @L_801BF238:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_801BF238
    lwz r0, 0x4(r4)
    lis r8, lbl_80466BC0@ha
    lis r5, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    lis r3, lbl_80466BC0@ha
    stw r0, 0x4(r6)
    addi r7, r5, lbl_80466BC0@l
    addi r6, r4, lbl_80466BC0@l
    addi r5, r3, lbl_80466BC0@l
    addi r8, r8, lbl_80466BC0@l
    li r0, 0x0
    stw r29, 0x58(r7)
    li r3, 0x0
    li r4, 0x0
    stb r0, 0x54(r8)
    stw r30, 0xb8(r6)
    stw r31, 0x118(r5)
    b @L_801BF328
    @L_801BF294:
    mulli r7, r3, 0x60
    lis r6, lbl_80466BC0@ha
    lis r5, lbl_80466BC0@ha
    addi r0, r6, lbl_80466BC0@l
    add r6, r0, r7
    addi r5, r5, lbl_80466BC0@l
    li r0, 0xb
    addi r7, r6, 0x5c
    subi r6, r5, 0x4
    mtctr r0
    @L_801BF2BC:
    lwz r5, 0x4(r6)
    lwzu r0, 0x8(r6)
    stw r5, 0x4(r7)
    stwu r0, 0x8(r7)
    bdnz @L_801BF2BC
    mulli r6, r3, 0x60
    lis r5, lbl_80466BC0@ha
    addi r0, r5, lbl_80466BC0@l
    add r5, r0, r6
    lwz r0, 0x58(r5)
    cmplwi r0, 0x0
    beq @L_801BF30C
    mulli r6, r3, 0x60
    lis r5, lbl_80466BC0@ha
    li r7, 0x2
    addi r0, r5, lbl_80466BC0@l
    add r5, r0, r6
    addi r4, r4, 0x1
    stw r7, 0x5c(r5)
    b @L_801BF324
    @L_801BF30C:
    mulli r6, r3, 0x60
    lis r5, lbl_80466BC0@ha
    li r7, 0x0
    addi r0, r5, lbl_80466BC0@l
    add r5, r0, r6
    stw r7, 0x5c(r5)
    @L_801BF324:
    addi r3, r3, 0x1
    @L_801BF328:
    cmpwi r3, 0x3
    blt @L_801BF294
    lis r7, lbl_80466BC0@ha
    lis r6, lbl_80466BC0@ha
    lis r5, lbl_80466BC0@ha
    lis r3, lbl_80466BC0@ha
    addi r8, r7, lbl_80466BC0@l
    addi r6, r6, lbl_80466BC0@l
    addi r5, r5, lbl_80466BC0@l
    addi r3, r3, lbl_80466BC0@l
    li r7, 0x0
    li r0, 0xb
    stw r4, 0x1d4(r8)
    addi r5, r5, 0x178
    subi r4, r3, 0x4
    stw r7, 0x178(r6)
    mtctr r0
    @L_801BF36C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801BF36C
    lis r3, fn_801BFF18@ha
    addi r3, r3, fn_801BFF18@l
    bl fn_800A880C
    lis r3, fn_801BFD10@ha
    addi r3, r3, fn_801BFD10@l
    bl fn_800A8850
    lis r7, lbl_80466BC0@ha
    lis r5, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    lis r6, lbl_80466BC0@ha
    addi r9, r7, lbl_80466BC0@l
    li r10, 0x0
    addi r7, r6, lbl_80466BC0@l
    li r8, 0x0
    lis r3, fn_801BFCB0@ha
    addi r5, r5, lbl_80466BC0@l
    li r6, 0x0
    addi r4, r4, lbl_80466BC0@l
    li r0, 0x0
    stw r10, 0x1d8(r9)
    addi r3, r3, fn_801BFCB0@l
    stw r8, 0x1dc(r7)
    stw r6, 0x1e0(r5)
    stw r0, 0x1e4(r4)
    bl fn_800B90A4
    lis r3, lbl_80466BC0@ha
    li r0, 0x0
    addi r3, r3, lbl_80466BC0@l
    stw r0, 0x1e8(r3)
    bl VIGetTvFormat
    cmplwi r3, 0x0
    li r6, 0x32
    bne @L_801BF408
    li r6, 0x3c
    @L_801BF408:
    lis r5, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    addi r5, r5, lbl_80466BC0@l
    li r0, 0x0
    addi r4, r4, lbl_80466BC0@l
    lis r3, lbl_80466BC0@ha
    stw r6, 0x1ec(r5)
    addi r3, r3, lbl_80466BC0@l
    stw r0, 0x1f0(r4)
    bl fn_800A94AC
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r3, 0x3c(r3)
    bl fn_800AA204
    bl fn_800AA068
    li r5, 0x0
    b @L_801BF470
    @L_801BF44C:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x2
    bne @L_801BF46C
    b @L_801BF47C
    @L_801BF46C:
    addi r5, r5, 0x1
    @L_801BF470:
    cmpwi r5, 0x3
    blt @L_801BF44C
    li r5, -0x1
    @L_801BF47C:
    mr r0, r5
    lis r4, lbl_80466BC0@ha
    mulli r0, r0, 0x60
    lis r3, lbl_80466BC0@ha
    addi r4, r4, lbl_80466BC0@l
    addi r3, r3, lbl_80466BC0@l
    add r4, r4, r0
    li r5, 0x0
    lwz r4, 0x58(r4)
    bl fn_801BFA1C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
