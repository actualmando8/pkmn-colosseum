    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bge @L_801DE5D0
    mr r3, r29
    bl fn_8017B3E4
    b @L_801DE604
@L_801DE5D0:
    mr r3, r29
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bne @L_801DE604
    cmplwi r30, 0x0
    beq @L_801DE604
    mr r3, r29
    mr r4, r30
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801DE604
    mr r3, r29
    bl fn_8017B3E4
@L_801DE604:
    lis r3, -32728
    addi r31, r3, -26168
@L_801DE60C:
    mr r3, r29
    bl fn_8017B2CC
    mr. r30, r3
    bge @L_801DE628
    mr r3, r31
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DE628:
    cmpwi r30, 0x0
    beq @L_801DE638
    bl fn_800F0308
    b @L_801DE60C
@L_801DE638:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
