    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    rlwinm r30, r5, 0, 16, 31
    mr r3, r30
    bl fn_8010C3FC
    mr r31, r3
    mr r3, r30
    bl fn_8010C3FC
    lwz r6, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r5, r6
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010B628
@L_8010B60C:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010B61C
    b @L_8010B62C
@L_8010B61C:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010B60C
@L_8010B628:
    li r4, -0x1
@L_8010B62C:
    cmpwi r4, 0x0
    bge @L_8010B64C
    cmplwi r31, 0x0
    beq @L_8010B700
    mr r4, r31
    li r3, 0x5c0
    bl fn_800F9210
    b @L_8010B700
@L_8010B64C:
    rlwinm r0, r4, 4, 0, 27
    mr r4, r31
    add r30, r6, r0
    li r3, 0x5c0
    bl fn_800F9318
    li r0, 0x0
    mr r4, r3
    stb r0, 0x7(r3)
    li r5, 0x6ec0
    lwz r0, 0x28(r3)
    subf r0, r3, r0
    stw r0, 0x28(r3)
    lbz r0, 0x7(r30)
    lwz r3, lbl_8047AD54(r13)
    extsb r0, r0
    rlwinm r0, r0, 3, 0, 28
    add r3, r3, r0
    lwz r3, 0x4(r3)
    bl memcpy
    mr r4, r31
    li r3, 0x5c0
    bl fn_800F9210
    lbz r0, 0x7(r30)
    lwz r3, lbl_8047AD54(r13)
    extsb r0, r0
    rlwinm r0, r0, 3, 0, 28
    add r3, r3, r0
    lwz r3, 0x4(r3)
    bl fn_800EFD3C
    lis r4, fn_8010C364@ha
    mr r5, r31
    addi r6, r4, fn_8010C364@l
    mr r31, r3
    li r4, 0x5c0
    bl fn_800F9378
    mr r3, r31
    li r4, 0x6ec0
    bl DCFlushRange
    bl fn_800BB29C
    li r0, 0x2
    li r3, 0x0
    stb r0, 0x6(r30)
    lwz r4, 0x8(r30)
    lwz r5, 0xc(r30)
    bl fn_8010B16C
@L_8010B700:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

