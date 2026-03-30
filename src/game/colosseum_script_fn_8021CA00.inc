    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x0
    mr r30, r0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    bl fn_801F0134
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r30
    mr r31, r0
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021CA9C
    mr r3, r31
    li r4, 0x1c
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_8021CAD0
@L_8021CA9C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    li r5, 0x1
    la r4, lbl_80478D78(r13)
    addi r0, r3, 0x1
    stb r5, 0x5(r4)
    stw r0, lbl_8047B610(r13)
    b @L_8021CB44
@L_8021CAD0:
    mr r3, r31
    li r4, 0xc
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021CB1C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    li r5, 0x2
    la r4, lbl_80478D78(r13)
    addi r0, r3, 0x1
    stb r5, 0x5(r4)
    stw r0, lbl_8047B610(r13)
    b @L_8021CB44
@L_8021CB1C:
    mr r3, r31
    mr r5, r29
    li r4, 0x1c
    bl fn_8020248C
    lwz r3, lbl_8047B610(r13)
    li r5, 0x0
    la r4, lbl_80478D78(r13)
    addi r0, r3, 0x1
    stb r5, 0x5(r4)
    stw r0, lbl_8047B610(r13)
@L_8021CB44:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
