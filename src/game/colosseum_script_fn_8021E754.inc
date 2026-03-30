    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x34(r1)
    li r5, 0x18
    li r6, 0x0
    stmw r23, 0xc(r1)
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x25
    li r6, 0x0
    bl fn_801F54A4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021E7A8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021E9E0
@L_8021E7A8:
    li r3, 0x0
    bl fn_801F2A7C
    mr. r30, r3
    bne @L_8021E7C8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021E9E0
@L_8021E7C8:
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr. r29, r3
    bne @L_8021E7F0
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021E9E0
@L_8021E7F0:
    mr r3, r30
    li r4, 0x0
    li r5, 0x48
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r28, r3, 0, 24, 31
    mr r4, r30
    li r3, 0x9
    bl fn_801F025C
    li r4, 0x0
    mr r24, r3
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r3, r3, 0, 16, 31
    bl fn_801FBD58
    bl fn_801FBD40
    mr r31, r3
    li r27, 0x0
    li r23, 0x0
@L_8021E854:
    mr r3, r24
    rlwinm r6, r23, 0, 16, 31
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r25, r3
    bl fn_80206A04
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E894
    mr r3, r25
    bl fn_80203E7C
    rlwinm r4, r27, 0, 24, 31
    rlwinm r0, r3, 0, 24, 31
    cmplw r4, r0
    bge @L_8021E894
    mr r27, r3
@L_8021E894:
    addi r23, r23, 0x1
    cmpwi r23, 0x6
    blt @L_8021E854
    li r25, 0x0
@L_8021E8A4:
    mr r3, r30
    rlwinm r6, r25, 0, 16, 31
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r24, r3
    bl fn_80206A04
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021E900
    mr r3, r24
    li r4, 0x0
    li r5, 0xcf
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_8021E900
    mr r3, r24
    bl fn_80203E7C
    rlwinm r4, r27, 0, 24, 31
    rlwinm r0, r3, 0, 24, 31
    cmplw r4, r0
    bge @L_8021E900
    mr r27, r3
@L_8021E900:
    addi r25, r25, 0x1
    cmpwi r25, 0x6
    blt @L_8021E8A4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_801F54A4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021E940
    cmplwi r26, 0x1
    bgt @L_8021E938
    li r3, 0x1
    b @L_8021E944
@L_8021E938:
    li r3, 0x2
    b @L_8021E944
@L_8021E940:
    li r3, 0x4
@L_8021E944:
    rlwinm r0, r27, 0, 24, 31
    rlwinm r7, r31, 0, 16, 31
    mullw r0, r0, r28
    rlwinm r8, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    mullw r0, r7, r0
    mullw r24, r8, r0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021E994
    mr r3, r24
    bl fn_801CA8B0
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021E9E0
@L_8021E994:
    mr r3, r29
    mr r4, r24
    bl fn_801294C4
    mr r4, r24
    li r3, 0x2f
    bl fn_80132A38
    mr r4, r24
    li r3, 0x4b
    bl fn_80132A38
    mr r3, r29
    li r4, 0x1
    li r5, 0x0
    bl fn_8012A5B0
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021E9E0:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
