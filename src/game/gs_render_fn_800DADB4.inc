    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x47e(r3)
    cmplwi r0, 0x0
    bne @L_800DADDC
    li r3, 0x0
    b @L_800DAF4C
    @L_800DADDC:
    lwz r31, 0x480(r3)
    lwz r5, 0x484(r3)
    lwz r3, 0x4(r31)
    lwz r0, 0x8(r31)
    addi r4, r5, 0x1f
    clrrwi r7, r4, 5
    add r0, r3, r0
    cmplw r7, r0
    bgt @L_800DAE0C
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    beq @L_800DAE24
    @L_800DAE0C:
    lhz r3, 0x2(r31)
    bl fn_800E24B0
    lhz r3, 0x2(r31)
    bl fn_800E209C
    li r3, 0x0
    b @L_800DAF4C
    @L_800DAE24:
    subf. r0, r5, r7
    li r6, 0x0
    mr r3, r0
    beq @L_800DAF08
    srwi. r0, r0, 3
    mtctr r0
    beq @L_800DAEEC
    @L_800DAE40:
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    bdnz @L_800DAE40
    andi. r3, r3, 0x7
    beq @L_800DAF08
    @L_800DAEEC:
    mtctr r3
    @L_800DAEF0:
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x484(r5)
    addi r0, r4, 0x1
    stw r0, 0x484(r5)
    stb r6, 0x0(r4)
    bdnz @L_800DAEF0
    @L_800DAF08:
    lwz r0, 0x4(r31)
    subf r0, r0, r7
    stw r0, 0x8(r31)
    lhz r3, 0x2(r31)
    lwz r4, 0x8(r31)
    bl fn_800E2AF8
    lwz r3, 0x4(r31)
    lwz r4, 0x8(r31)
    bl DCFlushRange
    lwz r4, lbl_8047AA80(r13)
    li r0, 0x0
    mr r3, r31
    stb r0, 0x47e(r4)
    lwz r4, lbl_8047AA80(r13)
    stw r0, 0x480(r4)
    lwz r4, lbl_8047AA80(r13)
    stw r0, 0x484(r4)
    @L_800DAF4C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
