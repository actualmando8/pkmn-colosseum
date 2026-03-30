    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0x5
    blt @L_800085F8
    lis r3, 0x1
    subi r3, r3, 0x149d
    b @L_80008668
    @L_800085F8:
    cmpwi r3, 0x0
    bne @L_8000860C
    lis r3, 0x1
    subi r0, r3, 0x12fc
    b @L_80008664
    @L_8000860C:
    cmpwi r3, 0x1
    bne @L_80008620
    lis r3, 0x1
    subi r0, r3, 0x12fb
    b @L_80008664
    @L_80008620:
    cmpwi r3, 0x2
    bne @L_80008634
    lis r3, 0x1
    subi r0, r3, 0x12fa
    b @L_80008664
    @L_80008634:
    cmpwi r3, 0x3
    bne @L_80008648
    lis r3, 0x1
    subi r0, r3, 0x12f9
    b @L_80008664
    @L_80008648:
    cmpwi r3, 0x4
    bne @L_8000865C
    lis r3, 0x1
    subi r0, r3, 0x12f8
    b @L_80008664
    @L_8000865C:
    lis r3, 0x1
    subi r0, r3, 0x12fc
    @L_80008664:
    mr r3, r0
    @L_80008668:
    cmplwi r3, 0x0
    bne @L_80008678
    lis r3, 0x1
    subi r3, r3, 0x149d
    @L_80008678:
    bl fn_800FA280
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
