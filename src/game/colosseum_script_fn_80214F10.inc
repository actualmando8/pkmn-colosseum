    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x54(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r29, 0x44(r1)
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r30, r0
    mr r4, r30
    bl fn_801F4354
    mr r6, r30
    addi r4, r1, 0x8
    li r5, 0x18
    bl fn_80215008
    mr. r31, r3
    beq @L_80214FE8
    bl fn_800E0C54
    rlwinm r10, r3, 0, 16, 31
    lwz r0, lbl_8047B618(r13)
    divw r6, r10, r31
    addi r4, r1, 0x8
    rlwinm r0, r0, 0, 22, 20
    stw r0, lbl_8047B618(r13)
    mr r3, r30
    mr r5, r29
    mullw r0, r6, r31
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    subf r0, r0, r10
    rlwinm r0, r0, 1, 0, 30
    lhzx r4, r4, r0
    sth r4, lbl_8047B60C(r13)
    bl fn_8022B2CC
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80214FF4
@L_80214FE8:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80214FF4:
    lmw r29, 0x44(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
