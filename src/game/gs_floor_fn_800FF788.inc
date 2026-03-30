    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -23368(r13)
    lwz r5, -23384(r13)
    cmpwi r0, 0x0
    bne @L_800FF80C
    lwz r0, -23372(r13)
    lwz r4, -23376(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800FF7D0
@L_800FF7B8:
    lwz r0, 0xc(r4)
    cmplw r0, r3
    bne @L_800FF7C8
    b @L_800FF7D4
@L_800FF7C8:
    addi r4, r4, 0x4c
    bdnz @L_800FF7B8
@L_800FF7D0:
    li r4, 0x0
@L_800FF7D4:
    cmplwi r4, 0x0
    bne @L_800FF7F4
    lis r5, -32729
    mr r4, r3
    addi r3, r5, 6128
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FF80C
@L_800FF7F4:
    stw r4, 0x0(r5)
    addi r3, r3, 0x5001
    li r0, 0x1
    stw r3, 0x4(r5)
    stw r0, 0x10(r5)
    stw r0, -23368(r13)
@L_800FF80C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
