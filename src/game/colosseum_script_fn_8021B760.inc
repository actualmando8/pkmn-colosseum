    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0xf8
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r29, r0
    li r4, 0xc
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021B7EC
    mr r3, r31
    li r4, 0xc
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    li r4, 0x22
    li r5, 0x0
    bl fn_8020248C
@L_8021B7EC:
    mr r3, r29
    mr r4, r30
    bl fn_8020A2B8
    mr r3, r31
    li r4, 0x0
    li r5, 0xf5
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
