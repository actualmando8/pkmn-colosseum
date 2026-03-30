    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, 0x8028
    stw r0, 0x24(r1)
    addi r5, r3, -23432
    lwz r0, -20924(r13)
    lwz r4, 0x0(r5)
    cmpwi r0, 0x1
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    bne @L_8025E588
    bne @L_8025E588
    lwz r3, -20928(r13)
    li r4, 0x1
    bl fn_80118A68
    li r0, 0x0
    stw r0, -20928(r13)
    stw r0, -20924(r13)
@L_8025E588:
    bl fn_80113F48
    lis r4, 0x108a
    addi r4, r4, 0x1400
    bl fn_800F9318
    li r4, 0x0
    li r5, 0x0
    bl fn_801190DC
    stw r3, -20928(r13)
    addi r4, r1, 0x8
    bl fn_80118F04
    li r0, 0x1
    lfs f1, -20508(r2)
    stw r0, -20924(r13)
    bl fn_8025DC2C
    lwz r0, -20924(r13)
    cmpwi r0, 0x1
    bne @L_8025E5E4
    lwz r3, -20928(r13)
    li r4, 0x1
    bl fn_80118A68
    li r0, 0x0
    stw r0, -20928(r13)
    stw r0, -20924(r13)
@L_8025E5E4:
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
