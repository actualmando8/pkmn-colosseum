    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r28, r3
    mr r31, r5
    mr r30, r6
    mr r3, r29
    bl fn_80205B8C
    addi r4, r1, 0xa
    addi r5, r1, 0x8
    bl fn_80120B00
    mr r3, r28
    mr r4, r29
    mr r5, r31
    mr r6, r30
    li r7, 0x0
    bl fn_8025C264
    lhz r0, 0xa(r1)
    mr r31, r3
    lhz r5, 0x8(r1)
    mr r3, r28
    mr r4, r30
    extsh r6, r0
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_80256830
    li r31, 0x0
@L_80256830:
    cmpwi r31, 0x0
    bne @L_80256840
    li r3, 0x0
    b @L_80256848
@L_80256840:
    cmpwi r31, -0x1
    li r3, 0x1
@L_80256848:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
