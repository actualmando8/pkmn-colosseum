    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    cmplwi r28, 0x0
    li r31, 0x0
    beq @L_80077520
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80077524
@L_80077520:
    li r31, 0x1
@L_80077524:
    cmpwi r31, 0x0
    beq @L_80077534
    li r3, 0x1
    b @L_800776C4
@L_80077534:
    cmpwi r30, 0x1
    beq @L_80077580
    bge @L_8007754C
    cmpwi r30, 0x0
    bge @L_80077558
    b @L_800776A8
@L_8007754C:
    cmpwi r30, 0x3
    bge @L_800776A8
    b @L_800775A8
@L_80077558:
    mr r3, r28
    bl fn_8011F4A8
    rlwinm r5, r3, 0, 24, 31
    lha r0, 0x0(r29)
    srawi r4, r5, 31
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r3, r0, 0, 24, 31
    b @L_800776C4
@L_80077580:
    mr r3, r28
    bl fn_8011F4A8
    lha r0, 0x2(r29)
    rlwinm r5, r3, 0, 24, 31
    rlwinm r3, r5, 1, 31, 31
    srawi r4, r0, 31
    subfc r0, r5, r0
    adde r0, r4, r3
    rlwinm r3, r0, 0, 24, 31
    b @L_800776C4
@L_800775A8:
    mr r3, r28
    bl fn_8011F1A0
    mr r30, r3
    bl fn_8006B420
    rlwinm r0, r30, 0, 16, 31
    mr r31, r3
    cmpwi r0, 0xaf
    beq @L_800775E0
    bge @L_800775E8
    cmpwi r0, 0x0
    beq @L_800775D8
    b @L_800775E8
@L_800775D8:
    li r3, 0x1
    b @L_800775F0
@L_800775E0:
    li r3, 0x0
    b @L_800775F0
@L_800775E8:
    mr r3, r30
    bl fn_80142984
@L_800775F0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80077604
    li r3, 0x0
    b @L_800776C4
@L_80077604:
    lwz r0, 0x8(r31)
    cmpwi r0, 0x1
    beq @L_80077634
    bge @L_80077620
    cmpwi r0, 0x0
    bge @L_8007762C
    b @L_800776A0
@L_80077620:
    cmpwi r0, 0x3
    bge @L_800776A0
    b @L_80077648
@L_8007762C:
    li r3, 0x1
    b @L_800776C4
@L_80077634:
    rlwinm r0, r30, 0, 16, 31
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    b @L_800776C4
@L_80077648:
    lis r3, -32721
    lwz r0, -32504(r13)
    addi r5, r3, -7080
    li r4, 0x0
    rlwinm r3, r30, 0, 16, 31
    mtctr r0
    cmplwi r0, 0x0
    ble @L_80077698
@L_80077668:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8007768C
    add r3, r31, r4
    lbz r0, 0x18(r3)
    cntlzw r0, r0
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    b @L_800776C4
@L_8007768C:
    addi r5, r5, 0x2
    addi r4, r4, 0x1
    bdnz @L_80077668
@L_80077698:
    li r3, 0x1
    b @L_800776C4
@L_800776A0:
    li r3, 0x0
    b @L_800776C4
@L_800776A8:
    lis r3, -32729
    lis r5, -32729
    addi r3, r3, -30136
    li r4, 0xfb
    addi r5, r5, -30120
    bl fn_80196E10
    li r3, 0x0
@L_800776C4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
