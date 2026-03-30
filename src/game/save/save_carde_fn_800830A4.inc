    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r30, r3
    cmplwi r30, 0x0
    lis r3, lbl_8026F1C8@ha
    addi r29, r3, lbl_8026F1C8@l
    beq @L_800830CC
    b @L_800830DC
@L_800830CC:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
    mr r30, r3
@L_800830DC:
    lbz r3, 0x4000(r30)
    li r0, 0x0
    cmplwi r3, 0x1
    beq @L_800830F4
    cmplwi r3, 0x2
    bne @L_800830F8
@L_800830F4:
    li r0, 0x1
@L_800830F8:
    cmpwi r0, 0x0
    bne @L_80083110
    addi r3, r29, 0x0
    addi r5, r29, 0xa0
    li r4, 0x161
    bl fn_80196E10
@L_80083110:
    li r0, 0x0
    li r27, 0x0
    stb r0, 0x4000(r30)
    addi r31, r1, 0x8
@L_80083120:
    cmplwi r30, 0x0
    beq @L_80083130
    mr r3, r30
    b @L_8008313C
@L_80083130:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_8008313C:
    cmplwi r31, 0x0
    addi r6, r3, 0x4000
    beq @L_80083150
    li r0, 0x0
    stw r0, 0x8(r1)
@L_80083150:
    li r7, 0x0
@L_80083154:
    addi r0, r3, 0x24
    cmplw r6, r0
    blt @L_800831EC
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_800831EC
    lbz r0, 0x1b(r3)
    extsb r0, r0
    cmpwi r0, 0x3
    bgt @L_8008319C
    lbz r0, 0x1c(r3)
    extsb r0, r0
    cmpwi r0, 0x6
    bgt @L_8008319C
    lbz r0, 0x1d(r3)
    extsb r0, r0
    cmpwi r0, 0x5
    ble @L_800831A8
@L_8008319C:
    li r0, 0x0
    sth r0, 0x0(r3)
    b @L_800831EC
@L_800831A8:
    cmpw r7, r27
    bne @L_800831B4
    stw r3, 0x8(r1)
@L_800831B4:
    lbz r4, 0x1c(r3)
    addi r7, r7, 0x1
    lbz r0, 0x1d(r3)
    extsb r4, r4
    lbz r5, 0x1b(r3)
    extsb r0, r0
    mullw r0, r4, r0
    extsb r5, r5
    rlwinm r4, r0, 4, 0, 27
    addi r0, r4, 0x76
    mullw r0, r5, r0
    add r3, r0, r3
    addi r3, r3, 0x24
    b @L_80083154
@L_800831EC:
    cmpwi r27, 0x0
    bge @L_800831F8
    stw r3, 0x8(r1)
@L_800831F8:
    lwz r28, 0x8(r1)
    cmplwi r28, 0x0
    bne @L_80083214
    addi r3, r29, 0x0
    li r4, 0x169
    addi r5, r2, -29984
    bl fn_80196E10
@L_80083214:
    lbz r3, 0x1a(r28)
    lbz r0, 0x4001(r30)
    cmplw r3, r0
    beq @L_8008322C
    addi r27, r27, 0x1
    b @L_80083120
@L_8008322C:
    cmplwi r28, 0x0
    lbz r27, 0x4002(r30)
    bne @L_80083248
    addi r3, r29, 0x0
    li r4, 0x17f
    addi r5, r2, -29984
    bl fn_80196E10
@L_80083248:
    extsb r3, r27
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_8008326C
    lbz r0, 0x1b(r28)
    extsb r0, r0
    cmpw r3, r0
    bge @L_8008326C
    li r4, 0x1
@L_8008326C:
    cmpwi r4, 0x0
    bne @L_80083284
    addi r3, r29, 0x0
    addi r5, r29, 0x10
    li r4, 0x180
    bl fn_80196E10
@L_80083284:
    lbz r3, 0x1c(r28)
    extsb r4, r27
    lbz r0, 0x1d(r28)
    li r5, 0x1
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r4, r0
    add r3, r28, r0
    stb r5, 0x95(r3)
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

