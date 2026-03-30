    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x12
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    mr r30, r3
    bl fn_80207BF4
    lwz r5, lbl_8047B610(r13)
    lis r4, -32734
    mr r31, r3
    li r3, 0x0
    lwz r29, 0x1(r5)
    addi r4, r4, -14792
    mr r5, r30
    li r6, 0x1
    bl fn_801F37B0
    rlwinm r0, r3, 0, 24, 31
    subfic r3, r0, 0x1
    subic r0, r3, 0x1
    subfe r0, r0, r3
    rlwinm. r0, r0, 0, 24, 31
    beq @L_8021C5F0
    stw r29, lbl_8047B610(r13)
    b @L_8021C624
@L_8021C5F0:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0xf
    beq @L_8021C604
    cmplwi r0, 0x48
    bne @L_8021C618
@L_8021C604:
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    stw r29, lbl_8047B610(r13)
    b @L_8021C624
@L_8021C618:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021C624:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
