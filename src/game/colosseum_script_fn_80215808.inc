    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 16, 31
    cmpw r0, r31
    bgt @L_80215898
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_802158BC
@L_80215898:
    mr r3, r30
    subf r7, r31, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_802158BC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
