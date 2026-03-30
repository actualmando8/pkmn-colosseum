    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r5
    mr r31, r3
    lwz r30, 0x0(r5)
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80215558
    li r3, 0x1
    b @L_80215600
@L_80215558:
    mr r3, r31
    mr r4, r30
    bl fn_80202B88
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80215578
    li r3, 0x1
    b @L_80215600
@L_80215578:
    mr r3, r30
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80205B8C
    li r28, 0x0
    mr r29, r3
    b @L_802155F0
@L_8021559C:
    extsb r31, r28
    mr r3, r30
    mr r4, r31
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802155EC
    mr r3, r30
    mr r6, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    mr r3, r29
    bl fn_80123B5C
    extsb. r0, r3
    blt @L_802155EC
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x4(r27)
    b @L_80215600
@L_802155EC:
    addi r28, r28, 0x1
@L_802155F0:
    extsb r0, r28
    cmpwi r0, 0x4
    blt @L_8021559C
    li r3, 0x1
@L_80215600:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
