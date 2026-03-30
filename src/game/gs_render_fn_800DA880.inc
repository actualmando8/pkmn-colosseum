    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r10, 0x0(r3)
    cmpwi r10, 0x15
    bge @L_800DA8B4
    cmpwi r10, 0x9
    bge @L_800DA934
    cmpwi r10, 0x0
    bge @L_800DA8C0
    b @L_800DAC88
    @L_800DA8B4:
    cmpwi r10, 0x19
    beq @L_800DA934
    b @L_800DAC88
    @L_800DA8C0:
    cmplwi r10, 0x8
    lis r8, 0x1
    lis r9, 0x1000
    bgt @L_800DA92C
    lis r3, jumptable_80315340@ha
    slwi r0, r10, 2
    addi r3, r3, jumptable_80315340@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r7, 0x1
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    b @L_800DA92C
    li r7, 0x40
    @L_800DA92C:
    addi r31, r31, 0x1
    b @L_800DAC88
    @L_800DA934:
    cmpwi r10, 0xb
    beq @L_800DA944
    cmpwi r10, 0xc
    bne @L_800DA9C4
    @L_800DA944:
    cmpwi r10, 0xb
    li r4, 0x20
    bne @L_800DA954
    li r4, 0x10
    @L_800DA954:
    lwz r10, 0xc(r3)
    mr r7, r4
    lis r9, 0x1000
    cmpwi r10, 0x3
    beq @L_800DA9AC
    bge @L_800DA984
    cmpwi r10, 0x1
    beq @L_800DA99C
    bge @L_800DA9A4
    cmpwi r10, 0x0
    bge @L_800DA994
    b @L_800DAB1C
    @L_800DA984:
    cmpwi r10, 0x5
    beq @L_800DA9BC
    bge @L_800DAB1C
    b @L_800DA9B4
    @L_800DA994:
    lis r8, 0x20
    b @L_800DAB1C
    @L_800DA99C:
    lis r8, 0x40
    b @L_800DAB1C
    @L_800DA9A4:
    lis r8, 0x80
    b @L_800DAB1C
    @L_800DA9AC:
    lis r8, 0x100
    b @L_800DAB1C
    @L_800DA9B4:
    lis r8, 0x200
    b @L_800DAB1C
    @L_800DA9BC:
    lis r8, 0x400
    b @L_800DAB1C
    @L_800DA9C4:
    lwz r8, 0xc(r3)
    cmpwi r8, 0x2
    beq @L_800DAA04
    bge @L_800DA9E4
    cmpwi r8, 0x0
    beq @L_800DA9F4
    bge @L_800DA9FC
    b @L_800DAA18
    @L_800DA9E4:
    cmpwi r8, 0x4
    beq @L_800DAA14
    bge @L_800DAA18
    b @L_800DAA0C
    @L_800DA9F4:
    lis r4, 0x1
    b @L_800DAA18
    @L_800DA9FC:
    lis r4, 0x2
    b @L_800DAA18
    @L_800DAA04:
    lis r4, 0x4
    b @L_800DAA18
    @L_800DAA0C:
    lis r4, 0x8
    b @L_800DAA18
    @L_800DAA14:
    lis r4, 0x10
    @L_800DAA18:
    cmpwi r10, 0xd
    mr r8, r4
    bge @L_800DAA3C
    cmpwi r10, 0xa
    beq @L_800DAA74
    bge @L_800DAB1C
    cmpwi r10, 0x9
    bge @L_800DAA54
    b @L_800DAB1C
    @L_800DAA3C:
    cmpwi r10, 0x19
    beq @L_800DAA8C
    bge @L_800DAB1C
    cmpwi r10, 0x15
    bge @L_800DAB1C
    b @L_800DAAA4
    @L_800DAA54:
    lwz r4, 0x8(r3)
    li r7, 0x2
    cmpwi r4, 0x0
    bne @L_800DAA6C
    lis r9, 0x2000
    b @L_800DAB1C
    @L_800DAA6C:
    lis r9, 0x4000
    b @L_800DAB1C
    @L_800DAA74:
    lwz r4, 0x8(r3)
    li r7, 0x4
    cmpwi r4, 0x0
    bne @L_800DAB1C
    lis r9, 0x4000
    b @L_800DAB1C
    @L_800DAA8C:
    lwz r4, 0x8(r3)
    li r7, 0x8
    cmpwi r4, 0x1
    bne @L_800DAB1C
    lis r9, 0x4000
    b @L_800DAB1C
    @L_800DAAA4:
    subi r9, r10, 0xd
    cmplwi r9, 0x7
    bgt @L_800DAB04
    lis r4, jumptable_80315320@ha
    slwi r9, r9, 2
    addi r4, r4, jumptable_80315320@l
    lwzx r4, r4, r9
    mtctr r4
    bctr
    li r7, 0x80
    b @L_800DAB04
    li r7, 0x100
    b @L_800DAB04
    li r7, 0x200
    b @L_800DAB04
    li r7, 0x400
    b @L_800DAB04
    li r7, 0x800
    b @L_800DAB04
    li r7, 0x1000
    b @L_800DAB04
    li r7, 0x2000
    b @L_800DAB04
    li r7, 0x4000
    @L_800DAB04:
    lwz r4, 0x8(r3)
    cmpwi r4, 0x0
    bne @L_800DAB18
    lis r9, 0x1000
    b @L_800DAB1C
    @L_800DAB18:
    lis r9, 0x2000
    @L_800DAB1C:
    lwz r4, 0x4(r3)
    cmpwi r4, 0x1
    bne @L_800DAC58
    lis r3, 0x20
    mr r4, r31
    cmpw r8, r3
    beq @L_800DABE4
    bge @L_800DAB8C
    lis r3, 0x4
    cmpw r8, r3
    beq @L_800DABE4
    bge @L_800DAB6C
    lis r3, 0x2
    cmpw r8, r3
    beq @L_800DABDC
    bge @L_800DABF8
    lis r3, 0x1
    cmpw r8, r3
    beq @L_800DABDC
    b @L_800DABF8
    @L_800DAB6C:
    lis r3, 0x10
    cmpw r8, r3
    beq @L_800DABF4
    bge @L_800DABF8
    lis r3, 0x8
    cmpw r8, r3
    beq @L_800DABE4
    b @L_800DABF8
    @L_800DAB8C:
    lis r3, 0x100
    cmpw r8, r3
    beq @L_800DABE4
    bge @L_800DABBC
    lis r3, 0x80
    cmpw r8, r3
    beq @L_800DABF4
    bge @L_800DABF8
    lis r3, 0x40
    cmpw r8, r3
    beq @L_800DABEC
    b @L_800DABF8
    @L_800DABBC:
    lis r3, 0x400
    cmpw r8, r3
    beq @L_800DABF4
    bge @L_800DABF8
    lis r3, 0x200
    cmpw r8, r3
    beq @L_800DABEC
    b @L_800DABF8
    @L_800DABDC:
    li r0, 0x1
    b @L_800DABF8
    @L_800DABE4:
    li r0, 0x2
    b @L_800DABF8
    @L_800DABEC:
    li r0, 0x3
    b @L_800DABF8
    @L_800DABF4:
    li r0, 0x4
    @L_800DABF8:
    lis r3, 0x2000
    cmpw r9, r3
    beq @L_800DAC3C
    bge @L_800DAC2C
    lis r3, 0x1000
    cmpw r9, r3
    beq @L_800DAC50
    bge @L_800DAC50
    lis r3, 0x8000
    addi r3, r3, 0x1
    cmpw r9, r3
    bge @L_800DAC50
    b @L_800DAC4C
    @L_800DAC2C:
    lis r3, 0x4000
    cmpw r9, r3
    beq @L_800DAC44
    b @L_800DAC50
    @L_800DAC3C:
    slwi r0, r0, 1
    b @L_800DAC50
    @L_800DAC44:
    mulli r0, r0, 0x3
    b @L_800DAC50
    @L_800DAC4C:
    slwi r0, r0, 2
    @L_800DAC50:
    add r31, r31, r0
    b @L_800DAC88
    @L_800DAC58:
    cmpwi r4, 0x2
    bne @L_800DAC6C
    lbz r4, 0x0(r31)
    addi r31, r31, 0x1
    b @L_800DAC74
    @L_800DAC6C:
    lhz r4, 0x0(r31)
    addi r31, r31, 0x2
    @L_800DAC74:
    lhz r0, 0x12(r3)
    clrlwi r4, r4, 16
    lwz r3, 0x14(r3)
    mullw r0, r4, r0
    add r4, r3, r0
    @L_800DAC88:
    lwz r12, 0x8(r5)
    cmplwi r12, 0x0
    beq @L_800DACA8
    or r0, r7, r8
    mr r5, r6
    or r3, r9, r0
    mtctr r12
    bctrl
    @L_800DACA8:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
