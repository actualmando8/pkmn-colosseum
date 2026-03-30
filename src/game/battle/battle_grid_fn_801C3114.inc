    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, lbl_80466DE8@ha
    li r4, 0x0
    stw r0, 0x24(r1)
    addi r3, r3, lbl_80466DE8@l
    li r5, 0x44
    stmw r27, 0xc(r1)
    bl memset
    lbz r0, lbl_8047B39A(r13)
    li r30, 0x0
    li r5, -0x1
    li r4, 0xc8
    cmplwi r0, 0x0
    lis r3, lbl_80466DE8@ha
    stw r5, lbl_80478CAC(r13)
    addi r27, r3, lbl_80466DE8@l
    stw r4, lbl_80478CA8(r13)
    stb r30, lbl_8047B399(r13)
    beq @L_801C31D8
    mr r28, r30
    b @L_801C31C4
    @L_801C316C:
    clrlwi r0, r30, 16
    la r4, lbl_8047B39C(r13)
    lwz r3, 0x0(r27)
    addi r30, r30, 0x1
    lbzx r4, r4, r0
    bl fn_801DA4E8
    li r29, 0x0
    la r31, lbl_8047B39C(r13)
    b @L_801C31B0
    @L_801C3190:
    clrlslwi r3, r29, 16, 2
    clrlwi r0, r30, 16
    addi r3, r3, 0x4
    lbzx r4, r31, r0
    lwzx r3, r27, r3
    addi r30, r30, 0x1
    bl fn_801DA4E8
    addi r29, r29, 0x1
    @L_801C31B0:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801C3190
    addi r28, r28, 0x1
    addi r27, r27, 0x10
    @L_801C31C4:
    clrlwi r0, r28, 16
    cmplwi r0, 0x4
    blt @L_801C316C
    li r0, 0x0
    stb r0, lbl_8047B39A(r13)
    @L_801C31D8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
