    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lis r3, -32698
    addi r3, r3, 29584
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_801D274C
    bl fn_801D16C4
    bl fn_801D1650
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_801D2728
    bl fn_8016557C
    mr r0, r3
    lis r3, 0x1
    mr r31, r0
    li r4, 0x20
    li r5, 0x0
    li r6, 0x408
    li r7, 0x0
    bl fn_800F9418
    mr r0, r3
    mr r3, r30
    mr r4, r0
    lis r5, 0x1
    bl fn_801654E0
    mr r3, r30
    li r4, 0x0
    li r5, 0x408
    bl fn_80166B3C
    mr r3, r30
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_801D272C
@L_801D2728:
    li r31, 0x0
@L_801D272C:
    lis r3, -32698
    li r4, 0x1
    addi r3, r3, 29584
    li r0, 0x0
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    stw r30, 0x8(r3)
    stw r31, 0xc(r3)
@L_801D274C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
