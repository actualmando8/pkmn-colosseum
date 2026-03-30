    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x14
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r30, r0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r30
    bl fn_801F0134
    mr r31, r3
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021760C
    lbz r0, lbl_8047B649(r13)
    stb r0, lbl_8047B648(r13)
    b @L_802177C4
@L_8021760C:
    li r29, 0x0
    li r28, 0x0
    b @L_8021774C
@L_80217618:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x5d
    bl fn_801F54A4
    mr. r29, r3
    beq @L_80217740
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80217740
    cmplw r30, r29
    beq @L_80217740
    mr r3, r30
    mr r4, r29
    bl fn_80202B88
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80217740
    mr r3, r29
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r27, r3
    beq @L_80217740
    bl fn_801F1170
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80217740
    mr r3, r27
    bl fn_801F0898
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x13
    bne @L_80217740
    mr r3, r29
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r29
    mr r27, r0
    bl fn_80205184
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xe4
    bne @L_80217740
    mr r3, r27
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r31, 0, 16, 31
    cmplw r0, r3
    bne @L_80217740
    mr r3, r29
    li r4, 0x8
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80217740
    mr r3, r29
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80217740
    mr r3, r29
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    beq @L_80217740
    mr r28, r29
    b @L_8021775C
@L_80217740:
    lbz r3, lbl_8047B648(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B648(r13)
@L_8021774C:
    lbz r6, lbl_8047B648(r13)
    lbz r0, lbl_8047B649(r13)
    cmplw r6, r0
    blt @L_80217618
@L_8021775C:
    cmplwi r28, 0x0
    beq @L_802177C4
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_80379F58@ha
    addi r3, r3, lbl_80379F58@l
    li r4, 0x1
    addis r3, r3, 0x1
    rlwinm r0, r0, 0, 22, 20
    stb r4, 0x6002(r3)
    mr r3, r29
    li r4, 0x0
    li r5, 0x112
    stw r0, lbl_8047B618(r13)
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_802177D0
@L_802177C4:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_802177D0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
