    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r4, r0
    bl fn_801F025C
    li r4, 0x4b
    mr r31, r3
    bl fn_801F6E44
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_80217960
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021797C
@L_80217960:
    mr r3, r31
    li r4, 0x4b
    li r5, 0x0
    bl fn_801F6DF0
    li r0, 0x5
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021797C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
