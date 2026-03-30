    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    addi r0, r5, 0x1f
    mr r5, r3
    mr r6, r4
    li r4, 0x20
    clrrwi r3, r0, 5
    li r7, 0x0
    bl fn_800F9418
    cmplwi r3, 0x0
    bne @L_801142E8
    li r3, 0x0
@L_801142E8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
