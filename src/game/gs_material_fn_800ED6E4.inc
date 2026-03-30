    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 24, 24
    bne @L_800ED730
    clrlwi. r0, r4, 24
    bne @L_800ED718
    lwz r8, 0x90(r3)
    li r5, 0x4000
    lwz r7, 0x8c(r3)
    li r6, 0x20
    b @L_800ED748
    @L_800ED718:
    lis r5, 0x1
    lwz r8, 0xa8(r3)
    lwz r7, 0xa4(r3)
    addi r5, r5, -0x8000
    li r6, 0x40
    b @L_800ED748
    @L_800ED730:
    clrlwi. r0, r4, 24
    bne @L_800ED748
    lwz r8, 0xc0(r3)
    li r5, 0x4000
    lwz r7, 0x8c(r3)
    li r6, 0x20
    @L_800ED748:
    cmpwi r7, 0x1
    beq @L_800ED770
    bge @L_800ED77C
    cmpwi r7, 0x0
    bge @L_800ED760
    b @L_800ED77C
    @L_800ED760:
    lwz r0, 0x0(r3)
    andc r0, r0, r6
    stw r0, 0x0(r3)
    b @L_800ED77C
    @L_800ED770:
    lwz r0, 0x0(r3)
    andc r0, r0, r5
    stw r0, 0x0(r3)
    @L_800ED77C:
    lwz r0, 0xdc(r3)
    cmplwi r0, 0x0
    beq @L_800ED7D4
    clrlwi. r0, r4, 24
    li r4, 0x0
    stw r4, 0x8(r1)
    bne @L_800ED7A0
    ori r0, r4, 0x1
    b @L_800ED7A4
    @L_800ED7A0:
    ori r0, r4, 0x2
    @L_800ED7A4:
    cmpwi r7, 0x1
    stw r0, 0x8(r1)
    bne @L_800ED7B8
    ori r0, r0, 0x4
    stw r0, 0x8(r1)
    @L_800ED7B8:
    stw r8, 0xc(r1)
    addi r4, r1, 0x8
    lwz r0, 0xe0(r3)
    stw r0, 0x10(r1)
    lwz r12, 0xdc(r3)
    mtctr r12
    bctrl
    @L_800ED7D4:
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
