    nofralloc
    stwu r1, -0x10(r1)
    lwz r6, 0x60(r3)
    lis r3, lbl_80266DD8@ha
    addi r3, r3, lbl_80266DD8@l
    lwz r0, 0x1c(r6)
    slwi r0, r0, 4
    add r3, r3, r0
    lwz r5, 0x4(r3)
    cmpwi r5, 0xa
    beq @L_80026894
    li r0, 0x0
    stb r0, 0x67(r4)
    b @L_800268E4
    @L_80026894:
    lwz r3, 0x34(r6)
    lwz r0, 0x0(r3)
    cmpw r0, r5
    blt @L_800268A8
    subi r0, r5, 0x1
    @L_800268A8:
    lwz r3, 0x40(r6)
    mulli r5, r0, 0x1a
    lfs f2, lbl_8047B938(r2)
    lwz r0, 0x0(r3)
    lfs f0, lbl_8047B934(r2)
    add r5, r5, r0
    extsh r0, r5
    sth r0, 0x50(r4)
    lwz r3, 0x30(r6)
    lfs f1, 0x0(r3)
    fnmsubs f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x67(r4)
    @L_800268E4:
    li r3, 0x0
    addi r1, r1, 0x10
    blr
