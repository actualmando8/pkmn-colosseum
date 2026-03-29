    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r0, 0x4(r3)
    li r31, 0x1
    cmpwi r0, 0xf7
    beq @L_8000F3A4
    cmpwi r0, 0xf8
    bne @L_8000F3A4
    li r4, 0x2
    bl fn_801040D0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000F3A4
    li r31, 0x0
    @L_8000F3A4:
    lha r3, 0x6(r30)
    subi r0, r3, 0x11ce
    cmplwi r0, 0x9
    bgt @L_8000F3E8
    lis r3, jumptable_802E4C80@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E4C80@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8000F3E8
    mr r3, r30
    mr r4, r31
    bl fn_80109220
    @L_8000F3E8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
