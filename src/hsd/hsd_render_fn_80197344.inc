    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    stw r30, 0x8(r1)
    beq @L_801973E8
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    rlwinm. r0, r0, 27, 5, 31
    beq @L_80197380
    mr r3, r31
    bl fn_80197784
    b @L_801973E8
@L_80197380:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_801973E8
    lwz r0, lbl_8047B240(r13)
    cmplwi r0, 0x0
    beq @L_801973E8
    lwz r30, 0x18(r31)
    b @L_801973E0
@L_801973A0:
    lwz r0, 0x4(r30)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801973D0
    lwz r0, 0x4(r30)
    mr r6, r31
    lwz r4, 0x4(r30)
    li r3, 0x0
    lwz r12, lbl_8047B240(r13)
    rlwinm r5, r0, 26, 8, 31
    rlwinm r4, r4, 0, 26, 31
    mtctr r12
    bctrl
@L_801973D0:
    lwz r0, 0x4(r30)
    rlwinm r0, r0, 0, 1, 31
    stw r0, 0x4(r30)
    lwz r30, 0x0(r30)
@L_801973E0:
    cmplwi r30, 0x0
    bne @L_801973A0
@L_801973E8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
