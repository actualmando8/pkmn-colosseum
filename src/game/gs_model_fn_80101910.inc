    nofralloc
    lis r4, lbl_80402518@ha
    li r0, 0x20
    addi r4, r4, lbl_80402518@l
    li r7, 0x0
    mr r6, r4
    mtctr r0
@L_80101928:
    lwz r5, 0x44(r6)
    cmpwi r5, 0x0
    beq @L_80101954
    lwz r4, 0x40(r6)
    lwz r0, 0x40(r3)
    cmplw r4, r0
    bne @L_80101954
    subi r0, r5, 0x1
    li r3, 0x1
    stw r0, 0x44(r6)
    blr
@L_80101954:
    lwz r5, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r5, 0x0
    beq @L_80101984
    lwz r4, 0x40(r6)
    lwz r0, 0x40(r3)
    cmplw r4, r0
    bne @L_80101984
    subi r0, r5, 0x1
    li r3, 0x1
    stw r0, 0x44(r6)
    blr
@L_80101984:
    lwz r5, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r5, 0x0
    beq @L_801019B4
    lwz r4, 0x40(r6)
    lwz r0, 0x40(r3)
    cmplw r4, r0
    bne @L_801019B4
    subi r0, r5, 0x1
    li r3, 0x1
    stw r0, 0x44(r6)
    blr
@L_801019B4:
    lwz r5, 0x8c(r6)
    addi r6, r6, 0x48
    cmpwi r5, 0x0
    beq @L_801019E4
    lwz r4, 0x40(r6)
    lwz r0, 0x40(r3)
    cmplw r4, r0
    bne @L_801019E4
    subi r0, r5, 0x1
    li r3, 0x1
    stw r0, 0x44(r6)
    blr
@L_801019E4:
    addi r7, r7, 0x3
    addi r6, r6, 0x48
    bdnz @L_80101928
    li r3, 0x1
    blr
