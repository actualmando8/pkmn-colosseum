    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, -32698
    stw r0, 0x24(r1)
    addi r3, r3, 31936
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lhz r29, 0x10(r3)
    lwz r30, 0x0(r3)
    lhz r28, 0x4(r3)
    b @L_801DAD04
@L_801DACC8:
    cmplwi r30, 0x0
    beq @L_801DACFC
    lbz r0, 0x74(r30)
    cmplwi r0, 0x0
    beq @L_801DACEC
    mr r3, r30
    bl fn_801DD3E4
    mr r3, r30
    bl fn_801DD23C
@L_801DACEC:
    mr r3, r30
    li r4, 0x0
    li r5, 0x8c
    bl memset
@L_801DACFC:
    addi r31, r31, 0x1
    addi r30, r30, 0x8c
@L_801DAD04:
    cmpw r31, r28
    blt @L_801DACC8
    mr r3, r29
    bl fn_800E24B0
    mr r3, r29
    bl fn_800E209C
    lis r3, -32698
    li r4, 0x0
    addi r3, r3, 31936
    li r5, 0x14
    bl memset
    lis r3, -32698
    li r29, 0x0
    addi r30, r3, 31956
    mr r31, r29
@L_801DAD40:
    lhz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_801DAD5C
    bl fn_800F915C
    lhz r3, 0x0(r30)
    bl fn_8017B1CC
    sth r31, 0x0(r30)
@L_801DAD5C:
    addi r29, r29, 0x1
    addi r30, r30, 0x2
    cmpwi r29, 0x10
    blt @L_801DAD40
    bl fn_801D2F94
    lis r3, -32698
    li r4, 0x0
    addi r3, r3, 31936
    stw r4, -21516(r13)
    lbz r0, 0x6(r3)
    stw r4, -21512(r13)
    cmplwi r0, 0x0
    bne @L_801DADA0
    li r3, 0x6f7
    bl fn_800F915C
    li r3, 0x6f7
    bl fn_8017B1CC
@L_801DADA0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
