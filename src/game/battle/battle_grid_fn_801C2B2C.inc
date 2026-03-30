    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, lbl_80466DE8@ha
    stw r0, 0x24(r1)
    addi r3, r3, lbl_80466DE8@l
    stmw r27, 0xc(r1)
    li r27, 0x0
    mr r30, r3
    lbz r0, lbl_8047B39A(r13)
    cmplwi r0, 0x0
    beq @L_801C2BCC
    li r29, 0x0
    b @L_801C2BB8
    @L_801C2B60:
    clrlwi r0, r27, 16
    la r4, lbl_8047B39C(r13)
    lwz r3, 0x0(r30)
    addi r27, r27, 0x1
    lbzx r4, r4, r0
    bl fn_801DA4E8
    li r28, 0x0
    la r31, lbl_8047B39C(r13)
    b @L_801C2BA4
    @L_801C2B84:
    clrlslwi r3, r28, 16, 2
    clrlwi r0, r27, 16
    addi r3, r3, 0x4
    lbzx r4, r31, r0
    lwzx r3, r30, r3
    addi r27, r27, 0x1
    bl fn_801DA4E8
    addi r28, r28, 0x1
    @L_801C2BA4:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801C2B84
    addi r29, r29, 0x1
    addi r30, r30, 0x10
    @L_801C2BB8:
    clrlwi r0, r29, 16
    cmplwi r0, 0x4
    blt @L_801C2B60
    li r0, 0x0
    stb r0, lbl_8047B39A(r13)
    @L_801C2BCC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
