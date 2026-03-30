    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r28, 0x10(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r28, r3, 0, 16, 31
    li r4, 0x0
    lbz r3, 0x1(r5)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205184
    lwz r5, lbl_8047B610(r13)
    mr r4, r3
    lbz r0, 0x2(r5)
    cmplwi r0, 0x7
    bgt @L_8021D668
    lis r3, jumptable_8039A088@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A088@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r31
    bl fn_80200B10
    b @L_8021D668
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r28, r0
    li r4, 0x0
    mr r7, r28
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r4, r28
    li r3, 0x3
    bl fn_801F025C
    mr r29, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r29
    li r28, 0x0
    li r4, 0x4d
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D538
    mr r3, r29
    li r4, 0x4d
    bl fn_801F6C4C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8021D538
    mr r4, r30
    bl fn_801F00D0
    mr. r30, r3
    beq @L_8021D538
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D538
    mr r28, r30
@L_8021D538:
    cmplwi r28, 0x0
    bne @L_8021D544
    mr r28, r31
@L_8021D544:
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    b @L_8021D668
    mr r3, r31
    mr r5, r28
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    cmplwi r3, 0x0
    beq @L_8021D668
    mr r7, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    b @L_8021D668
    mr r3, r31
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D5CC
    li r0, 0x1
    stb r0, lbl_80478D78(r13)
    b @L_8021D668
@L_8021D5CC:
    li r0, 0x0
    stb r0, lbl_80478D78(r13)
    b @L_8021D668
    lwz r28, lbl_8047B64C(r13)
    cmplwi r28, 0x0
    beq @L_8021D668
    mr r4, r28
    li r3, 0x0
    bl fn_801F4460
    cmplwi r3, 0x0
    beq @L_8021D668
    mr r4, r28
    bl fn_801F8E34
    mr. r29, r3
    beq @L_8021D668
    li r4, 0x36
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D668
    mr r3, r29
    li r4, 0x36
    bl fn_80201C58
    mr r0, r3
    mr r3, r29
    mr r28, r0
    bl fn_80205B8C
    mr r4, r28
    bl fn_80123B5C
    extsb. r0, r3
    bge @L_8021D668
    mr r3, r29
    li r4, 0x36
    bl fn_80202810
    b @L_8021D668
    mr r3, r31
    mr r4, r28
    li r5, 0x1
    bl fn_80265598
@L_8021D668:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
