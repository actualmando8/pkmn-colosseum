    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r5, lbl_80266B9C@ha
    lwz r9, 0x60(r3)
    addi r3, r5, lbl_80266B9C@l
    li r10, 0x0
    lwz r8, 0x0(r3)
    lwz r7, 0x4(r3)
    lwz r6, 0x8(r3)
    lwz r5, 0xc(r3)
    lwz r3, 0x10(r3)
    lha r0, 0x6(r4)
    cmpw r0, r8
    beq @L_80014CA8
    li r10, 0x1
    cmpw r0, r7
    beq @L_80014CA8
    li r10, 0x2
    cmpw r0, r6
    beq @L_80014CA8
    li r10, 0x3
    cmpw r0, r5
    beq @L_80014CA8
    li r10, 0x4
    cmpw r0, r3
    beq @L_80014CA8
    li r10, 0x5
    @L_80014CA8:
    lwz r3, 0x8(r9)
    subfic r0, r3, 0x5
    subf r10, r0, r10
    cmpwi r10, 0x0
    blt @L_80014CC4
    cmpw r10, r3
    blt @L_80014CCC
    @L_80014CC4:
    li r3, 0x0
    b @L_80014D0C
    @L_80014CCC:
    mulli r0, r10, 0xc
    lwz r5, 0x4(r9)
    li r3, 0x0
    li r4, 0x0
    add r6, r5, r0
    lhz r0, 0x8(r6)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80014CFC
    lis r5, 0x7f7f
    addi r5, r5, 0x7fff
    b @L_80014D00
    @L_80014CFC:
    li r5, -0x1
    @L_80014D00:
    lwz r6, 0x0(r6)
    bl fn_800FB680
    li r3, 0x0
    @L_80014D0C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
