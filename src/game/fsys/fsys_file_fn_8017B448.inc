    nofralloc
    stwu r1, -0x20(r1)
    stmw r27, 0xc(r1)
    li r30, 0x0
    lwz r31, lbl_8047B1B4(r13)
    b @L_8017B494
    @L_8017B45C:
    lwz r0, 0x48(r31)
    cmpwi r0, 0x0
    beq @L_8017B48C
    lwz r0, 0xf8(r31)
    cmplw r0, r3
    bne @L_8017B48C
    lwz r0, 0x5c(r31)
    cmpwi r0, 0x0
    beq @L_8017B48C
    lwz r28, 0x40(r31)
    lwz r29, 0xc(r28)
    b @L_8017B4A8
    @L_8017B48C:
    addi r31, r31, 0x140
    addi r30, r30, 0x1
    @L_8017B494:
    lis r4, lbl_80453FEC@ha
    lwz r0, lbl_80453FEC@l(r4)
    cmplw r30, r0
    blt @L_8017B45C
    li r29, -0x1
    @L_8017B4A8:
    mr r27, r29
    mr r3, r27
    lmw r27, 0xc(r1)
    addi r1, r1, 0x20
    blr
