    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lbz r0, 0x1(r3)
    extsb r0, r0
    cmpwi r0, 0x1
    beq @L_8009767C
    bge @L_80097424
    cmpwi r0, 0x0
    bge @L_80097430
    b @L_8009767C
@L_80097424:
    cmpwi r0, 0x3
    bge @L_8009767C
    b @L_80097584
@L_80097430:
    li r7, -0x1
    lis r4, -32704
    stb r7, 0x97(r3)
    addi r5, r4, -19584
    li r6, 0x0
    stb r6, 0x1(r5)
    lbz r0, 0x0(r5)
    lbz r4, 0x95(r3)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    stb r4, 0x2(r5)
    stb r7, 0x3(r5)
    stb r7, 0x1a(r5)
    beq @L_80097478
    stb r6, 0x95(r3)
    li r0, 0x1
    stb r0, 0x1(r5)
    b @L_80097488
@L_80097478:
    li r4, 0x1
    li r0, 0x7
    stb r4, 0x95(r3)
    stb r0, 0x1(r5)
@L_80097488:
    lis r4, -32704
    lhz r0, 0x94(r3)
    addi r4, r4, -19584
    li r31, 0x0
    lwz r3, 0xc(r4)
    sth r0, 0xc(r1)
    cmplwi r3, 0x0
    beq @L_8009767C
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpwi r0, 0x1
    beq @L_800974FC
    bge @L_800974C8
    cmpwi r0, 0x0
    bge @L_800974D4
    b @L_8009751C
@L_800974C8:
    cmpwi r0, 0x3
    bge @L_8009751C
    b @L_80097518
@L_800974D4:
    li r4, 0x0
    li r5, 0xc2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_800974F4
    li r31, 0x55
    b @L_8009751C
@L_800974F4:
    li r31, 0x54
    b @L_8009751C
@L_800974FC:
    lhz r0, 0x18(r4)
    cmplwi r0, 0x0
    beq @L_80097510
    li r31, 0x56
    b @L_8009751C
@L_80097510:
    li r31, 0x57
    b @L_8009751C
@L_80097518:
    li r31, 0x58
@L_8009751C:
    lis r3, -32721
    li r30, 0x0
    addi r29, r3, -4156
@L_80097528:
    lwz r3, 0x0(r29)
    cmpw r31, r3
    bne @L_80097558
    mr r3, r31
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80097570
    mr r3, r31
    li r4, 0x0
    bl fn_8010264C
    b @L_80097570
@L_80097558:
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80097570
    lwz r3, 0x0(r29)
    bl fn_80102510
@L_80097570:
    addi r29, r29, 0x4
    addi r30, r30, 0x1
    cmplwi r30, 0x5
    blt @L_80097528
    b @L_8009767C
@L_80097584:
    lis r4, -32704
    lhz r0, 0x94(r3)
    addi r4, r4, -19584
    li r30, 0x0
    lwz r3, 0xc(r4)
    sth r0, 0x8(r1)
    cmplwi r3, 0x0
    beq @L_8009767C
    lbz r0, 0x9(r1)
    extsb r0, r0
    cmpwi r0, 0x1
    beq @L_800975F8
    bge @L_800975C4
    cmpwi r0, 0x0
    bge @L_800975D0
    b @L_80097618
@L_800975C4:
    cmpwi r0, 0x3
    bge @L_80097618
    b @L_80097614
@L_800975D0:
    li r4, 0x0
    li r5, 0xc2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_800975F0
    li r30, 0x55
    b @L_80097618
@L_800975F0:
    li r30, 0x54
    b @L_80097618
@L_800975F8:
    lhz r0, 0x18(r4)
    cmplwi r0, 0x0
    beq @L_8009760C
    li r30, 0x56
    b @L_80097618
@L_8009760C:
    li r30, 0x57
    b @L_80097618
@L_80097614:
    li r30, 0x58
@L_80097618:
    lis r3, -32721
    li r31, 0x0
    addi r29, r3, -4156
@L_80097624:
    lwz r3, 0x0(r29)
    cmpw r30, r3
    bne @L_80097654
    mr r3, r30
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8009766C
    mr r3, r30
    li r4, 0x0
    bl fn_8010264C
    b @L_8009766C
@L_80097654:
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8009766C
    lwz r3, 0x0(r29)
    bl fn_80102510
@L_8009766C:
    addi r29, r29, 0x4
    addi r31, r31, 0x1
    cmplwi r31, 0x5
    blt @L_80097624
@L_8009767C:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
