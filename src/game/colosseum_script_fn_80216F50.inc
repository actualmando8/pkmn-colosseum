    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    li r4, 0x8
    li r0, 0x0
    stb r4, lbl_8047B649(r13)
    mr r31, r3
    li r29, 0x0
    stb r0, lbl_8047B648(r13)
    b @L_80216FC8
@L_80216F88:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x5d
    bl fn_801F54A4
    mr. r30, r3
    beq @L_80216FBC
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80216FBC
    cmplw r31, r30
    beq @L_80216FBC
    mr r29, r30
    b @L_80216FD8
@L_80216FBC:
    lbz r3, lbl_8047B648(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B648(r13)
@L_80216FC8:
    lbz r6, lbl_8047B648(r13)
    lbz r0, lbl_8047B649(r13)
    cmplw r6, r0
    blt @L_80216F88
@L_80216FD8:
    cmplwi r29, 0x0
    beq @L_80216FF8
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
@L_80216FF8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
