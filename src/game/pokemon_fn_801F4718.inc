    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x5a
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_801F4768
    li r5, 0x0
    mr r4, r5
    b @L_801F475C
    @L_801F4750:
    clrlslwi r0, r5, 16, 2
    addi r5, r5, 0x1
    stwx r4, r3, r0
    @L_801F475C:
    clrlwi r0, r5, 16
    cmplwi r0, 0x8
    blt @L_801F4750
    @L_801F4768:
    mr r3, r31
    li r4, 0x0
    li r5, 0x5a
    li r6, 0x0
    bl fn_801F54A4
    mr. r4, r3
    bne @L_801F478C
    li r3, 0x0
    b @L_801F47A0
    @L_801F478C:
    mr r3, r31
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801F61EC
    @L_801F47A0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
