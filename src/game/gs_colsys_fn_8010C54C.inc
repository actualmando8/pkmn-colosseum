    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r26, r3
    li r31, 0x0
    li r29, 0x0
    stb r4, 0x8(r1)
    b @L_8010C628
@L_8010C570:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_8010C624
    lbz r0, 0x8(r1)
    cmplwi r0, 0x1
    bne @L_8010C5D8
    rlwinm r3, r26, 0, 16, 31
    rlwinm r4, r29, 0, 16, 31
    bl fn_8010C508
    rlwinm r0, r3, 0, 16, 31
    mr r28, r3
    cmplwi r0, 0x43
    mr r30, r28
    bne @L_8010C5B0
    subi r31, r31, 0xa
    b @L_8010C624
@L_8010C5B0:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x42
    bne @L_8010C5C4
    subi r31, r31, 0xa
    b @L_8010C624
@L_8010C5C4:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x41
    bne @L_8010C624
    addi r31, r31, 0xa
    b @L_8010C624
@L_8010C5D8:
    rlwinm r3, r29, 0, 16, 31
    rlwinm r4, r26, 0, 16, 31
    bl fn_8010C508
    rlwinm r0, r3, 0, 16, 31
    mr r27, r3
    cmplwi r0, 0x43
    mr r30, r27
    bne @L_8010C600
    addi r31, r31, 0xa
    b @L_8010C624
@L_8010C600:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x42
    bne @L_8010C614
    addi r31, r31, 0xa
    b @L_8010C624
@L_8010C614:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x41
    bne @L_8010C624
    subi r31, r31, 0xa
@L_8010C624:
    addi r29, r29, 0x1
@L_8010C628:
    lwz r0, lbl_80478B38(r13)
    rlwinm r3, r29, 0, 16, 31
    cmplw r3, r0
    blt @L_8010C570
    mr r3, r31
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

