    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r3
    mr r28, r4
    mr r29, r5
    bl fn_80113F48
    mr r4, r31
    mr r30, r3
    bl fn_8018D998
    cmplwi r3, 0x0
    beq @L_801CB678
    mr r3, r30
    mr r4, r31
    mr r5, r30
    mr r6, r28
    mr r7, r29
    bl fn_801845E4
    b @L_801CB6E4
@L_801CB678:
    mr r3, r30
    mr r4, r31
    bl fn_800F9318
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_801CB698
    li r3, 0x0
    b @L_801CB6E8
@L_801CB698:
    mr r3, r30
    mr r4, r28
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801CB6B4
    li r3, 0x0
    b @L_801CB6E8
@L_801CB6B4:
    mr r4, r29
    bl fn_800EE150
    mr r0, r3
    mr r3, r31
    mr r31, r0
    li r5, 0x7
    mr r4, r31
    li r6, 0x0
    li r7, 0x1
    bl fn_800E4AC0
    mr r3, r31
    bl fn_800EE828
@L_801CB6E4:
    li r3, 0x1
@L_801CB6E8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
