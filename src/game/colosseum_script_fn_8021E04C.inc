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
    lwz r3, lbl_8047B610(r13)
    li r4, 0x0
    lbz r3, 0x1(r3)
    bl fn_801F025C
    lwz r0, lbl_8047B618(r13)
    mr r31, r3
    lwz r4, lbl_8047B610(r13)
    rlwinm. r0, r0, 0, 24, 24
    lwz r28, 0x2(r4)
    bne @L_8021E268
    cmplwi r28, 0x9
    bne @L_8021E10C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_8021E10C
    li r4, 0x34
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E10C
    mr r3, r29
    li r4, 0x34
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
@L_8021E10C:
    cmplwi r28, 0x18
    bne @L_8021E180
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E180
    li r4, 0x35
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E180
    mr r3, r30
    li r4, 0x35
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E180:
    cmplwi r28, 0x17
    bne @L_8021E1F4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E1F4
    li r4, 0x36
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E1F4
    mr r3, r30
    li r4, 0x36
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E1F4:
    cmplwi r28, 0xa
    bne @L_8021E268
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_8021E268
    li r4, 0x37
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E268
    mr r3, r30
    li r4, 0x37
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r31
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8021E268:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
