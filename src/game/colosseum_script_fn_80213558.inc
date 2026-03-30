    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x0(r5)
    cmplwi r0, 0x1
    bne @L_80213590
    li r4, 0x2b
    bl fn_80202810
    mr r3, r30
    li r4, 0x2c
    bl fn_80202810
    b @L_80213658
@L_80213590:
    bl fn_802075A4
    mr r3, r30
    li r4, 0x32
    bl fn_80202810
    mr r3, r30
    li r4, 0x37
    bl fn_80202810
    mr r3, r30
    li r4, 0x33
    bl fn_80202810
    mr r3, r30
    li r4, 0x0
    li r5, 0xed
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r3, r3, 0, 16, 31
    beq @L_802135F0
    subi r0, r3, 0x1
    mr r3, r30
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0xed
    li r6, 0x0
    bl fn_801254B4
@L_802135F0:
    mr r3, r30
    li r4, 0x12
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213658
    mr r3, r30
    li r4, 0x12
    bl fn_80202108
    mr r31, r3
    mr r3, r30
    li r4, 0x12
    bl fn_80202234
    extsb r4, r31
    extsb r3, r3
    addi r0, r4, 0x1
    extsb r5, r0
    cmpw r5, r3
    blt @L_8021364C
    mr r3, r30
    li r4, 0x12
    bl fn_80202810
    b @L_80213658
@L_8021364C:
    mr r3, r30
    li r4, 0x12
    bl fn_80201FDC
@L_80213658:
    mr r3, r30
    li r4, 0x14
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021368C
    mr r3, r30
    bl fn_801FEF74
    cmpwi r3, 0x0
    bgt @L_8021368C
    mr r3, r30
    li r4, 0x14
    bl fn_80202810
@L_8021368C:
    lmw r30, 0x8(r1)
    li r3, 0x1
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
