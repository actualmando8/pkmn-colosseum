    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r28, r3
    li r29, -0x1
    lwz r0, 0x10(r3)
    lwz r31, lbl_8047B1B8(r13)
    rlwinm. r0, r0, 0, 1, 1
    lwz r27, 0x8(r3)
    beq @L_8017D6C0
    li r3, 0x0
    b @L_8017D7EC
    @L_8017D6C0:
    li r30, 0x0
    b @L_8017D6F4
    @L_8017D6C8:
    lwz r0, 0x0(r31)
    cmpw r0, r27
    bne @L_8017D6EC
    mr r29, r30
    mr r4, r31
    addi r3, r1, 0x8
    li r5, 0x8
    bl memcpy
    b @L_8017D6FC
    @L_8017D6EC:
    addi r31, r31, 0x8
    addi r30, r30, 0x1
    @L_8017D6F4:
    cmpwi r30, 0x64
    blt @L_8017D6C8
    @L_8017D6FC:
    cmpwi r29, 0x0
    blt @L_8017D770
    lwz r31, lbl_8047B1B8(r13)
    li r30, 0x0
    b @L_8017D740
    @L_8017D710:
    cmpw r30, r29
    blt @L_8017D73C
    addi r3, r30, 0x1
    slwi r0, r30, 3
    slwi r3, r3, 3
    add r4, r31, r3
    add r5, r31, r0
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x0(r5)
    stw r0, 0x4(r5)
    @L_8017D73C:
    addi r30, r30, 0x1
    @L_8017D740:
    lwz r3, lbl_8047B1BC(r13)
    subi r0, r3, 0x1
    cmpw r30, r0
    blt @L_8017D710
    lwz r4, 0x8(r1)
    slwi r0, r30, 3
    slwi r3, r30, 3
    stwx r4, r31, r0
    addi r0, r3, 0x4
    lwz r3, 0xc(r1)
    stwx r3, r31, r0
    b @L_8017D7E8
    @L_8017D770:
    lwz r0, lbl_8047B1BC(r13)
    lwz r31, lbl_8047B1B8(r13)
    cmpwi r0, 0x64
    bne @L_8017D7C4
    li r30, 0x0
    b @L_8017D7B0
    @L_8017D788:
    addi r3, r30, 0x1
    slwi r0, r30, 3
    slwi r3, r3, 3
    addi r30, r30, 0x1
    add r4, r31, r3
    add r5, r31, r0
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x0(r5)
    stw r0, 0x4(r5)
    @L_8017D7B0:
    cmpwi r30, 0x63
    blt @L_8017D788
    lwz r3, lbl_8047B1BC(r13)
    subi r0, r3, 0x1
    stw r0, lbl_8047B1BC(r13)
    @L_8017D7C4:
    lwz r3, lbl_8047B1BC(r13)
    li r0, 0x0
    lwz r5, lbl_8047B1B8(r13)
    addi r4, r3, 0x1
    slwi r3, r3, 3
    stw r4, lbl_8047B1BC(r13)
    add r31, r5, r3
    stw r27, 0x0(r31)
    stw r0, 0x4(r31)
    @L_8017D7E8:
    mr r3, r31
    @L_8017D7EC:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
