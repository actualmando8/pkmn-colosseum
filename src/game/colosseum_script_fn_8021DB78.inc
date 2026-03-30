    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    mr r4, r31
    bl fn_801F4354
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80204A10
    rlwinm r0, r3, 0, 24, 31
    li r3, 0x0
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r29, r0, 27, 5, 31
    bl fn_801F18DC
    rlwinm r0, r3, 0, 24, 31
    mr r3, r30
    subfic r0, r0, 0x1
    cntlzw r0, r0
    rlwinm r28, r0, 27, 5, 31
    bl fn_801F8000
    cmplwi r3, 0x0
    bne @L_8021DC00
    rlwinm. r0, r29, 0, 24, 31
    bne @L_8021DC00
    li r28, 0x1
@L_8021DC00:
    mr r3, r31
    bl fn_802037DC
    mr r4, r3
    li r3, 0x16
    bl fn_80132A38
    mr r3, r31
    bl fn_802037DC
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r30
    bl fn_801F8000
    mr r4, r3
    li r3, 0x22
    bl fn_80132A38
    mr r3, r30
    bl fn_801F8100
    mr r4, r3
    li r3, 0x23
    bl fn_80132A38
    mr r3, r30
    bl fn_801F8100
    mr r4, r3
    li r3, 0x25
    bl fn_80132A38
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021DC80
    li r0, 0x5
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021DD04
@L_8021DC80:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021DCF8
    mr r4, r31
    li r3, 0x0
    bl fn_801F2434
    cmpwi r3, 0x46
    bge @L_8021DCA8
    cmpwi r3, 0x0
    bge @L_8021DCB8
@L_8021DCA8:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021DD04
@L_8021DCB8:
    cmpwi r3, 0x28
    blt @L_8021DCD0
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021DD04
@L_8021DCD0:
    cmpwi r3, 0xa
    blt @L_8021DCE8
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021DD04
@L_8021DCE8:
    li r0, 0x3
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021DD04
@L_8021DCF8:
    li r0, 0x4
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021DD04:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
