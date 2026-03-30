    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r5
    mr r29, r3
    lwz r30, 0xc(r5)
    lwz r28, 0x10(r5)
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023629C
    li r3, 0x1
    b @L_80236444
@L_8023629C:
    cmplwi r30, 0x0
    bne @L_802362AC
    li r27, 0x0
    b @L_802362EC
@L_802362AC:
    lwz r0, 0x8(r31)
    cmpwi r0, 0x1
    bne @L_802362CC
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    mr r27, r3
    b @L_802362EC
@L_802362CC:
    cmpwi r0, 0x2
    bne @L_802362E8
    mr r4, r30
    li r3, 0x3
    bl fn_801F025C
    mr r27, r3
    b @L_802362EC
@L_802362E8:
    li r27, 0x0
@L_802362EC:
    mr r4, r29
    li r3, 0x2
    bl fn_801F025C
    lwz r0, 0x8(r31)
    cmpwi r0, 0x1
    beq @L_8023630C
    cmpwi r0, 0x2
    bne @L_8023631C
@L_8023630C:
    cmplwi r27, 0x0
    bne @L_8023631C
    li r3, 0x1
    b @L_80236444
@L_8023631C:
    cmpwi r0, 0x0
    bne @L_8023633C
    cmplwi r30, 0x0
    beq @L_80236364
    cmplw r30, r29
    bne @L_80236364
    li r3, 0x1
    b @L_80236444
@L_8023633C:
    cmpwi r0, 0x1
    beq @L_8023634C
    cmpwi r0, 0x2
    bne @L_8023635C
@L_8023634C:
    cmplw r27, r3
    beq @L_80236364
    li r3, 0x1
    b @L_80236444
@L_8023635C:
    li r3, 0x1
    b @L_80236444
@L_80236364:
    lwz r0, 0x0(r31)
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    rlwinm r30, r0, 0, 16, 31
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r30, 0x0
    bne @L_802363A4
    li r0, 0x0
    b @L_80236428
@L_802363A4:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023640C
    mr r3, r29
    bl fn_80207BF4
    b @L_80236410
@L_8023640C:
    li r3, 0x0
@L_80236410:
    rlwinm r0, r3, 0, 16, 31
    cmplw r30, r0
    bne @L_80236424
    li r0, 0x1
    b @L_80236428
@L_80236424:
    li r0, 0x0
@L_80236428:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236440
    stw r29, 0x4(r31)
    li r3, 0x0
    b @L_80236444
@L_80236440:
    li r3, 0x1
@L_80236444:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
