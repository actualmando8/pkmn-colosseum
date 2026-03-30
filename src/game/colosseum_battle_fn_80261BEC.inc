    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r30, r3
    mr r28, r4
    mr r29, r5
    mr r4, r30
    li r3, 0x2
    mr r5, r28
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80261C28
    li r3, 0x0
    b @L_80261C84
@L_80261C28:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r31, r3, 0, 16, 31
    mr r3, r30
    mr r4, r28
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80261C58
    li r3, 0x0
    b @L_80261C84
@L_80261C58:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80261C70
    li r3, 0x0
    b @L_80261C84
@L_80261C70:
    mr r4, r31
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
@L_80261C84:
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80261CA4
    cmplwi r29, 0x0
    beq @L_80261CA4
    li r0, 0x0
    stb r0, 0x0(r29)
@L_80261CA4:
    lmw r28, 0x10(r1)
    li r3, 0x1
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
