    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    mr r0, r3
    li r3, 0x0
    stmw r25, 0x14(r1)
    mr r31, r0
    mr r4, r31
    bl fn_801F4354
    mr r4, r31
    li r3, 0x0
    bl fn_801F4220
    mr r3, r31
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r28, r0
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r27, r0
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    bl fn_8012640C
    rlwinm r26, r3, 0, 24, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 24, 31
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802230A4
    mr r3, r31
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80223038
    cmplwi r26, 0x1
    bne @L_80223000
    mr r3, r31
    bl fn_80206C94
    bl fn_801C3430
@L_80223000:
    cmplwi r25, 0x1
    bne @L_802230A4
    mr r3, r28
    bl fn_80124A60
    mr r3, r27
    bl fn_80124A60
    mr r3, r30
    bl fn_802078F0
    mr r3, r29
    bl fn_802078F0
    mr r3, r31
    bl fn_80206C94
    bl fn_801C3430
    b @L_802230A4
@L_80223038:
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x46
    li r6, 0x0
    bl fn_801F4C14
    cmplwi r25, 0x1
    bne @L_80223090
    mr r3, r28
    bl fn_80124A60
    mr r3, r27
    bl fn_80124A60
    mr r3, r30
    bl fn_802078F0
    mr r3, r29
    bl fn_802078F0
@L_80223090:
    lis r4, lbl_8037879E@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037879E@l
    li r5, 0x0
    bl fn_80211B94
@L_802230A4:
    lmw r25, 0x14(r1)
    li r3, 0x1
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
