    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xd6
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r4, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80201434
    li r3, -0x1
    b @L_80201468
    @L_80201434:
    bne @L_80201440
    li r3, 0x0
    b @L_80201450
    @L_80201440:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    @L_80201450:
    cmplwi r3, 0x0
    bne @L_80201460
    li r3, -0x1
    b @L_80201468
    @L_80201460:
    mr r4, r31
    bl fn_80120D6C
    @L_80201468:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
