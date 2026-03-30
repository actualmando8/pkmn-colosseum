    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x10c
    li r6, 0x0
    cmpwi r0, 0x0
    bne @L_80106018
    b @L_8010606C
@L_80106018:
    rlwinm r0, r5, 0, 24, 31
    li r4, 0x1
    cmplwi r0, 0x0
    stb r4, lbl_8047AD10(r13)
    beq @L_80106034
    ori r0, r6, 0x1
    rlwinm r6, r0, 0, 24, 31
@L_80106034:
    li r4, 0x0
    ori r0, r6, 0x2
    stw r4, 0x8(r1)
    mr r9, r3
    rlwinm r10, r0, 0, 24, 31
    li r3, 0x10c
    li r4, -0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
    mr r6, r3
@L_8010606C:
    mr r3, r6
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

