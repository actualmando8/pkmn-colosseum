    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r27, r3
    mr r31, r28
    bl fn_801DB060
    mr r30, r3
    bl fn_801DB060
    lwz r4, 0x3c(r27)
    mr r29, r3
    lwz r0, 0x0(r31)
    stw r0, 0xc(r27)
    lwz r3, 0xc(r27)
    cmpwi r3, 0xb
    beq @L_801DC644
    blt @L_801DC670
    cmpwi r3, 0x10
    bge @L_801DC670
    b @L_801DC65C
@L_801DC644:
    lhz r0, 0x14(r4)
    cmpw r0, r3
    bge @L_801DC670
    li r0, 0x0
    stw r0, 0xc(r27)
    b @L_801DC670
@L_801DC65C:
    lhz r0, 0x14(r4)
    cmpw r0, r3
    bge @L_801DC670
    li r0, 0x1
    stw r0, 0xc(r27)
@L_801DC670:
    lwz r4, 0x8(r31)
    li r3, 0x1
    li r0, 0x2
    stw r4, 0x8(r27)
    stb r3, 0x16(r27)
    stb r0, 0x17(r27)
    lwz r0, 0x10(r31)
    cmpwi r0, 0x2
    beq @L_801DC6BC
    bge @L_801DC6A4
    cmpwi r0, 0x1
    bge @L_801DC6B0
    b @L_801DC6D0
@L_801DC6A4:
    cmpwi r0, 0x5
    beq @L_801DC6C8
    b @L_801DC6D0
@L_801DC6B0:
    li r4, 0x0
    addi r28, r28, 0x10
    b @L_801DC6DC
@L_801DC6BC:
    li r4, 0x0
    addi r28, r28, 0x14
    b @L_801DC6DC
@L_801DC6C8:
    lwz r0, 0xc(r31)
    stb r0, 0x17(r27)
@L_801DC6D0:
    addi r0, r28, 0x37
    lwz r4, 0x14(r31)
    rlwinm r28, r0, 0, 0, 26
@L_801DC6DC:
    lwz r0, 0x10(r31)
    cmpwi r0, 0x3
    bgt @L_801DC6F4
    lwz r0, 0x8(r27)
    ori r0, r0, 0x78
    stw r0, 0x8(r27)
@L_801DC6F4:
    lwz r0, 0x10(r31)
    cmpwi r0, 0x5
    bgt @L_801DC778
    lwz r3, 0x8(r27)
    rlwinm. r0, r3, 0, 16, 16
    beq @L_801DC714
    xori r0, r3, 0x8000
    stw r0, 0x8(r27)
@L_801DC714:
    lwz r3, 0x8(r27)
    rlwinm. r0, r3, 0, 15, 15
    beq @L_801DC728
    xoris r0, r3, 0x1
    stw r0, 0x8(r27)
@L_801DC728:
    lwz r3, 0x8(r27)
    rlwinm. r0, r3, 0, 14, 14
    beq @L_801DC73C
    xoris r0, r3, 0x2
    stw r0, 0x8(r27)
@L_801DC73C:
    lwz r3, 0x8(r27)
    rlwinm. r0, r3, 0, 13, 13
    beq @L_801DC750
    xoris r0, r3, 0x4
    stw r0, 0x8(r27)
@L_801DC750:
    lwz r3, 0x8(r27)
    rlwinm. r0, r3, 0, 12, 12
    beq @L_801DC764
    xoris r0, r3, 0x8
    stw r0, 0x8(r27)
@L_801DC764:
    lwz r3, 0x8(r27)
    rlwinm. r0, r3, 0, 11, 11
    beq @L_801DC778
    xoris r0, r3, 0x10
    stw r0, 0x8(r27)
@L_801DC778:
    cmpwi r4, 0x0
    beq @L_801DC7F4
    li r0, 0x4e20
    mr r3, r28
    stw r0, 0x18(r27)
    mr r6, r30
    li r5, 0x4e20
    bl fn_8010147C
    mr r4, r30
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DC7E0
    stw r30, 0x1c(r27)
    mr r4, r30
    li r3, 0x4e20
    bl fn_800F9318
    mr r5, r29
    li r4, 0x4e20
    bl fn_801012E8
    mr r4, r29
    li r3, 0x4e20
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DC7E0
    stw r29, 0x20(r27)
@L_801DC7E0:
    lwz r3, 0x14(r31)
    addi r0, r3, 0x1f
    rlwinm r0, r0, 0, 0, 26
    add r28, r28, r0
    b @L_801DC804
@L_801DC7F4:
    li r0, 0x0
    stw r0, 0x18(r27)
    stw r0, 0x20(r27)
    stw r0, 0x1c(r27)
@L_801DC804:
    mr r3, r28
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
