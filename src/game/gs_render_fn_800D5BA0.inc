    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r7, lbl_8047AA80(r13)
    lbz r0, 0x47e(r7)
    cmplwi r0, 0x0
    bne @L_800D5BE4
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D5BE4
    mr r5, r3
    mr r6, r4
    li r3, 0x18
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5C08
    @L_800D5BE4:
    slwi r0, r3, 2
    lis r5, fn_800D737C@ha
    addi r6, r5, fn_800D737C@l
    add r5, r7, r0
    stw r6, 0x4e0(r5)
    mulli r0, r3, 0xc
    lwz r3, lbl_8047AA80(r13)
    add r3, r3, r0
    stw r4, 0x4f0(r3)
    @L_800D5C08:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
