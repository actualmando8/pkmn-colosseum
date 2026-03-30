    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x0
    bne @L_800D5F70
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D5F70
    li r3, 0x11
    li r4, 0xd
    creqv 6, 6, 6
    bl fn_800D4F98
    b @L_800D5F94
    @L_800D5F70:
    lis r3, fn_800D7468@ha
    addi r0, r3, fn_800D7468@l
    stw r0, 0x4c4(r4)
    lwz r3, lbl_8047AA80(r13)
    stfs f1, 0x4d4(r3)
    lwz r3, lbl_8047AA80(r13)
    stfs f2, 0x4d8(r3)
    lwz r3, lbl_8047AA80(r13)
    stfs f3, 0x4dc(r3)
    @L_800D5F94:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
