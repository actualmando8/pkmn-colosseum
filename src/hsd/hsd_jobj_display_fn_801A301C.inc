    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr. r29, r3
    stw r28, 0x10(r1)
    mr r28, r4
    beq @L_801A321C
    cmplwi r28, 0x0
    beq @L_801A31D8
    lwz r0, 0x7c(r29)
    cmplwi r0, 0x0
    beq @L_801A3068
    lwz r3, 0x7c(r29)
    bl fn_801C25E4
@L_801A3068:
    lwz r3, 0x8(r28)
    bl fn_801C2670
    stw r3, 0x7c(r29)
    lwz r6, 0x7c(r29)
    cmplwi r6, 0x0
    beq @L_801A30D4
    lwz r0, 0x14(r6)
    cmplwi r0, 0x0
    beq @L_801A30D4
    addi r5, r6, 0x14
    b @L_801A30C8
@L_801A3094:
    lwz r3, 0x0(r5)
    lbz r0, 0x13(r3)
    cmplwi r0, 0xc
    bne @L_801A30C4
    lwz r3, 0x0(r5)
    lwz r4, 0x0(r5)
    lwz r0, 0x0(r3)
    stw r0, 0x0(r5)
    lwz r0, 0x14(r6)
    stw r0, 0x0(r4)
    stw r4, 0x14(r6)
    b @L_801A30D4
@L_801A30C4:
    lwz r5, 0x0(r5)
@L_801A30C8:
    lwz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_801A3094
@L_801A30D4:
    lwz r3, 0x80(r29)
    lwz r4, 0xc(r28)
    bl fn_801AFE68
    lwz r0, 0x10(r28)
    clrlwi. r0, r0, 31
    beq @L_801A3164
    cmplwi r29, 0x0
    beq @L_801A31D8
    lwz r0, 0x14(r29)
    xori r0, r0, 0x8
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A3154
    cmplwi r29, 0x0
    beq @L_801A3154
    cmplwi r29, 0x0
    bne @L_801A3124
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A3124:
    lwz r0, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A3144
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A3144
    li r3, 0x1
@L_801A3144:
    cmpwi r3, 0x0
    bne @L_801A3154
    mr r3, r29
    bl fn_8019D620
@L_801A3154:
    lwz r0, 0x14(r29)
    ori r0, r0, 0x8
    stw r0, 0x14(r29)
    b @L_801A31D8
@L_801A3164:
    cmplwi r29, 0x0
    beq @L_801A31D8
    lwz r0, 0x14(r29)
    xori r0, r0, 0x8
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A31CC
    cmplwi r29, 0x0
    beq @L_801A31CC
    cmplwi r29, 0x0
    bne @L_801A319C
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A319C:
    lwz r0, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A31BC
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A31BC
    li r3, 0x1
@L_801A31BC:
    cmpwi r3, 0x0
    bne @L_801A31CC
    mr r3, r29
    bl fn_8019D620
@L_801A31CC:
    lwz r0, 0x14(r29)
    rlwinm r0, r0, 0, 29, 27
    stw r0, 0x14(r29)
@L_801A31D8:
    lwz r0, 0x14(r29)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A321C
    cmplwi r31, 0x0
    beq @L_801A31FC
    lwz r5, 0x8(r31)
    b @L_801A3200
@L_801A31FC:
    li r5, 0x0
@L_801A3200:
    cmplwi r30, 0x0
    lwz r3, 0x18(r29)
    beq @L_801A3214
    lwz r4, 0x8(r30)
    b @L_801A3218
@L_801A3214:
    li r4, 0x0
@L_801A3218:
    bl fn_80199654
@L_801A321C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
