    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    mr r30, r4
    cmplwi r31, 0x0
    bne @L_8008301C
    lis r3, lbl_8026F1C8@ha
    li r4, 0x17f
    addi r3, r3, lbl_8026F1C8@l
    addi r5, r2, -29984
    bl fn_80196E10
@L_8008301C:
    extsb r3, r30
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_80083040
    lbz r0, 0x1b(r31)
    extsb r0, r0
    cmpw r3, r0
    bge @L_80083040
    li r4, 0x1
@L_80083040:
    cmpwi r4, 0x0
    bne @L_80083060
    lis r3, lbl_8026F1C8@ha
    lis r5, lbl_8026F1D8@ha
    addi r3, r3, lbl_8026F1C8@l
    li r4, 0x180
    addi r5, r5, lbl_8026F1D8@l
    bl fn_80196E10
@L_80083060:
    lbz r3, 0x1c(r31)
    extsb r4, r30
    lbz r0, 0x1d(r31)
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r4, r0
    add r3, r31, r0
    addi r3, r3, 0x24
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

