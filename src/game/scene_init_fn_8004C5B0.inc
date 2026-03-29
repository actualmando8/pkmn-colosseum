    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    li r3, 0xa
    bl fn_80103E68
    srwi r0, r3, 16
    lis r3, lbl_802671D0@ha
    sth r0, 0x8(r1)
    addi r4, r3, lbl_802671D0@l
    li r5, 0x0
    sth r0, 0xc(r1)
    li r0, 0x2
    mtctr r0
    @L_8004C5EC:
    lha r3, 0x6(r31)
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8004C66C
    addi r4, r4, 0x4
    addi r5, r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8004C66C
    addi r4, r4, 0x4
    addi r5, r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8004C66C
    addi r4, r4, 0x4
    addi r5, r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8004C66C
    addi r4, r4, 0x4
    addi r5, r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8004C66C
    addi r4, r4, 0x4
    addi r5, r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8004C66C
    addi r4, r4, 0x4
    addi r5, r5, 0x1
    bdnz @L_8004C5EC
    @L_8004C66C:
    cmpwi r5, 0xc
    blt @L_8004C67C
    li r3, 0x0
    b @L_8004C6AC
    @L_8004C67C:
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpw r5, r0
    bne @L_8004C69C
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8004C6A8
    @L_8004C69C:
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_8004C6A8:
    li r3, 0x0
    @L_8004C6AC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
