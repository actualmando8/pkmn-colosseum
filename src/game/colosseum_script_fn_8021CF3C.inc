    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    lwz r4, lbl_8047B610(r13)
    mr r31, r3
    lbz r3, lbl_8047B648(r13)
    li r29, 0x0
    lwz r28, 0x1(r4)
    addi r0, r3, 0x1
    stb r0, lbl_8047B648(r13)
    b @L_8021CFB8
@L_8021CF78:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x5d
    bl fn_801F54A4
    mr. r30, r3
    beq @L_8021CFAC
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021CFAC
    cmplw r31, r30
    beq @L_8021CFAC
    mr r29, r30
    b @L_8021CFC8
@L_8021CFAC:
    lbz r3, lbl_8047B648(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B648(r13)
@L_8021CFB8:
    lbz r6, lbl_8047B648(r13)
    lbz r0, lbl_8047B649(r13)
    cmplw r6, r0
    blt @L_8021CF78
@L_8021CFC8:
    cmplwi r29, 0x0
    beq @L_8021CFF0
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    stw r28, lbl_8047B610(r13)
    b @L_8021CFFC
@L_8021CFF0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021CFFC:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
