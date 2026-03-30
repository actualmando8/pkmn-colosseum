    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f2
    stfd f30, 0x10(r1)
    fmr f30, f1
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x47e(r3)
    cmplwi r0, 0x0
    bne @L_800D6368
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D6368
    li r3, 0xa
    li r4, 0xc
    creqv 6, 6, 6
    bl fn_800D4F98
    b @L_800D6398
    @L_800D6368:
    bl fn_800D6B00
    lis r4, fn_800D7524@ha
    lwz r3, lbl_8047AA80(r13)
    addi r4, r4, fn_800D7524@l
    li r0, 0x1
    stw r4, 0x4a8(r3)
    lwz r3, lbl_8047AA80(r13)
    stfs f30, 0x4b8(r3)
    lwz r3, lbl_8047AA80(r13)
    stfs f31, 0x4bc(r3)
    lwz r3, lbl_8047AA80(r13)
    stb r0, 0x49f(r3)
    @L_800D6398:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lfd f30, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
