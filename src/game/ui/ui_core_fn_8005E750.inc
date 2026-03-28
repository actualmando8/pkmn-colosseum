    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r4, 0x0
    bl fn_80061F6C
    lis r3, lbl_803A9A60@ha
    li r0, 0xba
    addi r4, r3, lbl_803A9A60@l
    li r3, 0x0
    sth r0, 0x8(r4)
    bl fn_80103CC0
    li r3, 0xdf
    li r4, 0x0
    bl fn_8010264C
    li r3, 0xba
    li r4, 0x1
    bl fn_8010264C
    li r3, 0x1
    bl fn_80103CC0
    li r3, 0xba
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lis r3, lbl_803A9A60@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9A60@l
    stw r0, 0x4(r3)
    lwz r0, 0x4(r31)
    cmpwi r0, 0x2
    beq @L_8005E7D8
    li r3, 0xc4
    b @L_8005E7DC
@L_8005E7D8:
    li r3, 0xc6
@L_8005E7DC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
