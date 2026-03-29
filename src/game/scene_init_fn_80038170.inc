    nofralloc
    stwu r1, -0x20(r1)
    lis r5, lbl_803A654C@ha
    li r0, 0x0
    addi r5, r5, lbl_803A654C@l
    lfs f1, lbl_8047BA58(r2)
    lfs f0, 0x10(r5)
    fcmpu cr0, f1, f0
    beq @L_80038194
    b @L_800381D4
    @L_80038194:
    addi r5, r5, 0x18
    lfs f0, 0x10(r5)
    fcmpu cr0, f1, f0
    beq @L_800381A8
    b @L_800381D4
    @L_800381A8:
    addi r5, r5, 0x18
    lfs f0, 0x10(r5)
    fcmpu cr0, f1, f0
    beq @L_800381BC
    b @L_800381D4
    @L_800381BC:
    addi r5, r5, 0x18
    lfs f0, 0x10(r5)
    fcmpu cr0, f1, f0
    beq @L_800381D0
    b @L_800381D4
    @L_800381D0:
    li r0, 0x1
    @L_800381D4:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_800381F4
    lbz r0, 0x4(r4)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_80038204
    @L_800381F4:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    @L_80038204:
    lbz r5, 0x95(r3)
    lis r3, lbl_802E5288@ha
    addi r3, r3, lbl_802E5288@l
    extsb r0, r5
    stb r5, lbl_8047A47C(r13)
    slwi r0, r0, 3
    lfsx f0, r3, r0
    add r3, r3, r0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r4)
    lfs f0, 0x4(r3)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r4)
    addi r1, r1, 0x20
    blr
