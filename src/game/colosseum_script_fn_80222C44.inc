    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    lwz r5, lbl_8047B610(r13)
    lwz r0, 0x1(r5)
    rlwinm r27, r0, 0, 24, 31
    bl fn_801F025C
    mr r29, r3
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r28, r0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x5
    li r4, 0x0
    bl fn_801F025C
    mr r4, r30
    mr r5, r31
    bl fn_801F7174
    cmplwi r3, 0x0
    bne @L_80222CD8
    li r3, 0x0
    li r4, 0x2
    bl fn_801F3984
@L_80222CD8:
    li r3, 0x4
    li r4, 0x0
    bl fn_801F025C
    mr r4, r30
    mr r5, r31
    bl fn_801F7174
    cmplwi r3, 0x0
    bne @L_80222D04
    li r3, 0x0
    li r4, 0x3
    bl fn_801F3984
@L_80222D04:
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80222DDC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x34
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 24, 31
    bl fn_80077AF4
    mr r31, r3
    mr r4, r28
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r30, 0x1
    rlwinm r3, r3, 0, 16, 31
    bne @L_80222D6C
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222D6C
    cmplwi r3, 0x7
    bne @L_80222D6C
    li r0, 0x1
    b @L_80222D70
@L_80222D6C:
    li r0, 0x0
@L_80222D70:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222ED0
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7
    bne @L_80222ED0
    li r3, 0x0
    li r4, 0x0
    bl fn_801F3984
    mr r4, r29
    li r3, 0x2
    bl fn_801F025C
    mr r31, r3
    li r3, 0x4
    li r4, 0x0
    bl fn_801F025C
    cmplw r3, r31
    bne @L_80222DCC
    li r3, 0x0
    li r4, 0x3
    bl fn_801F3984
    b @L_80222ED0
@L_80222DCC:
    li r3, 0x0
    li r4, 0x2
    bl fn_801F3984
    b @L_80222ED0
@L_80222DDC:
    cmplwi r27, 0x1
    bne @L_80222ED0
    li r3, 0x0
    bl fn_801F32B0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80222EB8
    bl fn_801EF95C
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80222E14
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80222EDC
@L_80222E14:
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222E30
    bl fn_80265A6C
    bl fn_802659F8
@L_80222E30:
    bl fn_801C3430
    li r3, 0x0
    bl fn_801EF8F4
    mr r31, r3
    li r3, -0x1
    bl fn_80261AD0
    lis r4, -32734
    li r3, 0x0
    addi r4, r4, 12476
    li r5, 0x0
    li r6, 0x1
    bl fn_801F2B5C
    li r3, 0x0
    bl fn_80261954
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222E7C
    bl fn_801C2D68
    b @L_80222E80
@L_80222E7C:
    bl fn_801C2D5C
@L_80222E80:
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222E9C
    bl fn_802659A4
    bl fn_802658C8
@L_80222E9C:
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80222EB8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80222EDC
@L_80222EB8:
    lis r4, -32734
    li r3, 0x0
    addi r4, r4, 12016
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
@L_80222ED0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80222EDC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
