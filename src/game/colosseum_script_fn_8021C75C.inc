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
    li r3, 0x12
    mr r30, r0
    li r4, 0x0
    bl fn_801F025C
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    li r4, 0x1
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
    bne @L_8021C804
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021C8EC
@L_8021C804:
    mr r3, r31
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021C87C
    mr r3, r31
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021C87C
    mr r3, r31
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021C87C
    mr r3, r31
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021C87C
    mr r3, r31
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021C88C
@L_8021C87C:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021C898
@L_8021C88C:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021C898:
    mr r3, r31
    li r4, 0x8
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    li r4, 0x8
    li r5, 0x3
    bl fn_80201EB0
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021C8E0
    mr r3, r31
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8021C8E0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021C8EC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
