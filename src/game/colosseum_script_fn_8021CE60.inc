    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    mr r30, r3
    lbz r0, 0x5(r4)
    lwz r28, 0x1(r4)
    cmplwi r0, 0x11
    bne @L_8021CED8
    mr r30, r31
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
@L_8021CED8:
    mr r3, r30
    li r4, 0x2
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r29
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r30
    bl fn_80201704
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021CF1C
    mr r0, r28
    b @L_8021CF24
@L_8021CF1C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
@L_8021CF24:
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
