    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r31, r0
    mr r4, r31
    bl fn_801F025C
    mr r30, r3
    mr r5, r31
    li r3, 0x0
    li r4, 0x1
    li r6, 0x1
    bl fn_801F2654
    mr r31, r3
    mr r3, r30
    li r4, 0x48
    bl fn_801F6E44
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_8021CBE4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021CC3C
@L_8021CBE4:
    mr r3, r30
    li r4, 0x48
    li r5, 0x0
    bl fn_801F6DF0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x19
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8021CC30
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8021CC30
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021CC3C
@L_8021CC30:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021CC3C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
