    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    mr r30, r4
    mr r31, r5
    mr r27, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r28, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    li r0, 0x0
    li r5, 0xd
    lis r4, -32733
    stw r5, 0x20(r1)
    mr r29, r3
    addi r5, r1, 0x20
    stw r0, 0x24(r1)
    addi r4, r4, 25192
    li r3, 0x0
    li r6, 0x0
    stw r0, 0x28(r1)
    stw r0, 0x2c(r1)
    stw r27, 0x30(r1)
    bl fn_801F37B0
    lwz r0, 0x24(r1)
    cmplwi r0, 0x0
    beq @L_80239664
    li r29, 0x0
    b @L_802396A8
@L_80239664:
    li r0, 0x0
    li r4, 0x4d
    lis r3, -32733
    stw r4, 0xc(r1)
    addi r4, r3, 25192
    addi r5, r1, 0xc
    stw r0, 0x10(r1)
    li r3, 0x0
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r0, 0x18(r1)
    stw r27, 0x1c(r1)
    bl fn_801F37B0
    lwz r0, 0x10(r1)
    cmplwi r0, 0x0
    beq @L_802396A8
    li r29, 0x0
@L_802396A8:
    mr r4, r28
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802397A0
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_802396EC
    cmplwi r0, 0x164
    beq @L_802396EC
    cmplwi r0, 0xf8
    beq @L_802396EC
    cmplwi r0, 0x161
    bne @L_802396F4
@L_802396EC:
    li r3, 0x9
    b @L_802397A4
@L_802396F4:
    cmplwi r0, 0xed
    bne @L_80239718
    mr r3, r31
    bl fn_80205B8C
    addi r5, r1, 0x8
    li r4, 0x0
    bl fn_80120B00
    lhz r3, 0x8(r1)
    b @L_802397A4
@L_80239718:
    cmplwi r0, 0x137
    bne @L_80239784
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80239738
    li r0, 0xb
    sth r0, 0x8(r1)
    b @L_8023977C
@L_80239738:
    cmplwi r0, 0x3
    bne @L_8023974C
    li r0, 0x5
    sth r0, 0x8(r1)
    b @L_8023977C
@L_8023974C:
    cmplwi r0, 0x1
    bne @L_80239760
    li r0, 0xa
    sth r0, 0x8(r1)
    b @L_8023977C
@L_80239760:
    cmplwi r0, 0x4
    bne @L_80239774
    li r0, 0xf
    sth r0, 0x8(r1)
    b @L_8023977C
@L_80239774:
    li r0, 0x0
    sth r0, 0x8(r1)
@L_8023977C:
    lhz r3, 0x8(r1)
    b @L_802397A4
@L_80239784:
    mr r4, r30
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    b @L_802397A4
@L_802397A0:
    li r3, 0x9
@L_802397A4:
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
