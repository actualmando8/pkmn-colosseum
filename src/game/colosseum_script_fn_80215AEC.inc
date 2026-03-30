    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x1(r3)
    cmpwi r0, 0x1
    beq @L_80215BBC
    bge @L_80215C5C
    cmpwi r0, 0x0
    bge @L_80215B1C
    b @L_80215C5C
@L_80215B1C:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x35
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_80215B8C
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x2(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80215C5C
@L_80215B8C:
    mr r3, r31
    li r4, 0x35
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    mr r5, r30
    li r4, 0x35
    bl fn_80201764
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
    b @L_80215C5C
@L_80215BBC:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r30, r0
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x35
    mr r31, r3
    bl fn_80201890
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    li r4, 0x2
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r30
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    bl fn_80201704
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215C50
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x2(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80215C5C
@L_80215C50:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
@L_80215C5C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
