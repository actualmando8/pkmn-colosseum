    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    b @L_801BFD54
    @L_801BFD30:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x6
    bne @L_801BFD50
    b @L_801BFD60
    @L_801BFD50:
    addi r5, r5, 0x1
    @L_801BFD54:
    cmpwi r5, 0x3
    blt @L_801BFD30
    li r5, -0x1
    @L_801BFD60:
    mr r6, r5
    cmpwi r6, -0x1
    beq @L_801BFE40
    li r5, 0x0
    b @L_801BFD98
    @L_801BFD74:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x7
    bne @L_801BFD94
    b @L_801BFDA4
    @L_801BFD94:
    addi r5, r5, 0x1
    @L_801BFD98:
    cmpwi r5, 0x3
    blt @L_801BFD74
    li r5, -0x1
    @L_801BFDA4:
    mr r30, r5
    cmpwi r30, -0x1
    beq @L_801BFDC8
    mulli r4, r30, 0x60
    lis r3, lbl_80466BC0@ha
    li r5, 0x2
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    stw r5, 0x5c(r3)
    @L_801BFDC8:
    mulli r4, r6, 0x60
    lis r3, lbl_80466BC0@ha
    li r5, 0x7
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    li r6, 0x0
    stw r5, 0x5c(r3)
    b @L_801BFE0C
    @L_801BFDE8:
    mulli r4, r6, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x5
    bne @L_801BFE08
    b @L_801BFE18
    @L_801BFE08:
    addi r6, r6, 0x1
    @L_801BFE0C:
    cmpwi r6, 0x3
    blt @L_801BFDE8
    li r6, -0x1
    @L_801BFE18:
    mr r30, r6
    cmpwi r30, -0x1
    beq @L_801BFED4
    mulli r4, r30, 0x60
    lis r3, lbl_80466BC0@ha
    li r5, 0x6
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    stw r5, 0x5c(r3)
    b @L_801BFED4
    @L_801BFE40:
    li r5, 0x0
    b @L_801BFE6C
    @L_801BFE48:
    mulli r4, r5, 0x60
    lis r3, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    lwz r0, 0x5c(r3)
    cmpwi r0, 0x8
    bne @L_801BFE68
    b @L_801BFE78
    @L_801BFE68:
    addi r5, r5, 0x1
    @L_801BFE6C:
    cmpwi r5, 0x3
    blt @L_801BFE48
    li r5, -0x1
    @L_801BFE78:
    mr r30, r5
    cmpwi r30, -0x1
    beq @L_801BFED4
    mulli r4, r30, 0x60
    lis r3, lbl_80466BC0@ha
    lis r5, lbl_80466BC0@ha
    addi r0, r3, lbl_80466BC0@l
    add r3, r0, r4
    addi r5, r5, lbl_80466BC0@l
    lwz r4, 0x58(r3)
    addi r3, r5, 0x17c
    li r5, 0x0
    bl fn_801BFA1C
    mulli r5, r30, 0x60
    lis r4, lbl_80466BC0@ha
    lis r3, lbl_80466BC0@ha
    addi r0, r4, lbl_80466BC0@l
    add r4, r0, r5
    li r0, 0x7
    stw r0, 0x5c(r4)
    addi r3, r3, lbl_80466BC0@l
    li r0, 0x0
    stw r0, 0x178(r3)
    @L_801BFED4:
    lis r3, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r0, 0x1dc(r3)
    cmplwi r0, 0x0
    beq @L_801BFF00
    lis r4, lbl_80466BC0@ha
    mr r3, r31
    addi r4, r4, lbl_80466BC0@l
    lwz r12, 0x1dc(r4)
    mtctr r12
    bctrl
    @L_801BFF00:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
