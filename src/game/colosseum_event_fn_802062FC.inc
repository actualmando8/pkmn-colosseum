    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr. r30, r3
    bne @L_8020631C
    li r3, 0x0
    b @L_802065F4
@L_8020631C:
    bne @L_80206328
    li r0, 0x0
    b @L_80206444
@L_80206328:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80206340
    li r0, 0x0
    b @L_80206444
@L_80206340:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_80206364
    li r0, 0x0
    b @L_80206444
@L_80206364:
    bne @L_80206370
    li r0, 0x0
    b @L_80206430
@L_80206370:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80206388
    li r0, 0x0
    b @L_80206430
@L_80206388:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802063AC
    li r0, 0x0
    b @L_80206430
@L_802063AC:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802063C0
    li r0, 0x0
    b @L_80206430
@L_802063C0:
    cmplwi r31, 0x0
    bne @L_802063D0
    li r3, 0x0
    b @L_802063E4
@L_802063D0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802063E4:
    cmplwi r3, 0x0
    bne @L_802063F4
    li r0, 0x0
    b @L_80206430
@L_802063F4:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206408
    li r0, 0x0
    b @L_80206430
@L_80206408:
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_8020642C
    li r0, 0x0
    b @L_80206430
@L_8020642C:
    li r0, 0x1
@L_80206430:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80206440
    li r0, 0x0
    b @L_80206444
@L_80206440:
    li r0, 0x1
@L_80206444:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80206454
    li r3, 0x0
    b @L_802065F4
@L_80206454:
    mr r3, r30
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80206478
    li r3, 0x0
    b @L_802065F4
@L_80206478:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_8020649C
    li r0, 0x0
    b @L_802065E8
@L_8020649C:
    bne @L_802064A8
    li r0, 0x0
    b @L_80206568
@L_802064A8:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_802064C0
    li r0, 0x0
    b @L_80206568
@L_802064C0:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_802064E4
    li r0, 0x0
    b @L_80206568
@L_802064E4:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802064F8
    li r0, 0x0
    b @L_80206568
@L_802064F8:
    cmplwi r31, 0x0
    bne @L_80206508
    li r3, 0x0
    b @L_8020651C
@L_80206508:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020651C:
    cmplwi r3, 0x0
    bne @L_8020652C
    li r0, 0x0
    b @L_80206568
@L_8020652C:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206540
    li r0, 0x0
    b @L_80206568
@L_80206540:
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80206564
    li r0, 0x0
    b @L_80206568
@L_80206564:
    li r0, 0x1
@L_80206568:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80206578
    li r0, 0x0
    b @L_802065E8
@L_80206578:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_8020659C
    li r0, 0x0
    b @L_802065E8
@L_8020659C:
    cmplwi r31, 0x0
    bne @L_802065AC
    li r3, 0x0
    b @L_802065C0
@L_802065AC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802065C0:
    cmplwi r3, 0x0
    bne @L_802065D0
    li r0, 0x0
    b @L_802065E8
@L_802065D0:
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802065E4
    li r0, 0x0
    b @L_802065E8
@L_802065E4:
    li r0, 0x1
@L_802065E8:
    rlwinm r3, r0, 0, 24, 31
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_802065F4:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
