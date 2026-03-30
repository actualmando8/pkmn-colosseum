    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lhz r0, lbl_8047AAF8(r13)
    cmplwi r0, 0x0
    bne @L_800DD298
    li r3, 0x0
    b @L_800DD370
    @L_800DD298:
    lwz r0, lbl_8047AB08(r13)
    cmplw r31, r0
    ble @L_800DD2AC
    li r3, 0x0
    b @L_800DD370
    @L_800DD2AC:
    lhz r3, lbl_8047AAFA(r13)
    bl fn_800E27B0
    cmplwi r31, 0x0
    li r6, 0x0
    li r7, 0x0
    ble @L_800DD35C
    cmplwi r31, 0x8
    subi r4, r31, 0x8
    ble @L_800DD334
    addi r0, r4, 0x7
    mr r5, r3
    srwi r0, r0, 3
    mtctr r0
    cmplwi r4, 0x0
    ble @L_800DD334
    @L_800DD2E8:
    lhz r4, 0x0(r5)
    addi r7, r7, 0x8
    lhz r0, 0x2(r5)
    add r6, r6, r4
    lhz r4, 0x4(r5)
    add r6, r6, r0
    lhz r0, 0x6(r5)
    add r6, r6, r4
    lhz r4, 0x8(r5)
    add r6, r6, r0
    lhz r0, 0xa(r5)
    add r6, r6, r4
    lhz r4, 0xc(r5)
    add r6, r6, r0
    lhz r0, 0xe(r5)
    add r6, r6, r4
    addi r5, r5, 0x10
    add r6, r6, r0
    bdnz @L_800DD2E8
    @L_800DD334:
    slwi r4, r7, 1
    subf r0, r7, r31
    add r3, r3, r4
    mtctr r0
    cmplw r7, r31
    bge @L_800DD35C
    @L_800DD34C:
    lhz r0, 0x0(r3)
    addi r3, r3, 0x2
    add r6, r6, r0
    bdnz @L_800DD34C
    @L_800DD35C:
    lwz r0, lbl_8047AAFC(r13)
    lhz r3, lbl_8047AAFA(r13)
    add r31, r0, r6
    bl fn_800E24B0
    mr r3, r31
    @L_800DD370:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
