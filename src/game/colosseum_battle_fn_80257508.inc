    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r7, 0x0
    stw r0, 0x14(r1)
    bl fn_8025C264
    cmpwi r3, 0x0
    bne @L_8025752C
    li r3, 0x0
    b @L_80257534
@L_8025752C:
    cmpwi r3, -0x1
    li r3, 0x1
@L_80257534:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
