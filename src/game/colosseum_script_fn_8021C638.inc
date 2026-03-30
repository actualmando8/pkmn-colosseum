    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r5
    mr r30, r3
    mr r3, r29
    bl fn_80207BF4
    mr r31, r3
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021C674
    li r3, 0x1
    b @L_8021C6E0
@L_8021C674:
    mr r3, r30
    li r4, 0xb
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021C6DC
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x2b
    beq @L_8021C6DC
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    cmplw r30, r29
    bne @L_8021C6C8
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021C6D4
@L_8021C6C8:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021C6D4:
    li r3, 0x0
    b @L_8021C6E0
@L_8021C6DC:
    li r3, 0x1
@L_8021C6E0:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
