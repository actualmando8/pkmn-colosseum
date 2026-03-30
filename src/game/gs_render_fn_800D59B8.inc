    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lbz r0, 0x47e(r5)
    cmplwi r0, 0x0
    bne @L_800D59F8
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800D59F8
    mr r5, r3
    li r3, 0x1c
    li r4, 0xf
    creqv 6, 6, 6
    bl fn_800D4F98
    b @L_800D5A28
    @L_800D59F8:
    slwi r0, r3, 2
    lis r4, fn_800D7304@ha
    addi r4, r4, fn_800D7304@l
    slwi r6, r3, 4
    add r3, r5, r0
    stw r4, 0x500(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r6
    stfs f1, 0x528(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r6
    stfs f2, 0x52c(r3)
    @L_800D5A28:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
