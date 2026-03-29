    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8000FF90
    li r4, 0x18
    bl fn_80103FFC
    cmplwi r3, 0x0
    beq @L_8000FF90
    lwz r4, 0x60(r31)
    li r5, 0x18
    bl memcpy
    @L_8000FF90:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
