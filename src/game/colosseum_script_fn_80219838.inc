    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x12
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    mr r29, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r29
    mr r28, r0
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    mr r3, r28
    bl fn_80123B5C
    mr r30, r3
    extsb. r0, r30
    bge @L_80219898
    li r31, 0x0
    b @L_802198B0
@L_80219898:
    mr r3, r28
    extsb r6, r30
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm r31, r3, 0, 24, 31
@L_802198B0:
    mr r3, r29
    li r4, 0x29
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80219944
    extsb. r0, r30
    blt @L_80219944
    rlwinm. r0, r31, 0, 24, 31
    beq @L_80219944
    mr r3, r28
    extsb r6, r30
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    li r3, 0x0
    mr r4, r28
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r29
    li r4, 0x29
    li r5, 0x0
    bl fn_8020248C
    mr r3, r29
    mr r5, r28
    li r4, 0x29
    bl fn_80201B2C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80219950
@L_80219944:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80219950:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
