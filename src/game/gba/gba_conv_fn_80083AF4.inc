    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    cmplwi r3, 0x0
    beq @L_80083B14
    b @L_80083B20
@L_80083B14:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
@L_80083B20:
    addi r0, r1, 0x8
    addi r6, r3, 0x4000
    cmplwi r0, 0x0
    beq @L_80083B38
    li r0, 0x0
    stw r0, 0x8(r1)
@L_80083B38:
    li r7, 0x0
@L_80083B3C:
    addi r0, r3, 0x24
    cmplw r6, r0
    blt @L_80083BD4
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80083BD4
    lbz r0, 0x1b(r3)
    extsb r0, r0
    cmpwi r0, 0x3
    bgt @L_80083B84
    lbz r0, 0x1c(r3)
    extsb r0, r0
    cmpwi r0, 0x6
    bgt @L_80083B84
    lbz r0, 0x1d(r3)
    extsb r0, r0
    cmpwi r0, 0x5
    ble @L_80083B90
@L_80083B84:
    li r0, 0x0
    sth r0, 0x0(r3)
    b @L_80083BD4
@L_80083B90:
    cmpw r7, r31
    bne @L_80083B9C
    stw r3, 0x8(r1)
@L_80083B9C:
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
    b @L_80083B3C
@L_80083BD4:
    cmpwi r31, 0x0
    bge @L_80083BE0
    stw r3, 0x8(r1)
@L_80083BE0:
    lwz r3, 0x8(r1)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
