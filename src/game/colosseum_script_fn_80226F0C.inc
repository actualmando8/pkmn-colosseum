    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    mr r30, r3
    bl fn_80205184
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_80226F74
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x90
    beq @L_80226F74
    cmplwi r0, 0xa4
    bne @L_80226F98
@L_80226F74:
    lwz r3, lbl_8047B610(r13)
    lbz r29, 0x1(r3)
@L_80226F7C:
    mr r3, r29
    bl fn_801DA5C4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80226F98
    bl fn_800F0308
    b @L_80226F7C
@L_80226F98:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_80226FAC
    cmplwi r0, 0x6
    bne @L_80226FB4
@L_80226FAC:
    li r3, 0x0
    bl fn_80261E7C
@L_80226FB4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
