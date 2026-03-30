    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    cmplwi r5, 0x0
    stw r0, 0x84(r1)
    stmw r28, 0x70(r1)
    mr r28, r4
    mr r30, r3
    bne @L_802620AC
    li r29, 0x1
    b @L_802620B0
@L_802620AC:
    lbz r29, 0x0(r5)
@L_802620B0:
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802620C8
    li r3, 0x1
    b @L_802621B0
@L_802620C8:
    mr r4, r30
    mr r5, r28
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_802620E8
    li r31, 0x0
    b @L_80262148
@L_802620E8:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r31, r3, 0, 16, 31
    mr r3, r30
    mr r4, r28
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80262118
    li r31, 0x0
    b @L_80262148
@L_80262118:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80262130
    li r31, 0x0
    b @L_80262148
@L_80262130:
    mr r4, r31
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    mr r31, r3
@L_80262148:
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_801FE168
    rlwinm. r0, r29, 0, 24, 31
    bne @L_80262164
    li r0, 0x0
    stb r0, 0x31(r1)
@L_80262164:
    li r0, 0x6
    addi r5, r1, 0x34
    addi r4, r1, 0x4
    mtctr r0
@L_80262174:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80262174
    mr r3, r31
    addi r9, r1, 0x38
    li r4, -0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x1
@L_802621B0:
    lmw r28, 0x70(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
