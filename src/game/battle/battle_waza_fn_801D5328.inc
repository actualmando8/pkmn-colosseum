    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x58
    stw r0, 0x14(r1)
    bl fn_800F04C4
    lis r3, -32698
    li r0, 0x0
    addi r4, r3, 29688
    li r3, 0x0
    stw r0, 0x66c(r4)
    bl fn_801024E8
    lis r3, -32698
    li r0, 0x0
    addi r3, r3, 29688
    stw r0, 0xc(r3)
    lwz r3, 0x10(r3)
    bl fn_801684F0
    lis r3, -32698
    addi r3, r3, 29688
    lwz r3, 0x878(r3)
    cmplwi r3, 0x0
    beq @L_801D5384
    bl fn_800EF5A4
@L_801D5384:
    lis r3, -32698
    li r4, 0x0
    addi r3, r3, 29688
    li r5, 0x884
    bl memset
    lis r3, -32698
    addi r3, r3, 29688
    lbz r0, 0x87d(r3)
    cmplwi r0, 0x0
    bne @L_801D53B0
    bl fn_801E11F0
@L_801D53B0:
    lwz r3, -21544(r13)
    bl fn_800FE714
    li r0, 0x0
    stw r0, -21544(r13)
    bl fn_801DAC90
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
