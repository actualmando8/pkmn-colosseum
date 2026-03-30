    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
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
    mr r31, r3
    bl fn_800E0C54
    rlwinm r29, r3, 0, 24, 31
    cmplwi r29, 0x66
    bge @L_80217A0C
    mr r3, r30
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    li r7, 0x28
    bl fn_8011BBD8
    b @L_80217A7C
@L_80217A0C:
    cmplwi r29, 0xb2
    bge @L_80217A30
    mr r3, r30
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    li r7, 0x50
    bl fn_8011BBD8
    b @L_80217A7C
@L_80217A30:
    cmplwi r29, 0xcc
    bge @L_80217A54
    mr r3, r30
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    li r7, 0x78
    bl fn_8011BBD8
    b @L_80217A7C
@L_80217A54:
    mr r3, r31
    li r4, 0x4
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r30
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
@L_80217A7C:
    cmplwi r29, 0xcc
    bge @L_80217A94
    lis r3, lbl_80375F98@ha
    addi r0, r3, lbl_80375F98@l
    stw r0, lbl_8047B610(r13)
    b @L_80217AD0
@L_80217A94:
    mr r3, r31
    bl fn_80201704
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80217AB8
    lis r3, lbl_80377AD9@ha
    addi r0, r3, lbl_80377AD9@l
    stw r0, lbl_8047B610(r13)
    b @L_80217AD0
@L_80217AB8:
    mr r3, r30
    li r4, 0x43
    bl fn_80209960
    lis r3, lbl_80377AB8@ha
    addi r0, r3, lbl_80377AB8@l
    stw r0, lbl_8047B610(r13)
@L_80217AD0:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
