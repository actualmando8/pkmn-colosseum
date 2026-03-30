    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r29, r3
    mr r30, r4
    mr r31, r5
    bne @L_8019C290
    lis r3, lbl_804653A8@ha
    addi r0, r3, lbl_804653A8@l
    mr r29, r0
    @L_8019C290:
    lis r3, 0x4470
    subi r3, r3, 0x79a9
    mulhwu r3, r3, r30
    subf r0, r3, r30
    srwi r0, r0, 1
    add r0, r0, r3
    srwi r0, r0, 6
    mulli r0, r0, 0x65
    subf r0, r0, r30
    mr r27, r0
    slwi r0, r0, 2
    lwzx r4, r29, r0
    b @L_8019C2D4
    @L_8019C2C4:
    lwz r0, 0x4(r4)
    cmplw r0, r30
    beq @L_8019C2DC
    lwz r4, 0x0(r4)
    @L_8019C2D4:
    cmplwi r4, 0x0
    bne @L_8019C2C4
    @L_8019C2DC:
    cmplwi r4, 0x0
    beq @L_8019C2F0
    stw r30, 0x4(r4)
    stw r31, 0x8(r4)
    b @L_8019C344
    @L_8019C2F0:
    lis r3, lbl_8046553C@ha
    addi r3, r3, lbl_8046553C@l
    bl fn_801AA4CC
    mr. r28, r3
    bne @L_8019C314
    la r3, lbl_8047DAA0(r2)
    li r4, 0x43
    la r5, lbl_8047DAA8(r2)
    bl fn_80196E10
    @L_8019C314:
    mr r3, r28
    li r4, 0x0
    li r5, 0xc
    bl memset
    stw r30, 0x4(r28)
    slwi r3, r27, 2
    slwi r0, r27, 2
    mr r4, r28
    stw r31, 0x8(r28)
    lwzx r3, r29, r3
    stw r3, 0x0(r28)
    stwx r4, r29, r0
    @L_8019C344:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
